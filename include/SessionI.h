#ifndef SESSIONI_H_
#define SESSIONI_H_

#include <boost/thread/mutex.hpp>
#include <Glacier2/Session.h>
#include <Ice/Logger.h>
#include <Ice/ObjectAdapter.h>
#include <IceStorm/IceStorm.h>
#include <string>

#include "CardroidManagerI.h"
#include "SessionManager.h"
#include "Session.h"

namespace Cardroid {
namespace Network {
namespace Server {

class SessionManager;

/**
 * \class SessionI
 * The session object.
 * This is used to retrieve a CarDroid session on behalf of the client.
 * If the session is not refreshed on a periodic basis, it will be
 * automatically destroyed.
 */
class SessionI: public cardroid::network::zerocice::Session,
	public ::Glacier2::Session {

public:

	SessionI(const ::Ice::LoggerPtr& logger, bool trace, const ::Ice::ObjectAdapterPtr& adapter,
			const ::IceStorm::TopicPrx& topic, const ::std::string& userId,
			SessionManager& sessionManager, bool glacier2);
	virtual cardroid::zerocice::CardroidManagerPrx getCardroidManager(
			const ::Ice::Current& = ::Ice::Current());
	virtual IceStorm::TopicPrx getTopic(const ::Ice::Current& = ::Ice::Current());
	virtual void refresh(const ::Ice::Current& = ::Ice::Current());
	long getSessionTimeout();
	virtual void destroy(const ::Ice::Current& current);
	void shutdown();

	virtual bool ice_isA(const ::std::string&, const ::Ice::Current& = ::Ice::Current()) const;
	virtual ::std::vector< ::std::string> ice_ids(const ::Ice::Current& = ::Ice::Current()) const;
	virtual const ::std::string& ice_id(const ::Ice::Current& = ::Ice::Current()) const;
	virtual ::Ice::DispatchStatus __dispatch(::IceInternal::Incoming&, const ::Ice::Current&);
	virtual ::Ice::DispatchStatus ___destroy(::IceInternal::Incoming&, const ::Ice::Current&);
	virtual const ::std::string& ice_staticId();

protected:

	virtual void __writeImpl(::IceInternal::BasicStream*) const;
	virtual void __readImpl(::IceInternal::BasicStream*);

private:

	::Ice::LoggerPtr _logger;
	::IceStorm::TopicPrx _topic;
	/// true if destroy() was called, false otherwise
	bool _destroyed = false;
	/// The last time the session was refreshed
	long _timeout = 0;
	string _userId;
	SessionManager& _sessionManager;
	bool _glacier2;
	CardroidManagerI& _cardroidManagerServant;
	cardroid::zerocice::CardroidManagerPrx _cardroidManagerProxy;
	static boost::mutex _synchronized;
};
}
}
}

#endif /* SESSIONI_H_ */
