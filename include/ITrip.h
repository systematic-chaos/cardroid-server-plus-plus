#ifndef ITRIP_H_
#define ITRIP_H_

#include <Ice/Current.h>
#include <Ice/Handle.h>
#include <Ice/Identity.h>
#include <Ice/ObjectAdapter.h>
#include <Ice/ProxyHandle.h>
#include <IceUtil/Handle.h>
#include <IceUtil/Optional.h>
#include <occiControl.h>
#include <occiData.h>
#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Cardroid.h"
#include "DbTypes.h"
#include "IDate.h"
#include "IPlace.h"

using namespace std;

namespace Cardroid {
namespace Data {

/**
 * \interface ITrip
 * Public operations interface for the implementation of a Trip object
 */
class ITrip {

public:

	/**
	 * \enum Periodicity
	 * enum type depicting the possible week values for
	 * scheduling a periodic trip
	 */
	enum Periodicity {
		EVERYWEEK, EVENWEEKS, ODDWEEKS
	};

	/**
	 * Create an instance of the class implementing this interface,
	 * from the received parameter, which also implements it.
	 * This method acts as an Abstract Factory, for the sake of the
	 * implementation of the Abstract Factory pattern in the search of
	 * interoperability among the different implementations of this interface
	 * which could exist in the different subsystems of the platform
	 * @param tripObject An object instance implementing this interface
	 * @return An instance of the class implementing this interface, containing
	 * 			exactly the same data of the received bitmapObject parameter,
	 * 			from the viewpoint of the operations defined in this interface
	 */
	virtual shared_ptr<ITrip> newInstance(
			const shared_ptr<ITrip> tripObject) = 0;

	/**
	 * @param tripId The new identifier for this ITrip
	 */
	virtual void setTripId(int tripId) = 0;

	/**
	 * @return The identifier for this ITrip
	 */
	virtual int getTripId() = 0;

	/**
	 * @param from The new origin place for this ITrip
	 */
	virtual void setFromPlace(const IPlace& from) = 0;

	/**
	 * @return The origin place for this ITrip
	 */
	virtual const IPlace& getFromPlace() = 0;

	/**
	 * @param to The new destination place for this ITrip
	 */
	virtual void setToPlace(const IPlace& to) = 0;

	/**
	 * @return The destination place for this ITrip
	 */
	virtual const IPlace& getToPlace() = 0;

	/**
	 * @param datetime The new date and time for this ITrip
	 */
	virtual void setDate(IDate& datetime) = 0;

	/**
	 * @return The date and time for this ITrip
	 */
	virtual IDate& getDate() = 0;

	/**
	 * @param nSeats The new number of seats for this ITrip (offered or
	 * 					requested, depending on its nature)
	 */
	virtual void setNSeats(int nSeats) = 0;

	/**
	 * @return The number of seats for this ITrip (offered or requested,
	 * 			depending on its nature)
	 */
	virtual int getNSeats() = 0;

	/**
	 * @param returnDatetime The new return date and time for this ITrip
	 */
	virtual void setReturnDate(IDate& returnDateTime) = 0;

	/**
	 * @return The return date and time for this ITrip
	 */
	virtual IDate& getReturnDate() = 0;

	/**
	 * @return Whether this Trip has a return date and time
	 */
	virtual bool hasReturnDate() = 0;

	/**
	 * @param weekDays 		The week days on which this ITrip will be
	 * 							periodically performed
	 * @param periodicity 	The weekly periodicity for this ITrip
	 */
	virtual void setWeekDaysPeriodicity(array<char, 7> weekDays,
			Periodicity periodicity) = 0;

	/**
	 * \enum Periodicity
	 * enum type depicting the possible week values for
	 * scheduling a periodic trip
	 */
	virtual array<char, 7> getWeekDays() = 0;

	/**
	 * @return The weekly periodicity for this ITrip
	 */
	virtual Periodicity getPeriodicity() = 0;

	/**
	 * @return Whether this ITrip will be performed on a periodic fashion
	 */
	virtual bool hasWeekDaysPeriodicity() = 0;

	/**
	 * @param distance The new distance covered by this ITrip,
	 * 					expressed in kilometers
	 */
	virtual void setDistance(int distance) = 0;

	/**
	 * @return The distance covered by this ITrip, expressed in kilometers
	 */
	virtual int getDistance() = 0;

	/**
	 * @return Whether this ITrip has a value for this distance it covers
	 */
	virtual bool hasDistance() = 0;

	/**
	 * @param characteristics The new characteristics for this ITrip
	 */
	virtual void setCharacteristics(const string& characteristics) = 0;

	/**
	 * @return The additional characteristics for this ITrip
	 */
	virtual const string& getCharacteristics() = 0;

	/**
	 * @return Whether this ITrip has additional characteristics
	 */
	virtual bool hasCharacteristics() = 0;

	virtual void setTripType(int tripType) = 0;

	virtual int getTripType() = 0;

