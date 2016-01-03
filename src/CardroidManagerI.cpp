#include "CardroidManagerI.h"

#include <Ice/LocalException.h>
#include <forward_list>

using namespace std;
using namespace Cardroid::Network::Server;
using namespace ::Ice;


boost::mutex CardroidManagerI::_synchronized;

/**
 * Default constructor
 */
CardroidManagerI::CardroidManagerI() :
		_queries(forward_list<QueryProxyPair>()), _destroyed(false) {
}

/**
 * Execute the "getUserPlaces" query in the database
 * @param n			The maximum number of rows to be fetched
 * @param first		Holder for the result of the query to be performed
 * @param nrows		The number of rows already fetched
 * @param result	Holder for the rest of the result (not yet fetched)
 * 						of the query to be performed
 * @param current	Information about the current method invocation for
 * 						servers. Each operation on the server has a
 * 						Current as its implicit final parameter. Current
 * 						is mostly used for Ice services. Most applications
 * 						ignore this parameter.
 */
void CardroidManagerI::getUserPlaces(
		const ::cardroid::data::zerocice::UserTypPrx& usr, Int n,
		::cardroid::ResultSeq& first, Int& nrows,
		::cardroid::QueryResultPrx& result,	const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	context.getUserPlaces(usr, n, first, nrows, result, current);
}

/**
 * Execute the "searchTrips" query in the database
 * @param n			The maximum number of rows to be fetched
 * @param first		Holder for the result of the query to be performed
 * @param nrows		The number of rows already fetched
 * @param result	Holder for the rest of the result (not yet fetched)
 * 						of the query to be performed
 * @param current	Information about the current method invocation for
 * 						servers. Each operation on the server has a
 * 						Current as its implicit final parameter. Current
 * 						is mostly used for Ice services. Most applications
 * 						ignore this parameter.
 */
void CardroidManagerI::searchTrips(
		const ::cardroid::data::zerocice::TripRequestTypPtr& tRequest, Int n,
		::cardroid::ResultSeq& first, Int& nrows,
		 ::cardroid::QueryResultPrx& result, const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	context.searchTrips(tRequest, n, first, nrows, result, current);

}

/**
 * Execute the "getTripFromId" operation in the database
 * @return The result of the operation, if any
 */
::cardroid::data::zerocice::TripTypPrx CardroidManagerI::getTripFromId(
		Int tripId, const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	return context.getTripFromId(tripId, current);
}

/**
 * Execute the "getTripOfferFromId" operation in the database
 * @return The result of the operation, if any
 */
::cardroid::data::zerocice::TripOfferTypPrx CardroidManagerI::getTripOfferFromId(
		Int tripId, const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	return context.getTripOfferFromId(tripId, current);
}

/**
 * Execute the "getTripRequestFromId" operation in the database
 * @return The result of the operation, if any
 */
::cardroid::data::zerocice::TripRequestTypPrx CardroidManagerI::getTripRequestFromId(
		Int tripId, const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	return context.getTripRequestFromId(tripId, current);
}

/**
 * Execute the "joinTrip" operation in the database
 * @return The result of the operation, if any
 */
void CardroidManagerI::joinTrip(
		const ::cardroid::data::zerocice::TripOfferTypPrx& trip,
		const ::cardroid::data::zerocice::UserTypPrx& passenger,
		Int nSeats, const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	context.joinTrip(trip, passenger, nSeats, current);
}

/**
 * Execute the "organizeTrip" operation in the database
 * @return The result of the operation, if any
 */
::cardroid::data::zerocice::TripOfferTypPrx CardroidManagerI::organizeTrip(
		const ::cardroid::data::zerocice::TripRequestTypPrx& tripRequest,
		const ::cardroid::data::zerocice::TripOfferTypPtr& tripOffer,
		const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	return context.organizeTrip(tripRequest, tripOffer, current);
}

