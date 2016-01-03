#ifndef IMESSAGE_H_
#define IMESSAGE_H_

#include <occiData.h>
#include <boost/date_time/posix_time/conversion.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <string>

#include "IUser.h"

using namespace std;

namespace Cardroid {
namespace Data {

/**
 * \interface IMessage
 * Public operations interface for the implementation of a Message object
 */
class IMessage {

public:

	/**
	 * Create an instance of the class implementing this interface,
	 * from the received parameter, which also implements it.
	 * This method acts as an Abstract Factory, for the sake of the
	 * implementation of the Abstract Factory pattern in the search of
	 * interoperability among the different implementations of this interface
	 * which could exist in the different subsystems of the platform
	 * @param messageObject An object instance implementing this interface
	 * @return An instance of the class implementing this interface, containing
	 * 			exactly the same data of the received bitmapObject parameter,
	 * 			from the viewpoint of the operations defined in this interface
	 */
	virtual shared_ptr<IMessage> newInstance(
			const shared_ptr<IMessage> messageObject) = 0;

	/**
	 * @param user The sender of this IMessage
	 */
	virtual void setFromUser(const IUser& user) = 0;

	/**
	 * @return The sender of this IMessage
	 */
	virtual const IUser& getFromUser() = 0;

	/**
	 * @param user The receipt of this IMessage
	 */
	virtual void setToUser(const IUser& user) = 0;

	/**
	 * @return The receipt of this IMessage
	 */
	virtual const IUser& getToUser() = 0;

	/**
	 * @return The text of a this IMessage
	 */
	virtual const string& getMessageText() = 0;

	/**
	 * @param text The new text of this IMessage
	 */
	virtual void setMessageText(const string& text) = 0;

	/**
	 * @return The timestamp for this IMessage
	 */
	virtual oracle::occi::Timestamp getTimeStamp() = 0;

	/**
	 * @param timeStamp The new timestamp for this IMessage
	 */
	virtual void setTimeStamp(oracle::occi::Timestamp timeStamp) = 0;

