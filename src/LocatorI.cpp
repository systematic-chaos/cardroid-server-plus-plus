#include "LocatorI.h"

#include <Ice/Current.h>
#include <Ice/Identity.h>
#include <stdexcept>
#include <utility>

#include "ICar.h"
#include "IMessage.h"
#include "IPlace.h"
#include "ITrip.h"
#include "ITripOffer.h"
#include "ITripRequest.h"
#include "IUser.h"
#include "IUserActivity.h"

using namespace std;
using namespace Cardroid::Network::Server;
using namespace ::Ice;

const string LocatorI::CATEGORIES[] = { "place", "car", "user", "trip",
			"trip_offer", "trip_request", "user_activity", "message", "" };

/**
 * Constructor for the allocation of those object categories which do not
 * require an ObjectAdapter
 */
LocatorI::LocatorI() :
		_servantMap(new unordered_map<string, DispatchInterceptorPtr>) {
}

/**
 * Constructor for the allocation of those object categories which do
 * require an ObjectAdapter
 * @param adapter The ObjectAdapter this ServantLocator is attached to.
 * 					It is used in the allocation process of categories
 * 					whose related objects may hold proxies to objects
 * 					that need to be also allocated
 */
LocatorI::LocatorI(ObjectAdapterPtr& adapter) :
		_servantMap(new unordered_map<string, DispatchInterceptorPtr>),
		_adapter(adapter) {
}

/**
 * Called by the object adapter after a request has been made.
 * This operation is only called if locate was called prior to the request
 * and returned a non-null servant. This operation can be used for cleanup
 * purposes after a request.
 * finished can throw any user exception. If it does, that exception is
 * marshaled back to the client. If the slice definition for the
 * corresponding operation includes that user exception, the client
 * receives that user exception; otherwise, the client receives
 * UnknownUserException. If both the operation and finished throw an
 * exception, the exception thrown by finished is marshaled back to the
 * client.
 * @param c 		Information about the current operation call for which
 * 						a servant was located by locate
 * @param servant	The servant that was returned by locate
 * @param cookie	The cookie that was returned by locate
 */
void LocatorI::finished(const Current& curr, const ObjectPtr& servant,
		const LocalObjectPtr& cookie) {
	//_servantMap->erase(curr.id.name);
}

/**
 * Called when the object adapter in which this servant locator is
 * installed is finished
 * @param category Indicates for which category the servant locator is
 * 					being deactivated
 */
void LocatorI::deactivate(const string& category) {
}

/**
 * Constructor for the allocation of those object categories which do not
 * require an ObjectAdapter
 */
PlaceLocatorI::PlaceLocatorI() {
	_servantMap = new unordered_map<string, DispatchInterceptorPtr>();
}

/**
 * Called before a request is dispatched if a servant cannot be found in
 * the object adapter's active servant map. Note that the object adapter
 * does not automatically insert the returned servant into its active
 * servant map. This must be done by the servant locator implementation,
 * if this is desired.
 * locate can throw any user exception. If it does, that exception is
 * marshaled back to the client. If the Slice definition for the
 * corresponding operation includes that user exception, the client
 * receives that user exception; otherwise, the client receives
 * UnknownUserException. If locate throws any exception, the Ice run time
 * does not call finished
 * @param c 		Information about the current operation for which a
 * 						servant is required
 * @param cookie	A "cookie" that will be passed to finished
 * @return			The located servant, or null if no suitable servant
 * 						has been found
 */
ObjectPtr PlaceLocatorI::locate(const Current& c, LocalObjectPtr& cookie) {
	DispatchInterceptorPtr servant;
	try {
		servant = _servantMap->at(c.id.name);
	} catch (out_of_range&) {
		servant = new DispatchInterceptorI(new ::Cardroid::Data::Ice::Place());
		_servantMap->insert(
				pair<string, DispatchInterceptorPtr>(c.id.name, servant));
	}
	return servant;
}

PlaceLocatorI::~PlaceLocatorI() {
	_servantMap->clear();
	delete _servantMap;
}