/**
 * Execute the "getUserTrips" query in the database
 * @param n			The maximum number of rows to be fetched
 * @param first		Holder for the result of the query to be performed
 * @param nrows		The number of rows already fetched
 * @param result	Holder for the rest of the result (not yet fetched)
 * 						of the query to be performed
 * @param current	Information about the current method invocation for
 * 						servers. Each operation on the server has a
 * 						Current as its implicit final parameter. Current
 * 						is mostly used for Ice services. Most applications
 * 						ignore this parameter.
 */
void CardroidManagerI::getUserTrips(
		const ::cardroid::data::zerocice::UserTypPrx& usr, Int n,
		::cardroid::ResultSeq& first, Int& nrows,
		::cardroid::QueryResultPrx& result, const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	context.getUserTrips(usr, n, first, nrows, result, current);
}

/**
 * Execute the "getPassengerTrips" query in the database
 * @param n			The maximum number of rows to be fetched
 * @param first		Holder for the result of the query to be performed
 * @param nrows		The number of rows already fetched
 * @param result	Holder for the rest of the result (not yet fetched)
 * 						of the query to be performed
 * @param current	Information about the current method invocation for
 * 						servers. Each operation on the server has a
 * 						Current as its implicit final parameter. Current
 * 						is mostly used for Ice services. Most applications
 * 						ignore this parameter.
 */
void CardroidManagerI::getPassengerTrips(
		const ::cardroid::data::zerocice::UserTypPrx& passenger, Int n,
		::cardroid::ResultSeq& first, Int& nrows,
		::cardroid::QueryResultPrx& result, const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	context.getPassengerTrips(passenger, n, first, nrows, result, current);
}

/**
 * Execute the "calculatePriceEstimation" operation in the database
 * @return The result of the operation, if any
 */
Double CardroidManagerI::calculatePriceEstimation(
		::cardroid::data::zerocice::Fuel f, Int distance,
		 const Current& current) const {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	//reapQueries();

	return context.calculatePriceEstimation(f, distance, current);
}

/**
 * Execute the "newTripOffer" operation in the database
 * @return The result of the operation, if any
 */
::cardroid::data::zerocice::TripOfferTypPrx CardroidManagerI::newTripOffer(
		const ::cardroid::data::zerocice::TripOfferTypPtr& tOffer,
		const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	return context.newTripOffer(tOffer, current);
}

/**
 * Execute the "newTripRequest" operation in the database
 * @return The result of the operation, if any
 */
::cardroid::data::zerocice::TripRequestTypPrx CardroidManagerI::newTripRequest(
		const ::cardroid::data::zerocice::TripRequestTypPtr& tRequest,
		const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	return context.newTripRequest(tRequest, current);
}

/**
 * Execute the "getMessageTalksSpeakers" query in the database
 * @param n			The maximum number of rows to be fetched
 * @param first		Holder for the result of the query to be performed
 * @param nrows		The number of rows already fetched
 * @param result	Holder for the rest of the result (not yet fetched)
 * 						of the query to be performed
 * @param current	Information about the current method invocation for
 * 						servers. Each operation on the server has a
 * 						Current as its implicit final parameter. Current
 * 						is mostly used for Ice services. Most applications
 * 						ignore this parameter.
 */
void CardroidManagerI::getMessageTalksSpeakers(
		const ::cardroid::data::zerocice::UserTypPrx& usr, Int n,
		::cardroid::ResultSeq& first, Int& nrows,
		::cardroid::QueryResultPrx& result, const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	context.getMessageTalksSpeakers(usr, n, first, nrows, result, current);
}

/**
 * Execute the "getMessageTalks" query in the database
 * @param n			The maximum number of rows to be fetched
 * @param first		Holder for the result of the query to be performed
 * @param nrows		The number of rows already fetched
 * @param result	Holder for the rest of the result (not yet fetched)
 * 						of the query to be performed
 * @param current	Information about the current method invocation for
 * 						servers. Each operation on the server has a
 * 						Current as its implicit final parameter. Current
 * 						is mostly used for Ice services. Most applications
 * 						ignore this parameter.
 */
void CardroidManagerI::getMessageTalks(
		const ::cardroid::data::zerocice::UserTypPrx& usr1,
		const ::cardroid::data::zerocice::UserTypPrx& usr2,
		Int n, ::cardroid::ResultSeq& first, Int& nrows,
		::cardroid::QueryResultPrx& result,
		const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	context.getMessageTalks(usr1, usr2, n, first, nrows, result, current);
}