	virtual ~ITrip() {
	}
};

namespace Ice {

using namespace IceProxy;
using namespace Cardroid::Data;

/**
 * \class Trip
 * Domain class implementing a Trip for its transmission between systems
 * communicating across an Ice network infrastructure
 */
class Trip: public ::cardroid::data::zerocice::TripTyp,
		public ITrip, public ::Ice::ObjectFactory {

public:

	Trip() {
	}

	/**
	 * Short constructor
	 */

	Trip(::cardroid::data::zerocice::PlaceTyp* fp,
			::cardroid::data::zerocice::PlaceTyp* tp,
			::cardroid::data::zerocice::DateTyp* td, int ns, int tt) {
		tripId = -1;
		fromPlace = fp;
		toPlace = tp;
		tDate = td;
		nSeats = ns;
		tripType = tt;
	}

	Trip(::cardroid::data::zerocice::PlaceTyp* fp,
			::cardroid::data::zerocice::PlaceTyp* tp,
			::cardroid::data::zerocice::DateTyp* td, int ns,
			::cardroid::data::zerocice::DateTyp* rdt, vector<string> wd,
			::cardroid::data::zerocice::Periodicity tperiod, int d,
			const string& c, int tt) {
		tripId = -1;
		fromPlace = fp;
		toPlace = tp;
		tDate = td;
		nSeats = ns;
		tReturnDate = rdt;
		tWeekDays = wd;
		tPeriodicity = tperiod;
		tripDistance = d;
		tripCharacteristics = c;
		tripType = tt;
	}

	Trip(int ti, ::cardroid::data::zerocice::PlaceTyp* fp,
			::cardroid::data::zerocice::PlaceTyp* tp,
			::cardroid::data::zerocice::DateTyp* td, int ns, int tt) {
		tripId = ti;
		fromPlace = fp;
		toPlace = tp;
		tDate = td;
		nSeats = ns;
		tripType = tt;
	}

	Trip(int ti, ::cardroid::data::zerocice::PlaceTyp* fp,
			::cardroid::data::zerocice::PlaceTyp* tp,
			::cardroid::data::zerocice::DateTyp* td, int ns,
			::cardroid::data::zerocice::DateTyp* rdt, vector<string> wd,
			::cardroid::data::zerocice::Periodicity tperiod, int d,
			const string& c, int tt) {
		tripId = ti;
		fromPlace = fp;
		toPlace = tp;
		tDate = td;
		nSeats = ns;
		tReturnDate = rdt;
		tWeekDays = wd;
		tPeriodicity = tperiod;
		tripDistance = d;
		tripCharacteristics = c;
		tripType = tt;

	}

	Trip(const ::cardroid::data::zerocice::TripTyp& trip) {
		tripId = trip.tripId;
		fromPlace = trip.fromPlace;
		toPlace = trip.toPlace;
		tDate = trip.tDate;
		nSeats = trip.nSeats;
		tripType = trip.tripType;
		if (trip.hasTripReturnDate())
			super::setTripReturnDate(trip.tReturnDate.get());
		if (trip.hasWeekDaysPeriodicity())
			super::setTripWeekDaysPeriodicity(trip.tWeekDays.get(),
					trip.tPeriodicity.get());
		if (trip.hasDistance())
			super::setDistance(trip.tripDistance.get());
		if (trip.hasCharacteristics())
			super::setCharacteristics(trip.tripCharacteristics.get());
	}

	Trip(const Trip& t) {
		Trip& trip = const_cast<Trip&>(t);
		tripId = trip.tripId;
		fromPlace = trip.fromPlace;
		toPlace = trip.toPlace;
		tDate = trip.tDate;
		nSeats = trip.nSeats;
		tripType = trip.tripType;
		if (trip.tReturnDate)
			tReturnDate = trip.tReturnDate.get();
		if (trip.tWeekDays && trip.tPeriodicity) {
			tWeekDays = trip.tWeekDays.get();
			tPeriodicity = trip.tPeriodicity.get();
		}
		if (trip.tripDistance)
			tripDistance = trip.tripDistance.get();
		if (trip.tripCharacteristics)
			tripCharacteristics = trip.tripCharacteristics.get();
	}

	Trip(Trip&& trip) {
		swap(tripId, trip.tripId);
		swap(fromPlace, trip.fromPlace);
		swap(toPlace, trip.toPlace);
		swap(tDate, trip.tDate);
		swap(nSeats, trip.nSeats);
		swap(tripType, trip.tripType);
		if (trip.tReturnDate)
			tReturnDate = trip.tReturnDate.get();
		if (trip.tWeekDays && trip.tPeriodicity) {
			tWeekDays = trip.tWeekDays.get();
			tPeriodicity = trip.tPeriodicity.get();
		}
		if (trip.tripDistance)
			tripDistance = trip.tripDistance.get();
		if (trip.tripCharacteristics)
			tripCharacteristics = trip.tripCharacteristics.get();
	}

	~Trip() {
		delete &fromPlace;
		delete &toPlace;
		delete &tDate;
		if (tReturnDate)
			delete &tReturnDate;
		if (tWeekDays && tPeriodicity) {
			delete &tWeekDays;
			delete &tPeriodicity;
		}
		if (tripDistance)
			delete &tripDistance;
		if(tripCharacteristics)
			delete &tripCharacteristics;
	}

	Trip& operator=(Trip trip) {
		tripId = trip.tripId;
		fromPlace = trip.fromPlace;
		toPlace = trip.toPlace;
		tDate = trip.tDate;
		nSeats = trip.nSeats;
		tripType = trip.tripType;
		if (trip.tReturnDate)
			tReturnDate = trip.tReturnDate.get();
		if (trip.tWeekDays && trip.tPeriodicity) {
			tWeekDays = trip.tWeekDays.get();
			tPeriodicity = trip.tPeriodicity.get();
		}
		if (trip.tripDistance)
			tripDistance = trip.tripDistance.get();
		if (trip.tripCharacteristics)
			tripCharacteristics = trip.tripCharacteristics.get();
		return *this;
	}

	Trip& operator=(Trip& trip) {
		swap(tripId, trip.tripId);
		swap(fromPlace, trip.fromPlace);
		swap(toPlace, trip.toPlace);
		swap(tDate, trip.tDate);
		swap(nSeats, trip.nSeats);
		swap(tripType, trip.tripType);
		if (trip.hasTripReturnDate())
			swap(tReturnDate, trip.tReturnDate);
		if (trip.hasWeekDaysPeriodicity()) {
			swap(tWeekDays, trip.tWeekDays);
			swap(tPeriodicity, trip.tPeriodicity);
		}
		if (trip.hasDistance())
			swap(tripDistance, trip.tripDistance);
		if (trip.hasCharacteristics())
			swap(tripCharacteristics, trip.tripCharacteristics);
		return *this;
	}

	/**
	 * @return An Ice Identity for this datatype category and the data provided
	 */
	static ::Ice::Identity createIdentity(int tripId) {
		::Ice::Identity id;
		id.category = "trip";
		id.name = to_string(tripId);
		return id;
	}

	/**
	 * @return A proxy to an Ice Object incarnating the provided data, whose
	 * 			servant is added to adapter 's active servant map
	 */
	static ::cardroid::data::zerocice::TripTypPrx createProxy(
			::cardroid::data::zerocice::TripTyp& tripObject,
			::Ice::ObjectAdapterPtr adapter) {
		::cardroid::data::zerocice::TripTypPrx tripPrx =
				::cardroid::data::zerocice::TripTypPrx::uncheckedCast(
						adapter->createProxy(
								createIdentity(tripObject.getTripId())));

			fillProxyData(tripObject, tripPrx, adapter);
		return tripPrx;
	}

	/**
	 * @return A proxy to an Ice Object incarnating the provided data, whose
	 * 			servant is added to adapter 's active servant map
	 */
	static ::cardroid::data::zerocice::TripTypPrx createProxy(ITrip& tripObject,
			::Ice::ObjectAdapterPtr adapter) {
		::cardroid::data::zerocice::TripTypPrx tripPrx =
				::cardroid::data::zerocice::TripTypPrx::uncheckedCast(
						adapter->createProxy(
								createIdentity(tripObject.getTripId())));

			fillProxyData(tripObject, tripPrx, adapter);
		return tripPrx;
	}

	/**
	 * Fill the servant referenced by the given proxy with the data contained
	 * in the given object
	 */
	static void fillProxyData(const ::cardroid::data::zerocice::TripTyp& object,
			::cardroid::data::zerocice::TripTypPrx proxy,
			::Ice::ObjectAdapterPtr adapter) {
		proxy->setTripId(object.getTripId());
		proxy->setPlace1(object.getPlace1());
		proxy->setPlace2(object.getPlace2());
		proxy->setTripDate(object.getTripDate());
		proxy->setNSeats(object.getNSeats());
		proxy->setTripType(object.getTripType());
		if (object.hasTripReturnDate())
			proxy->setTripReturnDate(object.getTripDate());
		if (object.hasWeekDaysPeriodicity()) {
			::cardroid::data::zerocice::Periodicity periodicity =
					object.getTripPeriodicity();
			vector<string> weekDays = object.getTripWeekDays();
			proxy->setTripWeekDaysPeriodicity(weekDays, periodicity);
		}
		if (object.hasDistance())
			proxy->setDistance(object.getDistance());
		if (object.hasCharacteristics())
			proxy->setCharacteristics(object.getCharacteristics());
	}

	/**
	 * Fill the servant referenced by the given proxy with the data contained
	 * in the given object
	 */
	static void fillProxyData(ITrip& object,
			::cardroid::data::zerocice::TripTypPrx proxy,
			::Ice::ObjectAdapterPtr adapter) {
		proxy->setTripId(object.getTripId());
		proxy->setPlace1(
				static_pointer_cast<Place>(Place().newInstance(shared_ptr<IPlace>(&(const_cast<IPlace&>(object.getFromPlace()))))).get());
		proxy->setPlace2(
				static_pointer_cast<Place>(Place().newInstance(shared_ptr<IPlace>(&(const_cast<IPlace&>(object.getToPlace()))))).get());
		proxy->setTripDate(
				static_pointer_cast<Date>(Date().newInstance(shared_ptr<IDate>(&(object.getDate())))).get());
		proxy->setNSeats(object.getNSeats());
		proxy->setTripType(object.getTripType());
		if (object.hasReturnDate())
			proxy->setTripReturnDate(
					static_pointer_cast<Date>(Date().newInstance(
							shared_ptr<IDate>(&(object.getReturnDate())))).get());
		if (object.hasWeekDaysPeriodicity()) {
			::cardroid::data::zerocice::Periodicity periodicity =
					static_cast< ::cardroid::data::zerocice::Periodicity>(object.getPeriodicity());
			array<char, 7> wd = object.getWeekDays();
			vector<string> weekDays;
			for (auto n = wd.begin(); n != wd.end(); n++)
				weekDays.push_back(to_string(*n));
			proxy->setTripWeekDaysPeriodicity(weekDays, periodicity);
		}
		if (object.hasDistance())
			proxy->setDistance(object.getDistance());
		if (object.hasCharacteristics())
			proxy->setCharacteristics(object.getCharacteristics());
	}

	/**
	 * @param proxy A proxy to a remote object implementing a Trip
	 * @return A local Trip object containing the data of the remote object
	 * 			referenced by proxy
	 */
	static shared_ptr<Trip> extractObject(
			::cardroid::data::zerocice::TripTypPrx proxy) {
		shared_ptr<Trip> trip;

		if (!proxy->hasTripReturnDate() && proxy->hasWeekDaysPeriodicity()
				&& proxy->hasDistance() && proxy->hasCharacteristics()) {
			trip = make_shared<Trip>(proxy->getTripId(), proxy->getPlace1().get(),
					proxy->getPlace2().get(), proxy->getTripDate().get(),
					proxy->getNSeats(), proxy->getTripType());
			if (!proxy->hasTripReturnDate() && !proxy->hasWeekDaysPeriodicity()
					&& !proxy->hasDistance() && !proxy->hasCharacteristics())
				return trip;
		}

		::cardroid::data::zerocice::DateTyp* returnDate =
				proxy->getTripReturnDate().get();
		const vector<string>& weekDays = proxy->getTripWeekDays();
		::cardroid::data::zerocice::Periodicity periodicity =
				proxy->getTripPeriodicity();
		int distance = proxy->getDistance();
		const string& characteristics = proxy->getCharacteristics();

		if (!(proxy->hasTripReturnDate() && proxy->hasWeekDaysPeriodicity()
				&& proxy->hasDistance() && proxy->hasCharacteristics())) {
			if (proxy->hasTripReturnDate())
				trip->setTripReturnDate(returnDate);
			if (proxy->hasWeekDaysPeriodicity())
				trip->setTripWeekDaysPeriodicity(weekDays, periodicity);
			if (proxy->hasDistance())
				static_pointer_cast<super>(trip)->setDistance(distance);
			if (proxy->hasCharacteristics())
				static_pointer_cast<super>(trip)->setCharacteristics(characteristics);
		} else
			trip = make_shared<Trip>(proxy->getTripId(), proxy->getPlace1().get(),
					proxy->getPlace2().get(), proxy->getTripDate().get(),
					proxy->getNSeats(), returnDate, weekDays, periodicity,
					distance, characteristics, proxy->getTripType());

		return trip;
	}

	void setAdapter(::Ice::ObjectAdapterPtr adapter) {
		_adapter = adapter;
	}

	/* ITrip interface */

	shared_ptr<ITrip> newInstance(const shared_ptr<ITrip> tripObject) {
		if (!tripObject)
			return NULL;
		if (dynamic_cast<Trip*>(tripObject.get()))
			return tripObject;

		shared_ptr<ITrip> trip = NULL;
		Place* fromPlace = static_pointer_cast<Place>(Place().newInstance(
				shared_ptr<IPlace>(&const_cast<IPlace&>(tripObject->getFromPlace())))).get();
		Place* toPlace = static_pointer_cast<Place>(Place().newInstance(
				shared_ptr<IPlace>(&const_cast<IPlace&>(tripObject->getToPlace())))).get();
		Date* datetime = static_pointer_cast<Date>(Date().newInstance(
				shared_ptr<IDate>(&(tripObject->getDate())))).get();

		if (!(tripObject->hasReturnDate()
				&& tripObject->hasWeekDaysPeriodicity()
				&& tripObject->hasDistance() && tripObject->hasCharacteristics())) {
			trip = static_pointer_cast<ITrip>(make_shared<Trip>(tripObject->getTripId(), fromPlace, toPlace,
					datetime, tripObject->getNSeats(),
					tripObject->getTripType()));
			if (!tripObject->hasReturnDate()
					&& !tripObject->hasWeekDaysPeriodicity()
					&& !tripObject->hasDistance()
					&& !tripObject->hasCharacteristics())
				return trip;
		}

		Date* returnDatetime = NULL;
				if (tripObject->hasReturnDate())
					returnDatetime = static_pointer_cast<Date>(Date().newInstance(
							shared_ptr<IDate>(&(tripObject->getReturnDate())))).get();
		::cardroid::data::zerocice::Periodicity periodicity;
		if (tripObject->getPeriodicity())
			periodicity =
					static_cast< ::cardroid::data::zerocice::Periodicity>(tripObject->getPeriodicity());
		vector<string> weekDays;
		if (tripObject->hasWeekDaysPeriodicity()) {
			array<char, 7> wd = tripObject->getWeekDays();
			tripObject->getWeekDays();
			for (auto n = wd.begin(); n != wd.end(); n++)
				weekDays.push_back(to_string(*n));
		}
		int distance = tripObject->getDistance();
		string characteristics = tripObject->getCharacteristics();

		if (!(tripObject->hasReturnDate()
				&& tripObject->hasWeekDaysPeriodicity()
				&& tripObject->hasDistance() && tripObject->hasCharacteristics())) {
			if (tripObject->hasReturnDate())
				trip->setReturnDate(tripObject->getReturnDate());
			if (tripObject->hasWeekDaysPeriodicity())
				trip->setWeekDaysPeriodicity(tripObject->getWeekDays(), tripObject->getPeriodicity());
			if (tripObject->hasDistance())
				trip->setDistance(tripObject->getDistance());
			if (tripObject->hasCharacteristics())
				trip->setCharacteristics(tripObject->getCharacteristics());
		} else
			trip = static_pointer_cast<ITrip>(make_shared<Trip>(tripObject->getTripId(), fromPlace, toPlace,
					datetime, tripObject->getNSeats(), returnDatetime, weekDays,
					periodicity, distance, characteristics,
					tripObject->getTripType()));

		return trip;
	}

	void setFromPlace(const IPlace& from) {
		fromPlace = static_pointer_cast<Place>(Place().newInstance(shared_ptr<IPlace>(&const_cast<IPlace&>(from)))).get();
	}

	const IPlace& getFromPlace() {
		return const_cast<const IPlace&>(static_cast<IPlace&>(*(dynamic_cast<Place*>(fromPlace.get()) ?
				static_cast<Place*>(fromPlace.get()) : new Place(*(fromPlace.get())))));
	}

	void setToPlace(const IPlace& to) {
		toPlace = static_pointer_cast<Place>(Place().newInstance(shared_ptr<IPlace>(&const_cast<IPlace&>(to)))).get();
	}

	const IPlace& getToPlace() {
		return const_cast<const IPlace&>(static_cast<IPlace&>(*(dynamic_cast<Place*>(toPlace.get()) ?
				static_cast<Place*>(toPlace.get()) : new Place(*(toPlace.get())))));
	}

	void setDate(IDate& datetime) {
		tDate = static_pointer_cast<Date>(Date().newInstance(shared_ptr<IDate>(&datetime))).get();
	}

	IDate& getDate() {
		return static_cast<IDate&>(*(dynamic_cast<Date*>(tDate.get()) ? static_cast<Date*>(tDate.get()) : new Date(*(tDate.get()))));
	}

	void setReturnDate(IDate& returnDatetime) {
		tReturnDate =
				static_pointer_cast<Date>(Date().newInstance(shared_ptr<IDate>(&returnDatetime))).get();
	}

	IDate& getReturnDate() {
		if (!hasTripReturnDate())
			return dynamic_cast<IDate&>(*dynamic_cast<Date*>(getTripReturnDate().get()));
		::cardroid::data::zerocice::DateTypPtr rdt = getTripReturnDate();
		return static_cast<IDate&>(*(dynamic_cast<Date*>(rdt.get()) ? static_cast<Date*>(rdt.get()) : new Date(*(rdt.get()))));
	}

	bool hasReturnDate() {
		return hasTripReturnDate();
	}

	void setWeekDaysPeriodicity(array<char, 7> weekDays,
			ITrip::Periodicity periodicity) {
		vector<string> wd;
		for (auto n = weekDays.begin(); n != weekDays.end(); n++)
			wd.push_back(to_string(*n));
		::cardroid::data::zerocice::Periodicity p =
				static_cast< ::cardroid::data::zerocice::Periodicity>(periodicity);
		setTripWeekDaysPeriodicity(wd, p);
	}

	array<char, 7> getWeekDays() {
		array<char,7> wd = array<char, 7>();
		if (!hasWeekDaysPeriodicity())
			return wd;
		vector<string> weekDays = tWeekDays.get();
		for (unsigned int n = 0; n < weekDays.size(); n++)
			wd[n] = weekDays[n].c_str()[0];
		return wd;
	}

	ITrip::Periodicity getPeriodicity() {
		return hasWeekDaysPeriodicity() ?
				static_cast<ITrip::Periodicity>(tPeriodicity.get()) : ITrip::Periodicity::EVERYWEEK;
	}

	bool hasWeekDaysPeriodicity() {
		return tWeekDays && tPeriodicity;
	}

	void setDistance(int d) {
		tripDistance = d;
	}

	int getDistance() {
		return tripDistance ? tripDistance.get() : 0;
	}

	bool hasDistance() {
		return tripDistance;
	}

	void setNSeats(int ns) {
		nSeats = ns;
	}

	int getNSeats() {
		return nSeats;
	}

	void setTripType(int tt) {
		tripType = tt;
	}

	int getTripType() {
		return tripType;
	}

	void setTripId(int ti) {
		tripId = ti;
	}

	int getTripId() {
		return tripId;
	}

	void setCharacteristics(const string& c) {
		tripCharacteristics = c;
	}

	const string& getCharacteristics() {
		return tripCharacteristics.get();
	}

	bool hasCharacteristics() {
		return tripCharacteristics;
	}

	/* Overriding superclass */

	static const string& ice_staticId() {
		return super::ice_staticId();
	}

	int getTripId(const ::Ice::Current& = ::Ice::Current()) const {
		return tripId;
	}

	void setTripId(int tId, const ::Ice::Current& = ::Ice::Current()) {
		tripId = tId;
	}

	::cardroid::data::zerocice::PlaceTypPtr getPlace1(const ::Ice::Current& =
			::Ice::Current()) const {
		return fromPlace;
	}

	void setPlace1(const ::cardroid::data::zerocice::PlaceTypPtr& fp,
			const ::Ice::Current& = ::Ice::Current()) {
		fromPlace = fp;
	}

	::cardroid::data::zerocice::PlaceTypPtr getPlace2(const ::Ice::Current& =
			::Ice::Current()) const {
		return toPlace;
	}

	void setPlace2(const ::cardroid::data::zerocice::PlaceTypPtr& tp,
			const ::Ice::Current& = ::Ice::Current()) {
		toPlace = tp;
	}

	::cardroid::data::zerocice::DateTypPtr getTripDate(const ::Ice::Current& =
			::Ice::Current()) const {
		return tDate;
	}

	void setTripDate(const ::cardroid::data::zerocice::DateTypPtr& td,
			const ::Ice::Current& = ::Ice::Current()) {
		tDate = td;
	}

	int getNSeats(const ::Ice::Current& = ::Ice::Current()) const {
		return nSeats;
	}

	void setNSeats(int ns, const ::Ice::Current& = ::Ice::Current()) {
		nSeats = ns;
	}

	::cardroid::data::zerocice::DateTypPtr getTripReturnDate(
			const ::Ice::Current& = ::Ice::Current()) const {
		return tReturnDate ? tReturnDate.get() : (::cardroid::data::zerocice::DateTypPtr)NULL;
	}

	void setTripReturnDate(const ::cardroid::data::zerocice::DateTypPtr& trd,
			const ::Ice::Current& = ::Ice::Current()) {
		tReturnDate = trd;
	}

	bool hasTripReturnDate(const ::Ice::Current& = ::Ice::Current()) const {
		return tReturnDate;
	}

	vector<string> getTripWeekDays(const ::Ice::Current& =
			::Ice::Current()) const {
		return hasWeekDaysPeriodicity() ? tWeekDays.get() : vector<string>();
	}

	::cardroid::data::zerocice::Periodicity getTripPeriodicity(
			const ::Ice::Current& = ::Ice::Current()) const {
		return hasWeekDaysPeriodicity() ? tPeriodicity.get() :
				::cardroid::data::zerocice::Periodicity::EVERYWEEK;
	}

	void setTripWeekDaysPeriodicity(const vector<string>& twd,
			::cardroid::data::zerocice::Periodicity tp, const ::Ice::Current& =
					::Ice::Current()) {
		tWeekDays = twd;
		tPeriodicity = tp;
	}

	bool hasWeekDaysPeriodicity(const ::Ice::Current& =
			::Ice::Current()) const {
		return tWeekDays && tPeriodicity;
	}

	int getDistance(const ::Ice::Current& = ::Ice::Current()) const {
		return tripDistance ? tripDistance.get() : 0;
	}

	void setDistance(int d, const ::Ice::Current& = ::Ice::Current()) {
		tripDistance = d;
	}

	bool hasDistance(const ::Ice::Current& = ::Ice::Current()) const {
		return tripDistance;
	}

	string getCharacteristics(const ::Ice::Current& =
			::Ice::Current()) const {
		if (tripCharacteristics)
			return tripCharacteristics.get();
		else
			return "";
	}

	void setCharacteristics(const string& c, const ::Ice::Current& =
			::Ice::Current()) {
		tripCharacteristics = c;
	}

	bool hasCharacteristics(const ::Ice::Current& = ::Ice::Current()) const {
		return tripCharacteristics;
	}

	void setTripType(int t, const ::Ice::Current& = ::Ice::Current()) {
		tripType = t;
	}

	int getTripType(const ::Ice::Current& = ::Ice::Current()) const {
		return tripType;
	}

	string toString(const ::Ice::Current& = ::Ice::Current()) const {
		return fromPlace->getName() + " - " + toPlace->getName() + " ["
				+ tDate->toString() + "]";
	}

	/* Comparable<Trip> interface */

	bool operator==(const Trip& other) {
		return *(static_cast<Date*>(getTripDate().get()))
			 == *(static_cast<Date*>(const_cast<Trip&>(other).getTripDate().get()));
	}

	bool operator<(const Trip& other) {
		return *(static_cast<Date*>(getTripDate().get()))
			 < *(static_cast<Date*>(const_cast<Trip&>(other).getTripDate().get()));
	}

	bool operator>(const Trip& other) {
		return *(static_cast<Date*>(getTripDate().get()))
			 > *(static_cast<Date*>(const_cast<Trip&>(other).getTripDate().get()));
	}

	/* ObjectFactory interface */

	::Ice::ObjectPtr create(const string& type) {
		if (!type.compare(ice_staticId()))
			return new Trip();
		return NULL;
	}

	void destroy() {
	}

protected:

	::Ice::ObjectAdapterPtr _adapter;
	static const int TRIP_TYPE = 0, TRIP_OFFER_TYPE = 1, TRIP_REQUEST_TYPE = 2;
	static const long long int serialVersionUID = 1900796714995036513L;
	typedef ::cardroid::data::zerocice::TripTyp super;
};
}

namespace Oracle {

using namespace Cardroid::Data;

/**
 * \class Trip
 * Domain class implementing a Trip for its storage and retrieval from an
 * Oracle database
 */
class Trip: public ::TripTyp, public ITrip {

public:

