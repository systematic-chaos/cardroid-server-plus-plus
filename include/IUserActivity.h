#ifndef IUSERACTIVITY_H_
#define IUSERACTIVITY_H_

#include <occiData.h>

#include "IUser.h"
#include "ITripOffer.h"

namespace Cardroid {
namespace Data {

/**
 * \interface IUserActivity
 * Public operations interface for the implementation of an UserActivity object
 */
class IUserActivity {

public:

	enum ActivityType {
		TRIPJOIN, TRIPREQUESTANSWERED, TRIPACCEPT, TRIPREFUSE
	};

	/**
	 * Create an instance of the class implementing this interface,
	 * from the received parameter, which also implements it.
	 * This method acts as an Abstract Factory, for the sake of the
	 * implementation of the Abstract Factory pattern in the search of
	 * interoperability among the different implementations of this interface
	 * which could exist in the different subsystems of the platform
	 * @param userActivityObject An object instance implementing this interface
	 * @return An instance of the class implementing this interface, containing
	 * 			exactly the same data of the received bitmapObject parameter,
	 * 			from the viewpoint of the operations defined in this interface
	 */
	virtual shared_ptr<IUserActivity> newInstance(
			const shared_ptr<IUserActivity> userActivityObject) = 0;

	/**
	 * @param activityUser The new user for this IUserActivity
	 */
	virtual void setUser(const IUser& activityUser) = 0;

	/**
	 * @return The user for this IUserActivity
	 */
	virtual const IUser& getUser() = 0;

	/**
	 * @param activityTrip The new trip for this IUserActivity
	 */
	virtual void setTrip(const ITripOffer& activityTrip) = 0;

	/**
	 * @return The trip for this IUserActivity
	 */
	virtual const ITripOffer& getTrip() = 0;

	/**
	 * @param activityType The new type for this IUserActivity
	 */
	virtual void setType(ActivityType activityType) = 0;

	/**
	 * @return The type for this IUserActivity
	 */
	virtual ActivityType getType() = 0;

	/**
	 * @return The timestamp for this IUserActivity
	 */
	virtual oracle::occi::Timestamp getTimeStamp() = 0;

	/**
	 * @param timeStamp The new timestamp for this IUserActivity
	 */
	virtual void setTimeStamp(oracle::occi::Timestamp timeStamp) = 0;