	virtual ~IMessage() {
	}
};

namespace Ice {

using namespace IceProxy;
using namespace Cardroid::Data;

/**
 * \class Message
 * Domain class implementing a Message for its transmission between systems
 * communicating across an Ice network infrastructure
 */
class Message: public ::cardroid::data::zerocice::MessageTyp,
		public IMessage, public ::Ice::ObjectFactory {

public:

	Message(::Ice::ObjectAdapterPtr adapter) {
		_adapter = adapter;
	}

	/**
	 * Default constructor
	 */
	Message(::cardroid::data::zerocice::UserTypPrx u1,
			::cardroid::data::zerocice::UserTypPrx u2, const string& m,
			long long ts) {
		user1 = u1;
		user2 = u2;
		msg = m;
		timeStamp = ts;
	}

	Message(const ::cardroid::data::zerocice::MessageTyp& message) {
		user1 = message.user1;
		user2 = message.user2;
		msg = message.msg;
		timeStamp = message.timeStamp;
	}

	Message(const Message& message) {
		user1 = message.user1;
		user2 = message.user2;
		msg = message.msg;
		timeStamp = message.timeStamp;
	}

	Message(Message&& message) {
		swap(user1, message.user1);
		swap(user2, message.user2);
		swap(msg, message.msg);
		swap(timeStamp, message.timeStamp);
	}

	~Message() {
		delete &user1;
		delete &user2;
	}

	Message& operator=(Message message) {
		user1 = message.user1;
		user2 = message.user2;
		msg = message.msg;
		timeStamp = message.timeStamp;
		return *this;
	}

	Message& operator=(Message& message) {
		swap(user1, message.user1);
		swap(user2, message.user2);
		swap(msg, message.msg);
		swap(timeStamp, message.timeStamp);
		return *this;
	}

	/**
	 * @return An Ice Identity for this datatype category and the data provided
	 */
	static ::Ice::Identity createIdentity(const string& emailUser1,
			const string& emailUser2, long long timeStamp) {
		::Ice::Identity id;
		id.category = "message";
		id.name = emailUser1 + " - " + emailUser2 + " [" + to_string(timeStamp) + "]";
		return id;
	}

	/**
	 * @return A proxy to an Ice Object incarnating the provided data, whose
	 * 			servant is added to adapter 's active servant map
	 */
	static ::cardroid::data::zerocice::MessageTypPrx createProxy(
			const ::cardroid::data::zerocice::MessageTyp& messageObject,
			::Ice::ObjectAdapterPtr adapter) {
		::cardroid::data::zerocice::MessageTypPrx messagePrx =
				::cardroid::data::zerocice::MessageTypPrx::uncheckedCast(
						adapter->createProxy(
								createIdentity(
										messageObject.getUser1()->getEmail(),
										messageObject.getUser2()->getEmail(),
										messageObject.getTimeStampInMillis())));
		fillProxyData(messageObject, messagePrx, adapter);
		return messagePrx;
	}

	/**
	 * @return A proxy to an Ice Object incarnating the provided data, whose
	 * 			servant is added to adapter 's active servant map
	 */
	static ::cardroid::data::zerocice::MessageTypPrx createProxy(
			const IMessage& mo, ::Ice::ObjectAdapterPtr adapter) {
		IMessage& messageObject = const_cast<IMessage&>(mo);
		::cardroid::data::zerocice::MessageTypPrx messagePrx =
				::cardroid::data::zerocice::MessageTypPrx::uncheckedCast(
						adapter->createProxy(
								createIdentity(
										const_cast<IUser&>(messageObject.getFromUser()).getEmail(),
										const_cast<IUser&>(messageObject.getToUser()).getEmail(),
										::Cardroid::occi::timestampToMillis(
												messageObject.getTimeStamp()))));
		fillProxyData(messageObject, messagePrx, adapter);
		return messagePrx;
	}

	/**
	 * Fill the servant referenced by the given proxy with the data contained
	 * in the given object
	 */
	static void fillProxyData(
			const ::cardroid::data::zerocice::MessageTyp& object,
			::cardroid::data::zerocice::MessageTypPrx proxy,
			::Ice::ObjectAdapterPtr adapter) {
		proxy->setUser1(object.getUser1());
		proxy->setUser2(object.getUser2());
		proxy->setMessageText(object.getMessageText());
		proxy->setTimeStampInMillis(object.getTimeStampInMillis());
	}

	/**
	 * Fill the servant referenced by the given proxy with the data contained
	 * in the given object
	 */
	static void fillProxyData(const IMessage& o,
			::cardroid::data::zerocice::MessageTypPrx proxy,
			::Ice::ObjectAdapterPtr adapter) {
		IMessage& object = const_cast<IMessage&>(o);
		proxy->setUser1(User::createProxy(object.getFromUser(), adapter));
		proxy->setUser2(User::createProxy(object.getToUser(), adapter));
		proxy->setMessageText(object.getMessageText());
		proxy->setTimeStampInMillis(
				::Cardroid::occi::timestampToMillis(object.getTimeStamp()));
	}

	/**
	 * @param proxy A proxy to a remote object implementing a Message
	 * @return A local Message object containing the data of the remote object
	 * 			referenced by proxy
	 */
	static shared_ptr<Message> extractObject(
			::cardroid::data::zerocice::MessageTypPrx proxy) {
		return make_shared<Message>(proxy->getUser1(), proxy->getUser2(),
						proxy->getMessageText(), proxy->getTimeStampInMillis());
	}

	/* IMessage interface */

	shared_ptr<IMessage> newInstance(const shared_ptr<IMessage> messageObject) {
		if (!messageObject)
			return NULL;
		if (dynamic_cast<Message*>(messageObject.get()))
			return messageObject;

		::cardroid::data::zerocice::UserTypPrx user1 = User::createProxy(
				messageObject->getFromUser(), _adapter);
		::cardroid::data::zerocice::UserTypPrx user2 = User::createProxy(
				messageObject->getToUser(), _adapter);
		const string& msg = messageObject->getMessageText();
		long long timeStamp = ::Cardroid::occi::timestampToMillis(
				messageObject->getTimeStamp());
		return static_pointer_cast<IMessage>(make_shared<Message>(user1, user2, msg, timeStamp));
	}

	void setFromUser(const IUser& user) {
		setUser1(User::createProxy(user, _adapter));
	}

	const IUser& getFromUser() {
		return const_cast<const IUser&>(static_cast<IUser&>(*User::extractObject(user1).get()));
	}

	void setToUser(const IUser& user) {
		setUser2(User::createProxy(user, _adapter));
	}

	const IUser& getToUser() {
		return const_cast<const IUser&>(static_cast<IUser&>(*User::extractObject(user2).get()));
	}

	const string& getMessageText() {
		return msg;
	}

	void setMessageText(const string& text) {
		msg = text;
	}

	::oracle::occi::Timestamp getTimeStamp() {
		return ::Cardroid::occi::millisToTimestamp(timeStamp);
	}

	void setTimeStamp(::oracle::occi::Timestamp ts) {
		timeStamp = ::Cardroid::occi::timestampToMillis(ts);
	}

	/* Overriding superclass */

	static const string& ice_staticId() {
		return super::ice_staticId();
	}

	::cardroid::data::zerocice::UserTypPrx getUser1(const ::Ice::Current& =
			::Ice::Current()) const {
		return user1;
	}

	void setUser1(const ::cardroid::data::zerocice::UserTypPrx& u1,
			const ::Ice::Current& = ::Ice::Current()) {
		user1 = u1;
	}

	::cardroid::data::zerocice::UserTypPrx getUser2(const ::Ice::Current& =
			::Ice::Current()) const {
		return user2;
	}

	void setUser2(const ::cardroid::data::zerocice::UserTypPrx& u2,
			const ::Ice::Current& = ::Ice::Current()) {
		user2 = u2;
	}

	string getMessageText(const ::Ice::Current& =
			::Ice::Current()) const {
		return msg;
	}

	void setMessageText(const string& m, const ::Ice::Current& =
			::Ice::Current()) {
		msg = m;
	}

	long long getTimeStampInMillis(const ::Ice::Current& =
			::Ice::Current()) const {
		return timeStamp;
	}

	void setTimeStampInMillis(long long timeStampMillis, const ::Ice::Current& =
			::Ice::Current()) {
		timeStamp = timeStampMillis;
	}

	string toString(const ::Ice::Current& = ::Ice::Current()) const {
		::boost::posix_time::ptime p = ::boost::posix_time::from_time_t(
				timeStamp / 1000)
				+ ::boost::posix_time::milliseconds(timeStamp % 1000);
		struct tm tm = ::boost::posix_time::to_tm(p);
		char buf[32];
		strftime(buf, 31, "%e/%m/%Y %H:%M:%S", &tm);
		return user1->toString() + " - " + user2->toString() + ": " + msg
				+ " [" + string(buf) + "]";
	}

	/* ObjectFactory interface */

	::Ice::ObjectPtr create(const string& type) {
		if (!type.compare(ice_staticId()))
			return new Message(_adapter);

		return NULL;
	}

	void destroy() {
	}

private:

	::Ice::ObjectAdapterPtr _adapter;
	static const long long int serialVersionUID = 5764230398370402663L;
	typedef ::cardroid::data::zerocice::MessageTyp super;
};
}

namespace Oracle {

using namespace Cardroid::Data;
/**
 * \class Message
 * Domain class implementing a Message for its storage and retrieval from an
 * Oracle database
 */
class Message: public ::MessageTyp, public IMessage {

public:

