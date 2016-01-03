#include "Glacier2SessionManagerI.h"

#include <Ice/Communicator.h>
#include <Ice/Current.h>
#include <Ice/Handle.h>
#include <Ice/Identity.h>
#include <Ice/ObjectAdapter.h>
#include <Ice/ProxyHandle.h>

using namespace std;
using namespace Cardroid::Network::Server;

boost::mutex Glacier2SessionManagerI::_synchronized;

/**
 * Default constructor
 * @param topicManager	Proxy to the IceStorm topic manager
 * @param logger		Logger to post events and error related to the
 * 							session management, creation and destruction
 * @param reaper		Reference to the thread responsible for destroying
 * 							idle sessions, as well as reaping all the
 * 							sessions alive at the time the router is
 * 							shutdown
 */
Glacier2SessionManagerI::Glacier2SessionManagerI(
		const IceStorm::TopicManagerPrx& topicManager, const ::Ice::LoggerPtr& logger,
		ReapThread& reaper) :
		_topicManager(topicManager), _logger(logger), _reaper(reaper) {
}

/**
 * Create a new session
 * @param userId	The user id for the session
 * @param control	A proxy to the session control object
 * @return			A proxy to the newly created session
 */
Glacier2::SessionPrx Glacier2SessionManagerI::create(const string& userId,
		const Glacier2::SessionControlPrx&, const ::Ice::Current& current) {
	_synchronized.lock();

	IceStorm::TopicPrx topic = retrieveTopic(userId);
	SessionI* session = new SessionI(_logger, true, current.adapter, topic,
			userId, *this, true);
	Glacier2::SessionPrx proxy = Glacier2::SessionPrx::uncheckedCast(
			current.adapter->addWithUUID(session));

	_logger->trace("SessionFactory",
			"Create new session: "
					+ current.adapter->getCommunicator()->identityToString(
							proxy->ice_getIdentity()));

	// Add the newly created session to the reaper's record
	_reaper.add(proxy, *session, userId);

	_synchronized.unlock();

	return proxy;
}

/**
 * Destroy an existing session
 * @param session	The user session to be destroyed
 * @param userName	The user id for the session
 */
void Glacier2SessionManagerI::destroy(const SessionI& session,
		const string& userName) {
	_synchronized.lock();
	_reaper.remove(session, retrieveTopic(userName));
	_synchronized.unlock();
}

IceStorm::TopicPrx Glacier2SessionManagerI::retrieveTopic(const string& name) {
	IceStorm::TopicPrx topic;
	while (!topic) {
		try {
			topic = _topicManager->retrieve(name);
		} catch (const IceStorm::NoSuchTopic&) {
			try {
				topic = _topicManager->create(name);
			} catch (const IceStorm::TopicExists&) {
				/*
				 * Another session for the user is active and created the topic
				 */
			}
		}
	}
	return topic;
}