	virtual ~IUserActivity() {
	}
};

namespace Ice {

using namespace IceProxy;
using namespace Cardroid::Data;

/**
 * \class UserActivity
 * Domain class implementing an User Activity for its transmission between systems
 * communicating across an Ice network infrastructure
 */
class UserActivity: public ::cardroid::data::zerocice::UserActivityTyp,
		public IUserActivity, public ::Ice::ObjectFactory {

public:

	UserActivity(::Ice::ObjectAdapterPtr adapter) {
		_adapter = adapter;
	}

	/**
	 * Default constructor
	 */
	UserActivity(::cardroid::data::zerocice::UserTypPrx au,
			::cardroid::data::zerocice::TripOfferTypPrx at,
			::cardroid::data::zerocice::ActivityType uat, long ts) {
		activityUser = au;
		activityTrip = at;
		userActivityType = uat;
		timeStamp = ts;
	}

	UserActivity(const UserActivityTyp& userActivity) {
		activityUser = userActivity.activityUser;
		activityTrip = userActivity.activityTrip;
		userActivityType = userActivity.userActivityType;
		timeStamp = userActivity.timeStamp;
	}

	UserActivity(const UserActivity& ua) {
		UserActivity& userActivity = const_cast<UserActivity&>(ua);
		activityUser = userActivity.activityUser;
		activityTrip = userActivity.activityTrip;
		userActivityType = userActivity.userActivityType;
		timeStamp = userActivity.timeStamp;
	}

	UserActivity(UserActivity&& userActivity) {
		swap(activityUser, userActivity.activityUser);
		swap(activityTrip, userActivity.activityTrip);
		swap(userActivityType, userActivity.userActivityType);
		swap(timeStamp, userActivity.timeStamp);
	}

	~UserActivity() {
		delete &activityUser;
		delete &activityTrip;
	}

	UserActivity& operator=(UserActivity userActivity) {
		activityUser = userActivity.activityUser;
		activityTrip = userActivity.activityTrip;
		userActivityType = userActivity.userActivityType;
		timeStamp = userActivity.timeStamp;
		return *this;
	}

	UserActivity& operator=(UserActivity& userActivity) {
		swap(activityUser, userActivity.activityUser);
		swap(activityTrip, userActivity.activityTrip);
		swap(userActivityType, userActivity.userActivityType);
		swap(timeStamp, userActivity.timeStamp);
		return *this;
	}

	/**
	 *  @return An Ice Identity for this datatype category and the data provided
	 */
	static ::Ice::Identity createIdentity(const string& userEmail, int tripId,
			::cardroid::data::zerocice::ActivityType activityType,
			long long timeStamp) {
		string activityTypeName;
		switch (activityType) {
		case TRIPACCEPT:
			activityTypeName = "TRIPACCEPT";
			break;
		case TRIPJOIN:
			activityTypeName = "TRIPJOIN";
			break;
		case TRIPREFUSE:
			activityTypeName = "TRIPREFUSE";
			break;
		case TRIPREQUESTANSWERED:
			activityTypeName = "TRIPREQUESTANSWERED";
			break;
		}
		::Ice::Identity id;
		id.category = "user_activity";
		id.name = userEmail + "@" + to_string(tripId) + "[" + activityTypeName + " "
				+ to_string(timeStamp) + "]";
		return id;
	}

	/**
	 * @return A proxy to an Ice Object incarnating the provided data, whose
	 * 			servant is added to adapter 's active servant map
	 */
	static ::cardroid::data::zerocice::UserActivityTypPrx createProxy(
			const ::cardroid::data::zerocice::UserActivityTyp& userActivityObject,
			::Ice::ObjectAdapterPtr adapter) {
		::cardroid::data::zerocice::UserActivityTypPrx userActivityPrx =
				::cardroid::data::zerocice::UserActivityTypPrx::uncheckedCast(
						adapter->createProxy(
								createIdentity(
										userActivityObject.getActivityUser()->getEmail(),
										userActivityObject.getActivityTrip()->getTripId(),
										userActivityObject.getUserActivityType(),
										userActivityObject.getTimeStampInMillis())));
		fillProxyData(userActivityObject, userActivityPrx, adapter);
		return userActivityPrx;
	}

	/**
	 * @return A proxy to an Ice Object incarnating the provided data, whose
	 * 			servant is added to adapter 's active servant map
	 */
	static ::cardroid::data::zerocice::UserActivityTypPrx createProxy(
			const IUserActivity& uao, ::Ice::ObjectAdapterPtr adapter) {
		IUserActivity& userActivityObject = const_cast<IUserActivity&>(uao);
		::cardroid::data::zerocice::UserActivityTypPrx userActivityPrx =
				::cardroid::data::zerocice::UserActivityTypPrx::uncheckedCast(
						adapter->createProxy(
								createIdentity(
										const_cast<IUser&>(userActivityObject.getUser()).getEmail(),
										const_cast<ITripOffer&>(userActivityObject.getTrip()).getTripId(),
										static_cast< ::cardroid::data::zerocice::ActivityType>(userActivityObject.getType()),
										::Cardroid::occi::timestampToMillis(
												userActivityObject.getTimeStamp()))));
		fillProxyData(userActivityObject, userActivityPrx, adapter);
		return userActivityPrx;
	}

	/**
	 * Fill the servant referenced by the given proxy with the data contained
	 * in the given object
	 */
	static void fillProxyData(const UserActivityTyp& object,
			::cardroid::data::zerocice::UserActivityTypPrx proxy,
			::Ice::ObjectAdapterPtr adapter) {
		proxy->setActivityUser(object.getActivityUser());
		proxy->setActivityTrip(object.getActivityTrip());
		proxy->setUserActivityType(object.getUserActivityType());
		proxy->setTimeStampInMillis(object.getTimeStampInMillis());
	}

	/**
	 * Fill the servant referenced by the given proxy with the data contained
	 * in the given object
	 */
	static void fillProxyData(const IUserActivity& o,
			::cardroid::data::zerocice::UserActivityTypPrx proxy,
			::Ice::ObjectAdapterPtr adapter) {
		IUserActivity& object = const_cast<IUserActivity&>(o);
		proxy->setActivityUser(User::createProxy(object.getUser(), adapter));
		proxy->setActivityTrip(
				TripOffer::createProxy(
						const_cast<ITripOffer&>(object.getTrip()), adapter));
		proxy->setUserActivityType(
				static_cast< ::cardroid::data::zerocice::ActivityType>(object.getType()));
		proxy->setTimeStampInMillis(
				::Cardroid::occi::timestampToMillis(object.getTimeStamp()));
	}

	/**
	 * @param proxy A proxy to a remote object implementing a UserActivity
	 * @return A local UserActivity object containing the data of the remote
	 * 			object referenced by proxy
	 */
	static shared_ptr<UserActivity> extractObject(
			::cardroid::data::zerocice::UserActivityTypPrx proxy) {
		return make_shared<UserActivity>(
				proxy->getActivityUser(),
				proxy->getActivityTrip(),
				proxy->getUserActivityType(),
				proxy->getTimeStampInMillis());
	}

	/* IUserActivity interface */

	shared_ptr<IUserActivity> newInstance(
			const shared_ptr<IUserActivity> userActivityObject) {
		if (!userActivityObject)
			return NULL;
		if (dynamic_cast<UserActivity*>(userActivityObject.get()))
			return userActivityObject;

		::cardroid::data::zerocice::UserTypPrx activityUsr = User::createProxy(
				userActivityObject->getUser(), _adapter);
		::cardroid::data::zerocice::TripOfferTypPrx activityTrip =
				TripOffer::createProxy(
						const_cast<ITripOffer&>(userActivityObject->getTrip()),
						_adapter);
		::cardroid::data::zerocice::ActivityType userActivityType =
				static_cast< ::cardroid::data::zerocice::ActivityType>(userActivityObject->getType());
		long long timeStamp = ::Cardroid::occi::timestampToMillis(
				userActivityObject->getTimeStamp());

		return static_pointer_cast<IUserActivity>(make_shared<UserActivity>(
				activityUsr, activityTrip, userActivityType,
						timeStamp));
	}

	void setUser(const IUser& activityUser) {
		setActivityUser(User::createProxy(activityUser, _adapter));
	}

	IUser& getUser() {
		return *(User::extractObject(getActivityUser()).get());
	}

	void setTrip(const ITripOffer& activityTrip) {
		setActivityTrip(
				TripOffer::createProxy(const_cast<ITripOffer&>(activityTrip),
						_adapter));
	}

	ITripOffer& getTrip() {
		return *(TripOffer::extractObject(getActivityTrip()).get());
	}

	void setType(IUserActivity::ActivityType activityType) {
		setUserActivityType(
				static_cast< ::cardroid::data::zerocice::ActivityType>(activityType));
	}

	IUserActivity::ActivityType getType() {
		return static_cast<IUserActivity::ActivityType>(getUserActivityType());
	}

	void setTimeStamp(::oracle::occi::Timestamp timeStamp) {
		setTimeStampInMillis(::Cardroid::occi::timestampToMillis(timeStamp));
	}

	::oracle::occi::Timestamp getTimeStamp() {
		return ::Cardroid::occi::millisToTimestamp(getTimeStampInMillis());
	}

	/* Overriding superclass */

	static const string& ice_staticId() {
		return super::ice_staticId();
	}

	::cardroid::data::zerocice::UserTypPrx getActivityUser(
			const ::Ice::Current& = ::Ice::Current()) const {
		return activityUser;
	}

	void setActivityUser(const ::cardroid::data::zerocice::UserTypPrx& au,
			const ::Ice::Current& = ::Ice::Current()) {
		activityUser = au;
	}

	::cardroid::data::zerocice::TripOfferTypPrx getActivityTrip(
			const ::Ice::Current& = ::Ice::Current()) const {
		return activityTrip;
	}

	void setActivityTrip(const ::cardroid::data::zerocice::TripOfferTypPrx& at,
			const ::Ice::Current& = ::Ice::Current()) {
		activityTrip = at;
	}

	::cardroid::data::zerocice::ActivityType getUserActivityType(
			const ::Ice::Current& = ::Ice::Current()) const {
		return userActivityType;
	}

	void setUserActivityType(::cardroid::data::zerocice::ActivityType uat,
			const ::Ice::Current& = ::Ice::Current()) {
		userActivityType = uat;
	}

	long long getTimeStampInMillis(const ::Ice::Current& =
			::Ice::Current()) const {
		return timeStamp;
	}

	void setTimeStampInMillis(long long tsm, const ::Ice::Current& =
			::Ice::Current()) {
		timeStamp = tsm;
	}

	string toString(const ::Ice::Current& = ::Ice::Current()) const {
		string builder;
		switch (userActivityType) {
		case TRIPACCEPT:
			builder = " ha aceptado tun inclusión en el viaje ";
			break;
		case TRIPJOIN:
			builder = " ha solicitado unirse a tu viaje ";
			break;
		case TRIPREFUSE:
			builder = " ha rechazado tu inclusión en el viaje ";
			break;
		case TRIPREQUESTANSWERED:
			builder = " ha respondido a tu solicitud del viaje ";
			break;
		}
		return activityUser->toString() + builder + activityTrip->toString();
	}

	/* ObjectFactory interface */

	::Ice::ObjectPtr create(const string& type) {
		if (!type.compare(ice_staticId()))
			return new UserActivity(_adapter);
		return NULL;
	}

	void destroy() {
	}

private:

	::Ice::ObjectAdapterPtr _adapter;
	static const long long int serialVersionUID = 4842929861773747077L;
	typedef ::cardroid::data::zerocice::UserActivityTyp super;
};
}

namespace Oracle {

using namespace Cardroid::Data;

/**
 * \class UserActivity
 * Domain class implementing an UserActivity for its storage and retrieval
 * from an Oracle database
 */
class UserActivity: public ::UserActivityTyp, public IUserActivity {

public:

