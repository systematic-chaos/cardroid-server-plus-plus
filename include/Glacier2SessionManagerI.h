#ifndef GLACIER2SESSIONAMANAGERI_H_
#define GLACIER2SESSIONAMANAGERI_H_

#include <boost/thread/mutex.hpp>
#include <Glacier2/Session.h>
#include <Ice/Logger.h>
#include <IceStorm/IceStorm.h>
#include <string>

#include "ReapThread.h"
#include "SessionManager.h"
#include "SessionI.h"

namespace Cardroid {
namespace Network {
namespace Server {

/**
 * \class Glacier2SessionManagerI
 * The session manager for username/password authenticated users that is
 * responsible for managing Glacier2.Session objects. New session objects are
 * created by the Glacier2.Router object calling on an application-provided
 * session manager. If no session manager is provided by the application, no
 * client-visible sessions are passed to the client.
 */
class Glacier2SessionManagerI: public ::Glacier2::SessionManager,
	public SessionManager {

public:

	Glacier2SessionManagerI(const ::IceStorm::TopicManagerPrx& topicManager,
			const ::Ice::LoggerPtr& logger, ReapThread& reaper);

	virtual ::Glacier2::SessionPrx create(const ::std::string& userId,
			const ::Glacier2::SessionControlPrx&, const ::Ice::Current& current = ::Ice::Current());

	virtual void destroy(const SessionI& session, const ::std::string& userName);

private:
	::IceStorm::TopicManagerPrx _topicManager;
	::Ice::LoggerPtr _logger;
	ReapThread& _reaper;
	static ::boost::mutex _synchronized;

	static const long long int serialVersionUID = -8088570405686337161L;

	::IceStorm::TopicPrx retrieveTopic(const ::std::string& name);
};
}
}
}

#endif /* GLACIER2SESSIONMANAGERI_H_ */