/**
 * Constructor for the allocation of those object categories which do not
 * require an ObjectAdapter
 */
CarLocatorI::CarLocatorI() {
	_servantMap = new unordered_map<string, DispatchInterceptorPtr>();
}

/**
 * Called before a request is dispatched if a servant cannot be found in
 * the object adapter's active servant map. Note that the object adapter
 * does not automatically insert the returned servant into its active
 * servant map. This must be done by the servant locator implementation,
 * if this is desired.
 * locate can throw any user exception. If it does, that exception is
 * marshaled back to the client. If the Slice definition for the
 * corresponding operation includes that user exception, the client
 * receives that user exception; otherwise, the client receives
 * UnknownUserException. If locate throws any exception, the Ice run time
 * does not call finished
 * @param c 		Information about the current operation for which a
 * 						servant is required
 * @param cookie	A "cookie" that will be passed to finished
 * @return			The located servant, or null if no suitable servant
 * 						has been found
 */
ObjectPtr CarLocatorI::locate(const Current& c, LocalObjectPtr& cookie) {
	DispatchInterceptorPtr servant;
	try {
		servant = _servantMap->at(c.id.name);
	} catch (out_of_range&) {
		servant = new DispatchInterceptorI(new ::Cardroid::Data::Ice::Car());
		_servantMap->insert(
				pair<string, DispatchInterceptorPtr>(c.id.name, servant));
	}
	return servant;
}

CarLocatorI::~CarLocatorI() {
	_servantMap->clear();
	delete _servantMap;
}

/**
 * Constructor for the allocation of those object categories which do
 * require an ObjectAdapter
 * @param adapter The ObjectAdapter this ServantLocator is attached to.
 * 					It is used in the allocation process of categories
 * 					whose related objects may hold proxies to objects
 * 					that need to be also allocated
 */
UserLocatorI::UserLocatorI(ObjectAdapterPtr& adapter) {
	_servantMap = new unordered_map<string, DispatchInterceptorPtr>();
	_adapter = adapter;
}

/**
 * Called before a request is dispatched if a servant cannot be found in
 * the object adapter's active servant map. Note that the object adapter
 * does not automatically insert the returned servant into its active
 * servant map. This must be done by the servant locator implementation,
 * if this is desired.
 * locate can throw any user exception. If it does, that exception is
 * marshaled back to the client. If the Slice definition for the
 * corresponding operation includes that user exception, the client
 * receives that user exception; otherwise, the client receives
 * UnknownUserException. If locate throws any exception, the Ice run time
 * does not call finished
 * @param c 		Information about the current operation for which a
 * 						servant is required
 * @param cookie	A "cookie" that will be passed to finished
 * @return			The located servant, or null if no suitable servant
 * 						has been found
 */
ObjectPtr UserLocatorI::locate(const Current& c, LocalObjectPtr& cookie) {
	DispatchInterceptorPtr servant;
	try {
		servant = _servantMap->at(c.id.name);
	} catch (out_of_range&) {
		servant = new DispatchInterceptorI(
				new ::Cardroid::Data::Ice::User(_adapter));
		_servantMap->insert(
				pair<string, DispatchInterceptorPtr>(c.id.name, servant));
	}
	return servant;
}

UserLocatorI::~UserLocatorI() {
	_servantMap->clear();
	delete _servantMap;
}

/**
 * Constructor for the allocation of those object categories which do not
 * require an ObjectAdapter
 */
TripLocatorI::TripLocatorI() {
	_servantMap = new unordered_map<string, DispatchInterceptorPtr>();
}

/**
 * Called before a request is dispatched if a servant cannot be found in
 * the object adapter's active servant map. Note that the object adapter
 * does not automatically insert the returned servant into its active
 * servant map. This must be done by the servant locator implementation,
 * if this is desired.
 * locate can throw any user exception. If it does, that exception is
 * marshaled back to the client. If the Slice definition for the
 * corresponding operation includes that user exception, the client
 * receives that user exception; otherwise, the client receives
 * UnknownUserException. If locate throws any exception, the Ice run time
 * does not call finished
 * @param c 		Information about the current operation for which a
 * 						servant is required
 * @param cookie	A "cookie" that will be passed to finished
 * @return			The located servant, or null if no suitable servant
 * 						has been found
 */