	UserActivity() {
	}

	UserActivity(void* ctxOCCI_) :
			super(ctxOCCI_) {
	}

	/**
	 * Default constructor
	 */
	UserActivity(const User& activityUser, const TripOffer& activityTrip,
			const string& activityType) {
		setUser(activityUser);
		setTrip(activityTrip);
		if (!getActivity_type().compare("TRIPACCEPT")) {
			setType(IUserActivity::ActivityType::TRIPACCEPT);
			return;
		}
		if (!getActivity_type().compare("TRIPJOIN")) {
			setType(IUserActivity::ActivityType::TRIPJOIN);
			return;
		}
		if (!getActivity_type().compare("TRIPREFUSE")) {
			setType(IUserActivity::ActivityType::TRIPREFUSE);
			return;
		}
		if (!getActivity_type().compare("TRIPREQUESTANSWERED")) {
			setType(IUserActivity::ActivityType::TRIPREQUESTANSWERED);
			return;
		}
	}

	UserActivity(const UserActivityTyp& userActivity) {
		setActivity_user(userActivity.getActivity_user());
		setActivity_trip(userActivity.getActivity_trip());
		setActivity_type(userActivity.getActivity_type());
		setTime_stamp(userActivity.getTime_stamp());
	}

	UserActivity(const UserActivity& ua) {
		UserActivity& userActivity = const_cast<UserActivity&>(ua);
		setUser(userActivity.getUser());
		setTrip(userActivity.getTrip());
		setType(userActivity.getType());
		setTimeStamp(userActivity.getTimeStamp());
	}

