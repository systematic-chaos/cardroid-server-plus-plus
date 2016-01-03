#ifndef LOCATORI_H_
#define LOCATORI_H_

#include <Ice/LocalObject.h>
#include <Ice/ObjectAdapter.h>
#include <Ice/Object.h>
#include <Ice/ServantLocator.h>
#include <string>
#include <unordered_map>

#include "DispatchInterceptorI.h"

namespace Cardroid {
namespace Network {
namespace Server {

/**
 * \class LocatorI
 * A servant locator is a local object that you implement and attach to an
 * object adapter. Once an adapter has a servant locator, it consults its
 * active servant map (ASM) to locate a servant for an incoming request as
 * usual. If a servant for the request can be found in the ASM, the request
 * is dispatched to that servant. However, if the ASM does not have an entry
 * for the object identity of the request, the object adapter calls back into
 * the servant locator to ask it to provide a servant for the request.
 */
class LocatorI: public virtual ::Ice::ServantLocator {

protected:

	/// Data structure for keeping the servants located
	::std::unordered_map< ::std::string, DispatchInterceptorPtr>* _servantMap;

	/// The ObjectAdapter this ServantLocator is attached to
	::Ice::ObjectAdapterPtr _adapter;

public:

	/**
	 * The available object categories, for every one of which a
	 * category-specific ServantLocator implementation is provided
	 */
	const static ::std::string CATEGORIES[];

	LocatorI();

	LocatorI(::Ice::ObjectAdapterPtr& adapter);

	virtual ~LocatorI() {
		_servantMap->clear();
		delete _servantMap;
	}

	virtual void finished(const ::Ice::Current& curr, const ::Ice::ObjectPtr& servant,
			const ::Ice::LocalObjectPtr& cookie);

	virtual void deactivate(const ::std::string& category);
};

/**
 * \class PlaceLocatorI
 * Servant Locator for the "place" category
 */
class PlaceLocatorI: public LocatorI {

public:

	PlaceLocatorI();

	virtual ~PlaceLocatorI();

	virtual ::Ice::ObjectPtr locate(const ::Ice::Current& c, ::Ice::LocalObjectPtr& cookie);
};

/**
 * \class CarLocatorI
 * Servant Locator for the "car" category
 */
class CarLocatorI: public LocatorI {

public:

	CarLocatorI();

	virtual ~CarLocatorI();

	virtual ::Ice::ObjectPtr locate(const ::Ice::Current& c, ::Ice::LocalObjectPtr& cookie);
};

/**
 * \class UserLocatorI
 * Servant Locator for the "user" category
 */
class UserLocatorI: public LocatorI {

public:

	UserLocatorI(::Ice::ObjectAdapterPtr& adapter);

	virtual ~UserLocatorI();

	virtual ::Ice::ObjectPtr locate(const ::Ice::Current& c, ::Ice::LocalObjectPtr& cookie);
};

/**
 * \class TripLocatorI
 * Servant Locator for the "trip" category
 */
class TripLocatorI: public LocatorI {

public:

	TripLocatorI();

	virtual ~TripLocatorI();

	virtual ::Ice::ObjectPtr locate(const ::Ice::Current& c, ::Ice::LocalObjectPtr& cookie);
};

/**
 * \class TripOfferLocatorI
 * Servant Locator for the "trip_offer" category
 */
class TripOfferLocatorI: public LocatorI {

public:

	TripOfferLocatorI(::Ice::ObjectAdapterPtr& adapter);

	virtual ~TripOfferLocatorI();

	virtual ::Ice::ObjectPtr locate(const ::Ice::Current& c, ::Ice::LocalObjectPtr& cookie);
};

/**
 * \class TripRequestLocatorI
 * Servant Locator for the "trip_request" category
 */
class TripRequestLocatorI: public LocatorI {

public:

	TripRequestLocatorI(::Ice::ObjectAdapterPtr& adapter);

	virtual ~TripRequestLocatorI();

	virtual ::Ice::ObjectPtr locate(const ::Ice::Current& c, ::Ice::LocalObjectPtr& cookie);
};

/**
 * \class UserActivityLocatorI
 * Servant Locator for the "user_activity" category
 */
class UserActivityLocatorI: public LocatorI {

public:

	UserActivityLocatorI(::Ice::ObjectAdapterPtr& adapter);

	virtual ~UserActivityLocatorI();

	virtual ::Ice::ObjectPtr locate(const ::Ice::Current& c, ::Ice::LocalObjectPtr& cookie);
};

/**
 * \class MessageLocatorI
 * Servant Locator for the "message" category
 */
class MessageLocatorI: public LocatorI {

public:

	MessageLocatorI(::Ice::ObjectAdapterPtr& adapter);

	virtual ~MessageLocatorI();

	virtual ::Ice::ObjectPtr locate(const ::Ice::Current& c, ::Ice::LocalObjectPtr& cookie);
};
}
}
}

#endif /* LOCATORI_H_ */
