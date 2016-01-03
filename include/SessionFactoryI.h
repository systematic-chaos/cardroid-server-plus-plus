#ifndef SESSIONFACTORYI_H_
#define SESSIONFACTORYI_H_

#include <boost/thread/mutex.hpp>
#include <Ice/Logger.h>
#include <IceStorm/IceStorm.h>
#include <string>

#include "Session.h"
#include "SessionManager.h"
#include "SessionI.h"
#include "ReapThread.h"

namespace Cardroid {
namespace Network {
namespace Server {

/**
 * \class SessionFactoryI
 * The session manager for users responsible for creating session objects.
 * This class purpose is the creation of sessions for clients running on
 * platforms no compatible with Glacier2. Since these users sessions cannot
 * be created via a Glacier2 router, they are programmatically created here
 */
class SessionFactoryI: public cardroid::network::zerocice::SessionFactory,
	public SessionManager {

public:

	SessionFactoryI(const ::IceStorm::TopicManagerPrx& topicManager,
			const ::Ice::LoggerPtr& logger, ReapThread& reaper, long long timeout);
	virtual cardroid::network::zerocice::SessionPrx create
			(const ::Ice::Current& current = ::Ice::Current());
	virtual void destroy(const SessionI& session, const ::std::string& userName);
	virtual long long getSessionTimeout(const ::Ice::Current& = ::Ice::Current());

private:

	::IceStorm::TopicManagerPrx _topicManager;
	::Ice::LoggerPtr _logger;
	ReapThread& _reaper;
	long long _timeout;
	static ::boost::mutex _synchronized;

	static const long long int serialVersionUID = 3509017929914135668L;

	::IceStorm::TopicPrx retrieveTopic(const ::std::string& name);
};
}
}
}

#endif /* SESSIONFACTORYI_H_ */