	UserActivity(UserActivity&& userActivity) {
		setUser(userActivity.getUser());
		setTrip(userActivity.getTrip());
		setType(userActivity.getType());
		setTimeStamp(userActivity.getTimeStamp());
	}

	~UserActivity() {
	}

	UserActivity& operator=(UserActivity userActivity) {
		setUser(userActivity.getUser());
		setTrip(userActivity.getTrip());
		setType(userActivity.getType());
		setTimeStamp(userActivity.getTimeStamp());
		return *this;
	}

	UserActivity& operator=(UserActivity& userActivity) {
		setUser(userActivity.getUser());
		setTrip(userActivity.getTrip());
		setType(userActivity.getType());
		setTimeStamp(userActivity.getTimeStamp());
		return *this;
	}

	/* IUserActivity interface */

	shared_ptr<IUserActivity> newInstance(
			const shared_ptr<IUserActivity> userActivityObject) {
		if (!userActivityObject)
			return NULL;
		if (dynamic_cast<UserActivity*>(userActivityObject.get()))
			return userActivityObject;

		User user = *(static_pointer_cast<User>(User().newInstance(shared_ptr<IUser>(
				&const_cast<IUser&>(userActivityObject->getUser())))).get());
		TripOffer trip = *(static_pointer_cast<TripOffer>(TripOffer().newInstance(
				shared_ptr<ITripOffer>(&const_cast<ITripOffer&>(
						userActivityObject->getTrip())))).get());
		string typeName;
		switch (userActivityObject->getType()) {
		case TRIPACCEPT:
			typeName = "TRIPACCEPT";
			break;
		case TRIPJOIN:
			typeName = "TRIPJOIN";
			break;
		case TRIPREFUSE:
			typeName = "TRIPREFUSE";
			break;
		case TRIPREQUESTANSWERED:
			typeName = "TRIPREQUESTANSWERED";
			break;
		}
		return static_pointer_cast<IUserActivity>(make_shared<UserActivity>(
				user, trip, typeName));
	}

