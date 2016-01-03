#include "SessionFactoryI.h"

#include <Ice/Communicator.h>
#include <Ice/Current.h>
#include <Ice/Handle.h>
#include <Ice/Identity.h>
#include <Ice/ObjectAdapter.h>
#include <Ice/ProxyHandle.h>

#include "Glacier2SessionManagerI.h"

using namespace std;
using namespace Cardroid::Network::Server;

boost::mutex SessionFactoryI::_synchronized;

/**
 * Default constructor
 * @param topicManager	Proxy to the IceStorm topic manager
 * @param logger		Logger to post events and errors related to the
 * 							session management, creation and destruction
 * @param reaper		Reference to the thread responsible for destroying
 * 							idle sessions, as well as reaping all the
 * 							sessions alive at the time the router is
 * 							shutdown
 * @param timeout		The time of inactivity a session needs to be
 * 							considered to reach an idle state
 */
SessionFactoryI::SessionFactoryI(const IceStorm::TopicManagerPrx& topicManager,
		const ::Ice::LoggerPtr& logger, ReapThread& reaper, long long timeout) :
		_topicManager(topicManager), _logger(logger), _reaper(reaper), _timeout(
				timeout) {
}

cardroid::network::zerocice::SessionPrx SessionFactoryI::create(
		const ::Ice::Current& current) {
	_synchronized.lock();

	IceStorm::TopicPrx topic = retrieveTopic(current.id.name);
	SessionI* session = new SessionI(_logger, true, current.adapter, topic,
			current.id.name, *this, true);
	cardroid::network::zerocice::SessionPrx proxy =
			cardroid::network::zerocice::SessionPrx::uncheckedCast(
					current.adapter->addWithUUID(session));

	_logger->trace("SessionFactory",
			"Create new session: "
					+ current.adapter->getCommunicator()->identityToString(
							proxy->ice_getIdentity()));

	// Add the newly created session to the reaper's record
	_reaper.add(proxy, *session, current.id.name);

	_synchronized.unlock();

	return proxy;

}

void SessionFactoryI::destroy(const SessionI& session, const string& userName) {
	_synchronized.lock();
	_reaper.remove(session, retrieveTopic(userName));
	_synchronized.unlock();
}

long long SessionFactoryI::getSessionTimeout(const ::Ice::Current&) {
	return _timeout;
}

/**
 * Retrieve the IceStorm topic corresponding to the user creating the new
 * session. Create the topic if it doesn't exist. Such a topic will be
 * used for publishing event notifications destined to the clients on
 * which active sessions for the user are running
 * @param name	The user id, which will act as the name for the topic
 * @return		A proxy to the user's topic
 */
IceStorm::TopicPrx SessionFactoryI::retrieveTopic(const string& name) {
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