	Trip() {
	}

	Trip(void* ctxOCCI_) :
			super(ctxOCCI_) {
	}

	/**
	 * Long constructor
	 */
	Trip(Place& fromPlace, Place& toPlace, Date datetime, int nSeats,
			Date returnDatetime, vector<string>& weekDays, string& periodicity,
			int distance, string characteristics) {
		setFrom_place(&fromPlace);
		setTo_place(&toPlace);
		setDatetime(&datetime);
		setN_seats(nSeats);
		setReturn_datetime(&returnDatetime);
		setWeek_days(weekDays);
		setPeriodicity(periodicity);
		setDistance(distance);
		setCharacteristics(characteristics);
	}

	/**
	 * Short constructor
	 */
	Trip(Place& fromPlace, Place& toPlace, Date datetime, int nSeats) {
		setFrom_place(&fromPlace);
		setTo_place(&toPlace);
		setDatetime(&datetime);
		setN_seats(nSeats);
	}

	/**
	 * Another constructor
	 */
	Trip(Place& fromPlace, Place& toPlace, Date datetime, int nSeats,
			Date returnDatetime) {
		setFrom_place(&fromPlace);
		setTo_place(&toPlace);
		setDatetime(&datetime);
		setN_seats(nSeats);
		setReturn_datetime(&returnDatetime);
	}

