#include "ReapThread.h"

#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/date_time/microsec_time_clock.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/time.hpp>
#include <boost/date_time/time_duration.hpp>
#include <boost/chrono/duration.hpp>
#include <Ice/Communicator.h>
#include <Ice/Identity.h>
#include <Ice/LocalException.h>
#include <Ice/Logger.h>
#include <Ice/ProxyHandle.h>
#include <forward_list>
#include <utility>

using namespace std;
using namespace Cardroid::Network::Server;

boost::mutex ReapThread::_synchronized;

/**
 * Default constructor
 * @param logger		The logger used to post event or error messages
 * @param topicManager	Proxy to the IceStorm topic manager
 * @param timeout		The time of inactivity a session needs to be
 * 							removed due to an idle state
 */
ReapThread::ReapThread(::Ice::LoggerPtr logger,
		IceStorm::TopicManagerPrx topicManager, long timeout) :
		_logger(logger), _topicManager(topicManager), _timeout(timeout) {
	_sessions = unordered_multimap<string, SessionProxyPair*>();
}

void ReapThread::run() {
	_synchronized.lock();
	while (!_terminated) {
		// This thread will execute once every 500 ms
		long timeout = _timeout;
		_synchronized.unlock();
		boost::this_thread::sleep_for(
				boost::chrono::milliseconds(timeout * 500));

		_synchronized.lock();
		if (!_terminated) {
			for (auto sessionsIt = _sessions.begin(); sessionsIt != _sessions.end();) {
				bool increase = true;
				try {
					/* Session destruction may take time. Therefore the *
					 * currentEntry time is computed for each iteration */
					boost::posix_time::time_duration diff =
							boost::posix_time::microsec_clock::universal_time()
									- boost::posix_time::ptime(boost::gregorian::date(1970, 1, 1));
					timeout = diff.total_milliseconds() -
							sessionsIt->second->session.getSessionTimeout();
					if (timeout > _timeout * 1000) {
						// This is a Glacier2 session
						if (&sessionsIt->second->glacier2proxy) {
							logSessionDestruction(
									sessionsIt->second->glacier2proxy);
							sessionsIt->second->glacier2proxy->destroy();
						}

						// this is a factory's session
						if (&sessionsIt->second->proxy) {
							logSessionDestruction(sessionsIt->second->proxy);
							sessionsIt->second->proxy->destroy();
						}

						increase = false;
					}
				} catch (::Ice::ObjectNotExistException& e) {
					increase = false;
				}

				if (increase) {
					++sessionsIt;
				} else {
					// No sessions for the user are still active
					if (_sessions.count(sessionsIt->first) <= 1) {
						destroyTopic(sessionsIt->first);
					}
					sessionsIt = _sessions.erase(sessionsIt);
				}
			}
		}

		_synchronized.unlock();
	}
}

/**
 * Destroy each of the sessions, releasing any resources they may hold.
 * This calls directly on the session, not via the proxy since
 * terminate() is called after the communicator is shutdown, which means
 * call on collocated objects are not permitted.
 */
void ReapThread::terminate() {
	_synchronized.lock();

	_terminated = true;

	for (auto sessionsIt = _sessions.begin(); sessionsIt != _sessions.end();
			++sessionsIt) {
		sessionsIt->second->session.shutdown();
	}
	destroyAllTopics();
	_sessions.clear();

	_synchronized.unlock();
}

/**
 * Add a new user session to the reaper's record.
 * This method is called by a Glacier2 session manager
 * @param proxy		Proxy to the new user session
 * @param session	Servant of the new user session
 * @param userName	New session's owner user id
 */
void ReapThread::add(const Glacier2::SessionPrx& proxy, SessionI& session,
		const string& userName) {
	_synchronized.lock();

	_sessions.emplace(userName, new SessionProxyPair(proxy, session));

	_synchronized.unlock();
}

/**
 * Add a new user session to the reaper's record.
 * This method is called by a Glacier2 session manager
 * @param proxy		Proxy to the new user session
 * @param session	Servant of the new user session
 * @param userName	New session's owner user id
 */
void ReapThread::add(const cardroid::network::zerocice::SessionPrx& proxy,
		SessionI& session, const string& userName) {
	_synchronized.lock();

	_sessions.emplace(userName, new SessionProxyPair(proxy, session));

	_synchronized.unlock();
}

/**
 * Destroy a session and remove it from the reaper's record
 * @param servant	The servant of the session to be destroyed and removed
 * @param topic		Proxy to the topic this session is subscripted to
 */
void ReapThread::remove(const SessionI& servant, IceStorm::TopicPrx topic) {
	_synchronized.lock();

	auto sessionsItBounds = _sessions.equal_range(topic->getName());
	if (sessionsItBounds.first == sessionsItBounds.second) {
		return;
	}
	for (auto sessionsIt = sessionsItBounds.first;
			sessionsIt != sessionsItBounds.second;) {
		bool increase = true;
		try {
			if (&(sessionsIt->second->session) == &servant) {

				// This is a Glacier2 session
				if (sessionsIt->second->glacier2proxy) {
					logSessionDestruction(sessionsIt->second->glacier2proxy);
					sessionsIt->second->glacier2proxy->destroy();
				}

				// This is a factory session
				if (sessionsIt->second->proxy) {
					logSessionDestruction(sessionsIt->second->proxy);
					sessionsIt->second->proxy->destroy();
				}

				increase = false;
			}
		} catch (::Ice::ObjectNotExistException& e) {
			increase = false;
		}
		if (increase) {
			++sessionsIt;
		} else {
			// No sessions for the user are still active
			if (_sessions.count(sessionsIt->first) <= 1) {
				destroyTopic(sessionsIt->first);
			}
			sessionsIt = _sessions.erase(sessionsIt);
		}
	}

	_synchronized.unlock();
}

SessionProxyPair::SessionProxyPair(const Glacier2::SessionPrx& p, SessionI& s) :
		glacier2proxy(p), session(s) {
}

SessionProxyPair::SessionProxyPair(
		const cardroid::network::zerocice::SessionPrx& p, SessionI& s) :
		proxy(p), session(s) {
}

void ReapThread::logSessionDestruction(::Ice::ObjectPrx sessionProxy) {
	_logger->trace("ReapThread",
			"The session "
					+ sessionProxy->ice_getCommunicator()->identityToString(
							sessionProxy->ice_getIdentity())
					+ " has timed out.");
}

/**
 * Destroy the specified topic, if exists
 * @param name The name of the topic to destroy
 */
void ReapThread::destroyTopic(const string& name) {
	try {
		_topicManager->retrieve(name)->destroy();
	} catch (IceStorm::NoSuchTopic& nst) {
	}
}

void ReapThread::destroyAllTopics() {
	map<string, IceStorm::TopicPrx> topicsDict = _topicManager->retrieveAll();
	for (auto topicsIt = topicsDict.begin(); topicsIt != topicsDict.end(); topicsIt++)
		topicsIt->second->destroy();
}