	void setUser(const IUser& activityUser) {
		setActivity_user(static_pointer_cast<User>(
				User().newInstance(shared_ptr<IUser>(
						&const_cast<IUser&>(activityUser)))).get());
	}

	const User& getUser() {
		return *getActivity_user();
	}

	void setTrip(const ITripOffer& activityTrip) {
		setActivity_trip(static_pointer_cast<TripOffer>(
				TripOffer().newInstance(shared_ptr<ITripOffer>(
						&const_cast<ITripOffer&>(activityTrip)))).get());
	}

	const ITripOffer& getTrip() {
		return *getActivity_trip();
	}

	void setType(IUserActivity::ActivityType activityType) {
		string name = "";
		switch (activityType) {
		case TRIPACCEPT:
			name = "TRIPACCEPT";
			break;
		case TRIPJOIN:
			name = "TRIPJOIN";
			break;
		case TRIPREFUSE:
			name = "TRIPREFUSE";
			break;
		case TRIPREQUESTANSWERED:
			name = "TRIPREQUESTANSWERED";
			break;
		}
		setActivity_type(name);
	}

	IUserActivity::ActivityType getType() {
		IUserActivity::ActivityType type;
		if (!getActivity_type().compare("TRIPACCEPT"))
			return type = IUserActivity::ActivityType::TRIPACCEPT;
		if (!getActivity_type().compare("TRIPJOIN"))
			return type = IUserActivity::ActivityType::TRIPJOIN;
		if (!getActivity_type().compare("TRIPREFUSE"))
			return type = IUserActivity::ActivityType::TRIPREFUSE;
		else
			return type = IUserActivity::ActivityType::TRIPREQUESTANSWERED;
	}

	::oracle::occi::Timestamp getTimeStamp() {
		return getTime_stamp();
	}

	void setTimeStamp(::oracle::occi::Timestamp timeStamp) {
		setTime_stamp(timeStamp);
	}

	/* Overriding superclass */

	User* getActivity_user() const {
		User* activityUser = NULL;
		try {
			activityUser = super::getActivity_user();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return activityUser;
	}

	void setActivity_user(User* value) {
		try {
			super::setActivity_user(value);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	TripOffer* getActivity_trip() const {
		TripOffer* activityTrip = NULL;
		try {
			activityTrip = super::getActivity_trip();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return activityTrip;
	}

	void setActivity_trip(TripOffer* value) {
		try {
			super::setActivity_trip(value);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	string getActivity_type() const {
		string activityType = "";
		try {
			activityType = super::getActivity_type();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return activityType;
	}

	void setActivity_type(const string& value) {
		try {
			super::setActivity_type(value);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	::oracle::occi::Timestamp getTime_stamp() const {
		::oracle::occi::Timestamp timeStamp = ::oracle::occi::Timestamp();
		timeStamp.setNull();
		try {
			timeStamp = super::getTime_stamp();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return timeStamp;
	}

	void setTime_stamp(const ::oracle::occi::Timestamp& value) {
		try {
			super::setTime_stamp(value);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	static string getSqlTypeName() {
		return string("ANDROID.USER_ACTIVITY_TYP");
	}

private:

	typedef ::UserActivityTyp super;
};
}
}
}

#endif /* IUSERACTIVITY_H_ */