/**
 * Execute the "newMessage" operation in the database
 * @return The result of the operation, if any
 */
::cardroid::data::zerocice::MessageTypPrx CardroidManagerI::newMessage(
		const ::cardroid::data::zerocice::UserTypPrx& usr1,
		const ::cardroid::data::zerocice::UserTypPrx& usr2,
		const string& message, const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	return context.newMessage(usr1, usr2, message, current);
}

/**
 * Execute the "getUserActivity" query in the database
 * @param n			The maximum number of rows to be fetched
 * @param first		Holder for the result of the query to be performed
 * @param nrows		The number of rows already fetched
 * @param result	Holder for the rest of the result (not yet fetched)
 * 						of the query to be performed
 * @param current	Information about the current method invocation for
 * 						servers. Each operation on the server has a
 * 						Current as its implicit final parameter. Current
 * 						is mostly used for Ice services. Most applications
 * 						ignore this parameter.
 */
void CardroidManagerI::getUserActivity(
		const ::cardroid::data::zerocice::UserTypPrx& usr, Int n,
		::cardroid::ResultSeq& first, Int& nrows,
		::cardroid::QueryResultPrx& result, const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	context.getUserActivity(usr, n, first, nrows, result, current);
}

/**
 * Execute the "getUserFromEmail" operation in the database
 * @return The result of the operation, if any
 */
::cardroid::data::zerocice::UserTypPrx CardroidManagerI::getUserFromEmail(
		const string& email, const Current&current) {
	SQLRequestContext context = SQLRequestContext::getCurrentContext();

	reapQueries();

	return context.getUserFromEmail(email, current);
}

/**
 * Execute the "getCarFromPlate" operation in the database
 * @return The result of the operation, if any
 */
::cardroid::data::zerocice::CarTypPrx CardroidManagerI::getCarFromPlate(
		const string& plate, const ::cardroid::data::zerocice::UserTypPrx& owner,
		const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	return context.getCarFromPlate(plate, owner, current);
}

/**
 * Execute the "getCarFromPlateEmail" operation in the database
 * @return The result of the operation, if any
 */
::cardroid::data::zerocice::CarTypPrx CardroidManagerI::getCarFromPlateEmail(
		const string& plate, const string& ownerEmail,
		const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	return context.getCarFromPlateEmail(plate, ownerEmail, current);
}

/**
 * Execute the "updateUserData" operation in the database
 * @return The result of the operation, if any
 */
::cardroid::data::zerocice::UserTypPrx CardroidManagerI::updateUserData(
		const ::cardroid::data::zerocice::UserTypPtr& usr,
		const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	return context.updateUserData(usr, current);
}

/**
 * Execute the "updateCarData" operation in the database
 * @return The result of the operation, if any
 */
::cardroid::data::zerocice::CarTypPrx CardroidManagerI::updateCarData(
		const ::cardroid::data::zerocice::CarTypPtr& car,
		const ::cardroid::data::zerocice::UserTypPtr& usr,
		const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	return context.updateCarData(car, usr, current);
}

/**
 * Execute the "updateCarDataEmail" operation in the database
 * @return The result of the operation, if any
 */
::cardroid::data::zerocice::CarTypPrx CardroidManagerI::updateCarDataEmail(
		const ::cardroid::data::zerocice::CarTypPtr& car, const string& usrEmail,
		const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	return updateCarDataEmail(car, usrEmail, current);
}

/**
 * Execute the "addCar" operation in the database
 * @return The result of the operation, if any
 */
::cardroid::data::zerocice::CarTypPrx CardroidManagerI::addCar(
		const ::cardroid::data::zerocice::CarTypPtr& car,
		const ::cardroid::data::zerocice::UserTypPrx& usr,
		const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	return context.addCar(car, usr, current);
}

/**
 * Execute the "addCarEmail" operation in the database
 * @return The result of the operation, if any
 */