	/**
	 * Another constructor
	 */
	Trip(Place& fromPlace, Place& toPlace, Date datetime, int nSeats,
			vector<string>& weekDays, string& periodicity) {
		setFrom_place(&fromPlace);
		setTo_place(&toPlace);
		setDatetime(&datetime);
		setN_seats(nSeats);
		setWeek_days(weekDays);
		setPeriodicity(periodicity);
	}

	/**
	 * Another constructor
	 */
	Trip(Place& fromPlace, Place& toPlace, Date datetime, int nSeats,
			Date returnDatetime, vector<string>& weekDays,
			string& periodicity) {
		setFrom_place(&fromPlace);
		setTo_place(&toPlace);
		setDatetime(&datetime);
		setN_seats(nSeats);
		setReturn_datetime(&returnDatetime);
		setWeek_days(weekDays);
		setPeriodicity(periodicity);
	}

	Trip(const ::TripTyp& trip) {
		setTripId((int)(trip.getTrip_id()));
		setFrom_place(trip.getFrom_place());
		setTo_place(trip.getTo_place());
		setDatetime(trip.getDatetime());
		setN_seats(trip.getN_seats());
		if (trip.getReturn_datetime())
			setReturn_datetime(trip.getReturn_datetime());
		if (trip.getWeek_days().size() > 0 && trip.getPeriodicity().length() > 0) {
			setWeek_days(trip.getWeek_days());
			setPeriodicity(trip.getPeriodicity());
		}
		if (((int)trip.getDistance()) > 0)
			setDistance((int)(trip.getDistance()));
		if (trip.getCharacteristics().length() > 0)
			setCharacteristics(trip.getCharacteristics());
	}

