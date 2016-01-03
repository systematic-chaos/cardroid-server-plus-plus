#ifndef REAPTHREAD_H_
#define REAPTHREAD_H_

#include <boost/thread/mutex.hpp>
#include <Glacier2/Session.h>
#include <Ice/Ice.h>
#include <IceStorm/IceStorm.h>
#include <forward_list>
#include <string>
#include <unordered_map>

#include "Session.h"
#include "SessionI.h"

namespace Cardroid {
namespace Network {
namespace Server {

class SessionProxyPair;

/**
 * \class ReapThread
 * Thread responsible for the destruction of idle user sessions
 * It also keeps a record of the currently active user sessions and manages
 * the destruction of IceStorm topics when no sessions for the user a topic is
 * referred to are active
 */
class ReapThread {
public:

	ReapThread(::Ice::LoggerPtr logger, ::IceStorm::TopicManagerPrx topicManager,
			long timeout);
	void run();
	void terminate();
	void add(const ::Glacier2::SessionPrx& proxy, SessionI& session,
			const string& userName);
	void add(const cardroid::network::zerocice::SessionPrx& proxy,
			SessionI& session, const string& userName);
	void remove(const SessionI& servant, ::IceStorm::TopicPrx topic);

private:

	::Ice::LoggerPtr _logger;
	::IceStorm::TopicManagerPrx _topicManager;
	const long _timeout;
	bool _terminated = false;
	/// Data structure for keeping track of the currently active user sessions
	unordered_multimap<string, SessionProxyPair*> _sessions;
	static boost::mutex _synchronized;

	void logSessionDestruction(const ::Ice::ObjectPrx sessionProxy);
	void destroyTopic(const string& name);
	void destroyAllTopics();
};

/**
 * \class SessionProxyPair
 * Class containing a proxy to a session (created via a Glacier2 router or
 * via a session factory) and the servant's session
 */
class SessionProxyPair {

public:

	::Glacier2::SessionPrx glacier2proxy;
	cardroid::network::zerocice::SessionPrx proxy;
	SessionI& session;

	SessionProxyPair(const ::Glacier2::SessionPrx& p, SessionI& s);
	SessionProxyPair(const cardroid::network::zerocice::SessionPrx& p, SessionI& s);
};
}
}
}

#endif /* REAPTHREAD_H_ */