::cardroid::data::zerocice::CarTypPrx CardroidManagerI::addCarEmail(
		const ::cardroid::data::zerocice::CarTypPtr& car, const string& usrEmail,
		const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	return context.addCarEmail(car, usrEmail, current);
}

/**
 * Execute the "removeCar" operation in the database
 * @return The result of the operation, if any
 */
void CardroidManagerI::removeCar(const ::cardroid::data::zerocice::CarTypPrx& car,
		const ::cardroid::data::zerocice::UserTypPrx& usr,
		const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	context.removeCar(car, usr, current);
}

/**
 * Execute the "removeCarPlateEmail" operation in the database
 * @return The result of the operation, if any
 */
void CardroidManagerI::removeCarPlateEmail(const string& plate,
		const string& ownerEmail, const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	context.removeCarPlateEmail(plate, ownerEmail, current);
}

/**
 * Execute the "userTripRegistered" operation in the database
 * @return The result of the operation, if any
 */
bool CardroidManagerI::userTripRegistered(
		const ::cardroid::data::zerocice::UserTypPrx& usr,
		const ::cardroid::data::zerocice::TripTypPrx& trip,
		const Current& current) {
	SQLRequestContext& context = SQLRequestContext::getCurrentContext();
	context.setQueryResultListener(*this);

	reapQueries();

	return context.userTripRegistered(usr, trip, current);
}

/**
 * Destroy this CardroidManager and the query result objects it currently
 * holds
 */
void CardroidManagerI::destroy() {
	_synchronized.lock();

	if (_destroyed) {
		_synchronized.unlock();
		return;
	}
	_destroyed = true;
	for (auto p = _queries.begin(); p != _queries.end(); ++p) {
		try {
			p->proxy->destroy();
		} catch (ObjectNotExistException&) {
			// Ignore, it could have already been destroyed
		}
	}

	_synchronized.unlock();
}

/**
 * Shutdown this CardroidManager and the query result objects it currently
 * holds
 */
void CardroidManagerI::shutdown() {
	_synchronized.lock();

	if (_destroyed) {
		_synchronized.unlock();
		return;
	}
	_destroyed = true;

	// Shutdown each of the associated query objects
	for (auto p = _queries.begin(); p != _queries.end(); ++p) {
		p->impl->shutdown();
	}

	_synchronized.unlock();
}

/**
 * Add a new query result to the list of query result objects currently
 * kept by this CardroidManager
 * @param proxy		Proxy to the query result to add
 * @param impl		Servant of the query result to add
 * @param queryType	The type of the query which originated the result to add
 */
void CardroidManagerI::add(const ::cardroid::QueryResultPrx& proxy,
		QueryResultI* impl,	QueryType queryType) {
	_synchronized.lock();

	/* If the session has been destroyed, then destroy the query result, *
	 * and raise an ObjectNotExistException								 */
	if (_destroyed) {
		proxy->destroy();
		throw ObjectNotExistException(__FILE__, __LINE__);
	}
	if (_queries.empty()) {
		_queries.push_front(QueryProxyPair(proxy, impl));
	} else {
		auto beforeIt = _queries.cbefore_begin();
		for (auto it = _queries.begin(); it != _queries.end(); ++it, ++beforeIt)
			;
		_queries.insert_after(beforeIt, QueryProxyPair(proxy, impl));
	}

	_synchronized.unlock();
}

/**
 * Remove the context of the queries held by this CardroidManager which
 * have already been destroyed
 */
void CardroidManagerI::reapQueries() {
	_synchronized.lock();

	if (_destroyed) {
		throw ObjectNotExistException(__FILE__, __LINE__);
	}

	auto beforeIt = _queries.cbefore_begin();
	for (auto it = _queries.begin(); it != _queries.end();) {
		bool increase = true;
		try {
			it->proxy->ice_ping();
		} catch (ObjectNotExistException& e) {
			increase = false;
			it = _queries.erase_after(beforeIt);
		}
		if (increase) {
			++beforeIt;
			++it;
		}
	}

	_synchronized.unlock();
}

QueryProxyPair::QueryProxyPair(const cardroid::QueryResultPrx& p, QueryResultI* i) :
		proxy(p), impl(i) {
}