	Message() {
	}

	Message(void* ctxOCCI_) :
			super(ctxOCCI_) {
	}

	/**
	 * Default constructor
	 */
	Message(const User& user1, const User& user2, const string& message,
			const ::oracle::occi::Timestamp& timestamp) {
		setFromUser(user1);
		setToUser(user2);
		setMessage(message);
		setTimeStamp(timestamp);
	}

	Message(const ::MessageTyp& message) {
		setUser1(message.getUser1());
		setUser2(message.getUser2());
		setMessage(message.getMessage());
		setTimeStamp(message.getTime_stamp());
	}

	Message(const Message& m) {
		Message& message = const_cast<Message&>(m);
		setFromUser(message.getFromUser());
		setToUser(message.getToUser());
		setMessage(message.getMessage());
		setTimeStamp(message.getTimeStamp());
	}

	Message(const Message&& m) {
		Message& message = const_cast<Message&>(m);
		setFromUser(message.getFromUser());
		setToUser(message.getToUser());
		setMessage(message.getMessage());
		setTimeStamp(message.getTimeStamp());
	}

	~Message() {
	}

	/* IMessage interface */

	shared_ptr<IMessage> newInstance(const shared_ptr<IMessage> messageObject) {
		if (!messageObject)
			return NULL;
		if (dynamic_cast<Message*>(messageObject.get()))
			return messageObject;

		User* user1 = static_pointer_cast<User>(User().newInstance(
				shared_ptr<IUser>(&const_cast<IUser&>(messageObject->getFromUser())))).get();
		User* user2 = static_pointer_cast<User>(User().newInstance(
				shared_ptr<IUser>(&const_cast<IUser&>(messageObject->getToUser())))).get();
		return static_pointer_cast<IMessage>(
				make_shared<Message>(user1, user2, messageObject->getMessageText(),
						messageObject->getTimeStamp()));
	}