	Trip(const Trip& t) {
		Trip& trip = const_cast<Trip&>(t);
		setTrip_id(trip.getTrip_id());
		setFrom_place(trip.getFrom_place());
		setTo_place(trip.getTo_place());
		setDatetime(trip.getDatetime());
		setN_seats(trip.getN_seats());
		if (trip.hasReturnDate())
			setReturn_datetime(trip.getReturn_datetime());
		if (trip.hasWeekDaysPeriodicity())
			setWeek_days(trip.getWeek_days());
		if (trip.hasDistance())
			setDistance(trip.getDistance());
		if (trip.hasCharacteristics())
			setCharacteristics(trip.getCharacteristics());
	}

	Trip(Trip&& trip) {
		setTrip_id(trip.getTrip_id());
		setFrom_place(trip.getFrom_place());
		setTo_place(trip.getTo_place());
		setDatetime(trip.getDatetime());
		setN_seats(trip.getN_seats());
		if (trip.hasReturnDate())
			setReturn_datetime(trip.getReturn_datetime());
		if (trip.hasWeekDaysPeriodicity())
			setWeek_days(trip.getWeek_days());
		if (trip.hasDistance())
			setDistance(trip.getDistance());
		if (trip.hasCharacteristics())
			setCharacteristics(trip.getCharacteristics());
	}