ObjectPtr TripLocatorI::locate(const Current& c, LocalObjectPtr& cookie) {
	DispatchInterceptorPtr servant;
	try {
		servant = _servantMap->at(c.id.name);
	} catch (out_of_range&) {
		servant = new DispatchInterceptorI(new ::Cardroid::Data::Ice::Trip());
		_servantMap->insert(
				pair<string, DispatchInterceptorPtr>(c.id.name, servant));
	}
	return servant;
}

TripLocatorI::~TripLocatorI() {
	_servantMap->clear();
	delete _servantMap;
}

/**
 * Constructor for the allocation of those object categories which do
 * require an ObjectAdapter
 * @param adapter The ObjectAdapter this ServantLocator is attached to.
 * 					It is used in the allocation process of categories
 * 					whose related objects may hold proxies to objects
 * 					that need to be also allocated
 */
TripOfferLocatorI::TripOfferLocatorI(ObjectAdapterPtr& adapter) {
	_servantMap = new unordered_map<string, DispatchInterceptorPtr>();
	_adapter = adapter;
}

/**
 * Called before a request is dispatched if a servant cannot be found in
 * the object adapter's active servant map. Note that the object adapter
 * does not automatically insert the returned servant into its active
 * servant map. This must be done by the servant locator implementation,
 * if this is desired.
 * locate can throw any user exception. If it does, that exception is
 * marshaled back to the client. If the Slice definition for the
 * corresponding operation includes that user exception, the client
 * receives that user exception; otherwise, the client receives
 * UnknownUserException. If locate throws any exception, the Ice run time
 * does not call finished
 * @param c 		Information about the current operation for which a
 * 						servant is required
 * @param cookie	A "cookie" that will be passed to finished
 * @return			The located servant, or null if no suitable servant
 * 						has been found
 */
ObjectPtr TripOfferLocatorI::locate(const Current& c, LocalObjectPtr& cookie) {
	DispatchInterceptorPtr servant;
	try {
		servant = _servantMap->at(c.id.name);
	} catch (out_of_range&) {
		servant = new DispatchInterceptorI(
				new ::Cardroid::Data::Ice::TripOffer(_adapter));
		_servantMap->insert(
				pair<string, DispatchInterceptorPtr>(c.id.name, servant));
	}
	return servant;
}

TripOfferLocatorI::~TripOfferLocatorI() {
	_servantMap->clear();
	delete _servantMap;
}

/**
 * Constructor for the allocation of those object categories which do
 * require an ObjectAdapter
 * @param adapter The ObjectAdapter this ServantLocator is attached to.
 * 					It is used in the allocation process of categories
 * 					whose related objects may hold proxies to objects
 * 					that need to be also allocated
 */
TripRequestLocatorI::TripRequestLocatorI(ObjectAdapterPtr& adapter) {
	_servantMap = new unordered_map<string, DispatchInterceptorPtr>();
	_adapter = adapter;
}

/**
 * Called before a request is dispatched if a servant cannot be found in
 * the object adapter's active servant map. Note that the object adapter
 * does not automatically insert the returned servant into its active
 * servant map. This must be done by the servant locator implementation,
 * if this is desired.
 * locate can throw any user exception. If it does, that exception is
 * marshaled back to the client. If the Slice definition for the
 * corresponding operation includes that user exception, the client
 * receives that user exception; otherwise, the client receives
 * UnknownUserException. If locate throws any exception, the Ice run time
 * does not call finished
 * @param c 		Information about the current operation for which a
 * 						servant is required
 * @param cookie	A "cookie" that will be passed to finished
 * @return			The located servant, or null if no suitable servant
 * 						has been found
 */