	void setFromUser(const IUser& fromUser) {
		setUser1(static_pointer_cast<User>(User().newInstance(
				shared_ptr<IUser>(&const_cast<IUser&>(fromUser)))).get());
	}

	IUser& getFromUser() {
		return static_cast<IUser&>(*getUser1());
	}

	void setToUser(const IUser& toUser) {
		setUser2(static_pointer_cast<User>(User().newInstance(
				shared_ptr<IUser>(&const_cast<IUser&>(toUser)))).get());
	}

	IUser& getToUser() {
		return static_cast<IUser&>(*getUser2());
	}

	void setMessageText(const string& message) {
		setMessage(message);
	}

	const string& getMessageText() {
		const string& msg = getMessage();
		return msg;
	}

	void setTimeStamp(::oracle::occi::Timestamp timeStamp) {
		setTime_stamp(timeStamp);
	}

	::oracle::occi::Timestamp getTimeStamp() {
		return getTime_stamp();
	}

	/* Overriding superclass */

	User* getUser1() const {
		User* usr = NULL;
		try {
			usr = super::getUser1();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return usr;
	}

	void setUser1(User* value) {
		try {
			super::setUser1(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	User* getUser2() const {
		User* usr = NULL;
		try {
			usr = super::getUser2();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return usr;
	}

	void setUser2(User* value) {
		try {
			super::setUser2(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	string getMessage() const {
		string message = "";
		try {
			message = super::getMessage();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return message;
	}

	void setMessage(const string& value) {
		try {
			super::setMessage(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	::oracle::occi::Timestamp getTime_stamp() const {
		::oracle::occi::Timestamp timeStamp = ::oracle::occi::Timestamp();
		timeStamp.setNull();
		try {
			timeStamp = super::getTime_stamp();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return timeStamp;
	}

	void setTime_stamp(const ::oracle::occi::Timestamp& value) {
		try {
			super::setTime_stamp(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	static string getSqlTypeName() {
		return string("ANDROID.MESSAGE_TYP");
	}

private:

	typedef ::MessageTyp super;
};

}
}
}

#endif /* IMESSAGE_H_ */