	~Trip() {
	}

	Trip& operator=(Trip trip) {
		setTrip_id(trip.getTrip_id());
		setFrom_place(trip.getFrom_place());
		setTo_place(trip.getTo_place());
		setDatetime(trip.getDatetime());
		setN_seats(trip.getN_seats());
		if (trip.hasReturnDate())
			setReturn_datetime(trip.getReturn_datetime());
		if (trip.hasWeekDaysPeriodicity())
			setWeek_days(trip.getWeek_days());
		if (trip.hasDistance())
			setDistance(trip.getDistance());
		if (trip.hasCharacteristics())
			setCharacteristics(trip.getCharacteristics());
		return *this;
	}

	Trip& operator=(Trip& trip) {
		setTrip_id(trip.getTrip_id());
		setFrom_place(trip.getFrom_place());
		setTo_place(trip.getTo_place());
		setDatetime(trip.getDatetime());
		setN_seats(trip.getN_seats());
		if (trip.hasReturnDate())
			setReturn_datetime(trip.getReturn_datetime());
		if (trip.hasWeekDaysPeriodicity())
			setWeek_days(trip.getWeek_days());
		if (trip.hasDistance())
			setDistance(trip.getDistance());
		if (trip.hasCharacteristics())
			setCharacteristics(trip.getCharacteristics());
		return *this;
	}