ObjectPtr TripRequestLocatorI::locate(const Current& c,
		LocalObjectPtr& cookie) {
	DispatchInterceptorPtr servant;
	try {
		servant = _servantMap->at(c.id.name);
	} catch (out_of_range&) {
		servant = new DispatchInterceptorI(
				new ::Cardroid::Data::Ice::TripRequest(_adapter));
		_servantMap->insert(
				pair<string, DispatchInterceptorPtr>(c.id.name, servant));
	}
	return servant;
}

TripRequestLocatorI::~TripRequestLocatorI() {
	_servantMap->clear();
	delete _servantMap;
}

/**
 * Constructor for the allocation of those object categories which do
 * require an ObjectAdapter
 * @param adapter The ObjectAdapter this ServantLocator is attached to.
 * 					It is used in the allocation process of categories
 * 					whose related objects may hold proxies to objects
 * 					that need to be also allocated
 */
UserActivityLocatorI::UserActivityLocatorI(ObjectAdapterPtr& adapter) {
	_servantMap = new unordered_map<string, DispatchInterceptorPtr>();
	_adapter = adapter;
}

/**
 * Called before a request is dispatched if a servant cannot be found in
 * the object adapter's active servant map. Note that the object adapter
 * does not automatically insert the returned servant into its active
 * servant map. This must be done by the servant locator implementation,
 * if this is desired.
 * locate can throw any user exception. If it does, that exception is
 * marshaled back to the client. If the Slice definition for the
 * corresponding operation includes that user exception, the client
 * receives that user exception; otherwise, the client receives
 * UnknownUserException. If locate throws any exception, the Ice run time
 * does not call finished
 * @param c 		Information about the current operation for which a
 * 						servant is required
 * @param cookie	A "cookie" that will be passed to finished
 * @return			The located servant, or null if no suitable servant
 * 						has been found
 */
ObjectPtr UserActivityLocatorI::locate(const Current& c,
		LocalObjectPtr& cookie) {
	DispatchInterceptorPtr servant;
	try {
		servant = _servantMap->at(c.id.name);
	} catch (out_of_range&) {
		servant = new DispatchInterceptorI(
				new ::Cardroid::Data::Ice::UserActivity(_adapter));
		_servantMap->insert(
				pair<string, DispatchInterceptorPtr>(c.id.name, servant));
	}
	return servant;
}

UserActivityLocatorI::~UserActivityLocatorI() {
	_servantMap->clear();
	delete _servantMap;
}

/**
 * Constructor for the allocation of those object categories which do
 * require an ObjectAdapter
 * @param adapter The ObjectAdapter this ServantLocator is attached to.
 * 					It is used in the allocation process of categories
 * 					whose related objects may hold proxies to objects
 * 					that need to be also allocated
 */
MessageLocatorI::MessageLocatorI(ObjectAdapterPtr& adapter) {
	_servantMap = new unordered_map<string, DispatchInterceptorPtr>();
	_adapter = adapter;
}

/**
 * Called before a request is dispatched if a servant cannot be found in
 * the object adapter's active servant map. Note that the object adapter
 * does not automatically insert the returned servant into its active
 * servant map. This must be done by the servant locator implementation,
 * if this is desired.
 * locate can throw any user exception. If it does, that exception is
 * marshaled back to the client. If the Slice definition for the
 * corresponding operation includes that user exception, the client
 * receives that user exception; otherwise, the client receives
 * UnknownUserException. If locate throws any exception, the Ice run time
 * does not call finished
 * @param c 		Information about the current operation for which a
 * 						servant is required
 * @param cookie	A "cookie" that will be passed to finished
 * @return			The located servant, or null if no suitable servant
 * 						has been found
 */
ObjectPtr MessageLocatorI::locate(const Current& c, LocalObjectPtr& cookie) {
	DispatchInterceptorPtr servant;
	try {
		servant = _servantMap->at(c.id.name);
	} catch (out_of_range&) {
		servant = new DispatchInterceptorI(
				new ::Cardroid::Data::Ice::Message(_adapter));
		_servantMap->insert(
				pair<string, DispatchInterceptorPtr>(c.id.name, servant));
	}
	return servant;
}

MessageLocatorI::~MessageLocatorI() {
	_servantMap->clear();
	delete _servantMap;
}