	/* ITrip interface */

	shared_ptr<ITrip> newInstance(const shared_ptr<ITrip> tripObject) {
		if (!tripObject)
			return NULL;
		if (dynamic_cast<Trip*>(tripObject.get()))
			return tripObject;

		shared_ptr<ITrip> trip = NULL;
		Place& from = *(static_pointer_cast<Place>(Place().newInstance(
				shared_ptr<IPlace>(&const_cast<IPlace&>(tripObject->getFromPlace())))).get());
		Place& to = *(static_pointer_cast<Place>(Place().newInstance(
				shared_ptr<IPlace>(&const_cast<IPlace&>(tripObject->getToPlace())))).get());
		Date datetime = *(static_pointer_cast<Date>(Date().newInstance(
				shared_ptr<IDate>(&tripObject->getDate()))).get());
		Date returnDatetime =
				tripObject->hasReturnDate() ?
						*(static_pointer_cast<Date>(Date().newInstance(
								shared_ptr<IDate>(&tripObject->getReturnDate()))).get()) :
						NULL;
		vector<string> weekDaysVarray;
		string periodicityName;
		if (tripObject->hasWeekDaysPeriodicity()) {
			array<char, 7> weekDays = tripObject->getWeekDays();
			for (auto n = weekDays.begin(); n != weekDays.end(); n++)
				weekDaysVarray.push_back(to_string(*n));
			switch (tripObject->getPeriodicity()) {
			case EVERYWEEK:
				periodicityName = "EVERYWEEK";
				break;
			case EVENWEEKS:
				periodicityName = "EVENWEEKS";
				break;
			case ODDWEEKS:
				periodicityName = "ODDWEEKS";
				break;
			}
		}

		if (tripObject->hasReturnDate() && tripObject->hasWeekDaysPeriodicity())
			trip = static_pointer_cast<ITrip>(make_shared<Trip>(from, to, datetime, tripObject->getNSeats(),
					returnDatetime, weekDaysVarray, periodicityName));
		if (!tripObject->hasReturnDate()
				&& !tripObject->hasWeekDaysPeriodicity())
			trip = static_pointer_cast<ITrip>(make_shared<Trip>(from, to, datetime, tripObject->getNSeats(),
					returnDatetime));
		if (tripObject->hasReturnDate()
				&& !tripObject->hasWeekDaysPeriodicity())
			trip = static_pointer_cast<ITrip>(make_shared<Trip>(from, to, datetime, tripObject->getNSeats(),
					returnDatetime));
		if (!tripObject->hasReturnDate()
				&& tripObject->hasWeekDaysPeriodicity())
			trip = static_pointer_cast<ITrip>(make_shared<Trip>(from, to, datetime, tripObject->getNSeats(),
					weekDaysVarray, periodicityName));

		trip->setTripId(tripObject->getTripId());
		if (tripObject->hasDistance())
			trip->setDistance(tripObject->getDistance());
		if (tripObject->hasCharacteristics())
			trip->setCharacteristics(tripObject->getCharacteristics());

		return trip;
	}

	void setTripId(int tripId) {
		try {
			super::setTrip_id(tripId);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	int getTripId() {
		int tripId = -1;
		try {
			tripId = super::getTrip_id();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return tripId;
	}

	void setFromPlace(const IPlace& fromPlace) {
		IPlace& from = const_cast<IPlace&>(fromPlace);
		try {
			super::setFrom_place(
					dynamic_cast<Place*>(&from) ?
							dynamic_cast<Place*>(&from) :
							static_pointer_cast<Place>(Place().newInstance(shared_ptr<IPlace>(&from))).get());
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	const IPlace& getFromPlace() {
		const Place& p = Place();
		try {
			const Place& place = *super::getFrom_place();
			return static_cast<const IPlace&>(place);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return p;
	}

	void setToPlace(const IPlace& toPlace) {
		IPlace& to = const_cast<IPlace&>(toPlace);
		try {
			super::setTo_place(
					dynamic_cast<Place*>(&to) ?
							dynamic_cast<Place*>(&to) :
							static_pointer_cast<Place>(Place().newInstance(
									shared_ptr<IPlace>(&to))).get());
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	const IPlace& getToPlace() {
		const Place& p = Place();
		try {
			const Place& place = *super::getTo_place();
			return static_cast<const IPlace&>(place);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return static_cast<const IPlace&>(p);
	}

	void setDate(IDate& datetime) {
		try {
			super::setDatetime(
					dynamic_cast<Date*>(&datetime) ?
							dynamic_cast<Date*>(&datetime) :
							static_pointer_cast<Date>(Date().newInstance(
									shared_ptr<IDate>(&datetime))).get());
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	IDate& getDate() {
		const Date& dt = Date();
		try {
			const Date& datetime = *super::getDatetime();
			return static_cast<IDate&>(const_cast<Date&>(datetime));
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return static_cast<IDate&>(const_cast<Date&>(dt));
	}

	void setNSeats(int nSeats) {
		try {
			super::setN_seats(nSeats);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	int getNSeats() {
		int nSeats = 0;
		try {
			nSeats = super::getN_seats();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return nSeats;
	}

	void setReturnDate(IDate& returnDatetime) {
		try {
			super::setReturn_datetime(
					dynamic_cast<Date*>(&returnDatetime) ?
							dynamic_cast<Date*>(&returnDatetime) :
							static_pointer_cast<Date>(Date().newInstance(
									shared_ptr<IDate>(&returnDatetime))).get());
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	IDate& getReturnDate() {
		const Date& rdt = Date();
		try {
			const Date& returnDateTime = *super::getReturn_datetime();
			return static_cast<IDate&>(const_cast<Date&>(returnDateTime));
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return static_cast<IDate&>(const_cast<Date&>(rdt));
	}

	bool hasReturnDate() {
		return getReturnDate().getTimeInMillis() > 0;
	}

	void setWeekDays(vector<string>& weekDaysVarray) {
		try {
			super::setWeek_days(weekDaysVarray);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	void setWeekDays(array<char, 7>& weekDays) {
		vector<string> weekDaysStr;
		for (auto n = weekDays.begin(); n != weekDays.end(); n++)
			weekDaysStr.push_back(to_string(*n));
		setWeekDays(weekDaysStr);
	}

	array<char, 7> getWeekDays() {
		const array<char, 7>& wdc = array<char, 7>();
		try {
			if (super::getWeek_days().size() == 7)
				return wdc;
			vector<string>& wd = super::getWeek_days();
			array<char, 7> weekDays = array<char, 7>();
			for (unsigned int n = 0; n < wd.size(); n++)
				weekDays[n] = wd[n].c_str()[0];
			const array<char, 7>& weekDaysChar = const_cast<const array<char, 7>&>(weekDays);
			return weekDaysChar;
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return wdc;
	}

	void setPeriodicity(ITrip::Periodicity periodicity) {
		string periodicityName;
		switch (periodicity) {
		case EVERYWEEK:
			periodicityName = "EVERYWEEK";
			break;
		case EVENWEEKS:
			periodicityName = "EVENWEEKS";
			break;
		case ODDWEEKS:
			periodicityName = "ODDWEEKS";
			break;
		}
		setPeriodicity(periodicityName);
	}

	void setPeriodicity(const string& periodicityName) {
		try {
			super::setPeriodicity(periodicityName);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	ITrip::Periodicity getPeriodicity() {
		ITrip::Periodicity periodicity = ITrip::Periodicity::EVERYWEEK;
		try {
			if (super::getPeriodicity().length() > 0)
				return periodicity;
			if (!super::getPeriodicity().compare("EVERYWEEK"))
				return periodicity = ITrip::Periodicity::EVERYWEEK;
			if (!super::getPeriodicity().compare("EVENWEEKS"))
				return periodicity = ITrip::Periodicity::EVENWEEKS;
			if (!super::getPeriodicity().compare("ODDWEEKS"))
				return periodicity = ITrip::Periodicity::ODDWEEKS;
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return periodicity;
	}

	void setWeekDaysPeriodicity(array<char, 7> weekDays,
			ITrip::Periodicity periodicity) {
		setWeekDays(weekDays);
		setPeriodicity(periodicity);
	}

	bool hasWeekDaysPeriodicity() {
		return getWeekDays().size() == 7 && getPeriodicity();
	}

	void setDistance(int distance) {
		try {
			super::setDistance(distance);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	int getDistance() {
		int distance = 0;
		try {
			distance = super::getDistance();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return distance;
	}

	bool hasDistance() {
		return getDistance() > 0;
	}

	void setCharacteristics(const string& characteristics) {
		try {
			super::setCharacteristics(characteristics);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	const string& getCharacteristics() {
		const string& c = "";
		try {
			const string& characteristics = super::getCharacteristics();
			return characteristics;
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return c;
	}

	bool hasCharacteristics() {
		return getCharacteristics().length() > 0;
	}

	void setTripType(int tripType) {
		try {
			super::setTrip_type(tripType);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	int getTripType() {
		int tripType = -1;
		try {
			tripType = super::getTrip_type();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return tripType;
	}

	/* Overriding superclass */

	::oracle::occi::Number getTrip_id() const {
		int tripId = -1;
		try {
			tripId = ((super*) this)->getTrip_id();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return tripId;
	}

	void setTrip_id(const oracle::occi::Number& value) {
		try {
			((super*) this)->setTrip_id(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	Place* getFrom_place() const {
		Place* from = NULL;
		try {
			from = ((super*) this)->getFrom_place();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return from;
	}

	void setFrom_place(Place* value) {
		try {
			((super*) this)->setFrom_place(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	Place* getTo_place() const {
		Place* to = NULL;
		try {
			to = ((super*) this)->getTo_place();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return to;
	}

	void setTo_place(Place* value) {
		try {
			((super*) this)->setTo_place(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	Date* getDatetime() const {
		Date* datetime = NULL;
		try {
			datetime = ((super*) this)->getDatetime();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return datetime;
	}

	void setDatetime(Date* value) {
		try {
			((super*) this)->setDatetime(dynamic_cast<Date*>(value));
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	oracle::occi::Number getN_seats() const {
		int nSeats = 0;
		try {
			nSeats = ((super*) this)->getN_seats();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return nSeats;
	}

	void setN_seats(const ::oracle::occi::Number& value) {
		try {
			((super*) this)->setN_seats(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	Date* getReturn_datetime() const {
		Date* rdatetime = NULL;
		try {
			rdatetime = ((super*) this)->getReturn_datetime();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return rdatetime;
	}

	void setReturn_datetime(Date* value) {
		try {
			((super*) this)->setReturn_datetime(dynamic_cast<Date*>(value));
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	static string getSqlTypeName() {
		return string("ANDROID.TRIP_TYP");
	}

	/* Comparable<Trip> interface */

	bool operator==(const Trip& other) {
		return *getDatetime()
				== *(const_cast<Trip&>(other).getDatetime());
	}

	bool operator<(const Trip& other) {
			return *getDatetime()
				< *(const_cast<Trip&>(other).getDatetime());
	}

	bool operator>(const Trip& other) {
			return *getDatetime()
				> *(const_cast<Trip&>(other).getDatetime());
	}

protected:

	typedef ::TripTyp super;
};
}
}
}

#endif /* ITRIP_H_ */
