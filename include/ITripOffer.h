#ifndef ITRIPOFFER_H_
#define ITRIPOFFER_H_

#include <array>
#include <vector>
#include <boost/lexical_cast.hpp>

#include "IDateTime.h"
#include "IPassenger.h"
#include "ITrip.h"
#include "IWaypoint.h"

using namespace std;

namespace Cardroid {
namespace Data {

/**
 * \interface ITripOffer
 * Public operations interface for the implementation of a TripOffer object
 */
class ITripOffer: public ITrip {

public:

	/**
	 * Create an instance of the class implementing this interface,
	 * from the received parameter, which also implements it.
	 * This method acts as an Abstract Factory, for the sake of the
	 * implementation of the Abstract Factory pattern in the search of
	 * interoperability among the different implementations of this interface
	 * which could exist in the different subsystems of the platform
	 * @param tripOfferObject An object instance implementing this interface
	 * @return An instance of the class implementing this interface, containing
	 * 			exactly the same data of the received bitmapObject parameter,
	 * 			from the viewpoint of the operations defined in this interface
	 */
	virtual shared_ptr<ITripOffer> newInstance(
			const shared_ptr<ITripOffer> tripOfferObject) = 0;

	/**
	 * @param datetime The new date and time for this ITripOffer
	 */
	virtual void setDateTime(IDateTime& datetime) = 0;

	/**
	 * @return The date and time for this ITripOffer
	 */
	virtual IDateTime& getDateTime() = 0;

	/**
	 * @param returnDatetime The new return date and time for this ITripOffer
	 */
	virtual void setReturnDateTime(IDateTime& returnDatetime) = 0;

	/**
	 * @return The return date and time for this ITripOffer
	 */
	virtual IDateTime& getReturnDateTime() = 0;

	/**
	 * @param driver The new driver for this ITripOffer
	 */
	virtual void setDriver(const IUser& driver) = 0;

	/**
	 * @return The driver for this ITripOffer
	 */
	virtual const IUser& getDriver() = 0;

	/**
	 * @param waypoints The new waypoint collection for this ITripOffer
	 */
	virtual void setWaypointsCollection(
			vector<IWaypoint*> waypoints) = 0;

	/**
	 * @return The waypoint collection for this ITripOffer
	 */
	virtual vector<IWaypoint*> getWaypointsCollection() = 0;

	/**
	 * @param waypoint Place to be added to this ITripOffer's
	 * 					waypoint collection
	 * @return			true
	 */
	virtual bool addWaypoint(const IPlace& waypoint) = 0;

	/**
	 * @param pos 	The position in the waypoint collection of the place to be
	 * 					removed
	 * @return		Whether the waypoint collection was modified
	 */
	virtual bool removeWaypoint(unsigned int pos) = 0;

	/**
	 * Remove every place in this ITripOffer's waypoint collection
	 */
	virtual void clearWaypoints() = 0;

	/**
	 * @return The number of places in this ITripOffer's waypoint collection
	 */
	virtual int getNWaypoints() = 0;

	/**
	 * @param car The new car for this ITripOffer
	 */
	virtual void setCar(const ICar& car) = 0;

	/**
	 * @return The car for this ITripOffer
	 */
	virtual const ICar& getCar() = 0;

	/**
	 * @param passengers The new passenger collection for this ITripOffer
	 */
	virtual void setPassengersCollection(
			vector<IPassenger*> passengers) = 0;

	/**
	 * @return The passenger collection for this ITripOffer
	 */
	virtual vector<IPassenger*> getPassengersCollection() = 0;

	/**
	 * @param passenger Passenger to be added to this ITripOffer 's passenger
	 * 						collection
	 * @return 			true
	 */
	virtual bool addPassenger(const IPassenger& passenger) = 0;

	/**
	 * @param passenger User to be removed from this ITripOffer 's passenger
	 * 						collection
	 * @return Whether the passenger collection was modified
	 */
	virtual bool removePassenger(IUser& passenger) = 0;

	/**
	 * Remove every passenger in this ITripOffer's passenger collection
	 */
	virtual void clearPassengers() = 0;

	/**
	 * @return The number of passengers in this ITripOffer's passenger collection
	 */
	virtual int getNPassengers() = 0;

	/**
	 * @param price The new price for this ITripOffer
	 */
	virtual void setPrice(double price) = 0;

	/**
	 * @return The price for this ITripOffer
	 */
	virtual double getPrice() = 0;

	/**
	 * @param allowed The new set of allowed features for this ITripOffer
	 */
	virtual void setAllowed(array<bool, 4> allowed) = 0;

	/**
	 * @return The set of allowed features for this ITripOffer
	 */
	virtual const array<bool, 4> getAllowed() = 0;
};

namespace Ice {

using namespace IceProxy;
using namespace Cardroid::Data;

/**
 * \class TripOffer
 * Domain class implementing a TripOffer for its transmission between systems
 * communicating across an Ice network infrastructure
 */
class TripOffer: public ::cardroid::data::zerocice::TripOfferTyp,
		public ITripOffer, public ::Ice::ObjectFactory {

public:

	TripOffer(::Ice::ObjectAdapterPtr adapter) {
		_adapter = adapter;
	}

	/**
	 * Short constructor
	 */
	TripOffer(::cardroid::data::zerocice::PlaceTyp* fp,
			::cardroid::data::zerocice::PlaceTyp* tp,
			::cardroid::data::zerocice::DateTimeTyp* dt, int ns,
			::cardroid::data::zerocice::UserTypPrx d,
			vector< ::cardroid::data::zerocice::WaypointTypPtr> wp,
			::cardroid::data::zerocice::CarTypPrx toc,
			vector< ::cardroid::data::zerocice::PassengerTypPtr> pass, double pr,
			vector<string> a) {
		tripId = -1;
		fromPlace = fp;
		toPlace = tp;
		tDate = dt;
		tDateTime = dt;
		nSeats = ns;
		tripType = TRIP_OFFER_TYPE;
		driver = d;
		waypoints = wp;
		tripOfferCar = toc;
		passengers = pass;
		price = pr;
		allowed = a;
	}

	TripOffer(::cardroid::data::zerocice::PlaceTyp* fp,
			::cardroid::data::zerocice::PlaceTyp* tp,
			::cardroid::data::zerocice::DateTimeTyp* dt, int ns,
			::cardroid::data::zerocice::DateTimeTyp* rdt, vector<string> wd,
			::cardroid::data::zerocice::Periodicity tper, int dist,
			const string& c, ::cardroid::data::zerocice::UserTypPrx drive,
			vector< ::cardroid::data::zerocice::WaypointTypPtr> wp,
			::cardroid::data::zerocice::CarTypPrx toc,
			vector< ::cardroid::data::zerocice::PassengerTypPtr> pass, double pr,
			vector<string> a) {
		tripId = -1;
		fromPlace = fp;
		toPlace = tp;
		tDate = dt;
		tDateTime = dt;
		nSeats = ns;
		tripType = TRIP_OFFER_TYPE;
		tReturnDate = rdt;
		tReturnDateTime = rdt;
		tWeekDays = wd;
		tPeriodicity = tper;
		tripDistance = dist;
		tripCharacteristics = c;
		driver = drive;
		waypoints = wp;
		tripOfferCar = toc;
		passengers = pass;
		price = pr;
		allowed = a;
	}

	TripOffer(int ti, ::cardroid::data::zerocice::PlaceTyp* fp,
			::cardroid::data::zerocice::PlaceTyp* tp,
			::cardroid::data::zerocice::DateTimeTyp* dt, int ns,
			::cardroid::data::zerocice::UserTypPrx drive,
			vector< ::cardroid::data::zerocice::WaypointTypPtr> wp,
			::cardroid::data::zerocice::CarTypPrx toc,
			vector< ::cardroid::data::zerocice::PassengerTypPtr> pass, double pr,
			vector<string> a) {
		tripId = ti;
		fromPlace = fp;
		toPlace = tp;
		tDate = dt;
		tDateTime = dt;
		nSeats = ns;
		tripType = TRIP_OFFER_TYPE;
		driver = drive;
		waypoints = wp;
		tripOfferCar = toc;
		passengers = pass;
		price = pr;
		allowed = a;
	}

	TripOffer(int ti, ::cardroid::data::zerocice::PlaceTyp* fp,
			::cardroid::data::zerocice::PlaceTyp* tp,
			::cardroid::data::zerocice::DateTimeTyp* dt, int ns,
			::cardroid::data::zerocice::DateTimeTyp* rdt, vector<string> wd,
			::cardroid::data::zerocice::Periodicity tper, int dist,
			const string& c, ::cardroid::data::zerocice::UserTypPrx drive,
			vector< ::cardroid::data::zerocice::WaypointTypPtr> wp,
			::cardroid::data::zerocice::CarTypPrx toc,
			vector< ::cardroid::data::zerocice::PassengerTypPtr> pass, double pr,
			vector<string> a) {
		tripId = ti;
		fromPlace = fp;
		toPlace = tp;
		tDate = dt;
		tDateTime = dt;
		nSeats = ns;
		tripType = TRIP_OFFER_TYPE;
		tReturnDate = rdt;
		tReturnDateTime = rdt;
		tWeekDays = wd;
		tPeriodicity = tper;
		tripDistance = dist;
		tripCharacteristics = c;
		driver = drive;
		waypoints = wp;
		tripOfferCar = toc;
		passengers = pass;
		price = pr;
		allowed = a;
	}

	TripOffer(const ::cardroid::data::zerocice::TripOfferTyp& tripOffer) {
		tripId = tripOffer.tripId;
		fromPlace = tripOffer.fromPlace;
		toPlace = tripOffer.toPlace;
		tDate = tripOffer.tDateTime;
		tDateTime = tripOffer.tDateTime;
		nSeats = tripOffer.nSeats;
		driver = tripOffer.driver;
		waypoints = tripOffer.waypoints;
		tripOfferCar = tripOffer.tripOfferCar;
		passengers = tripOffer.passengers;
		price = tripOffer.price;
		allowed = tripOffer.allowed;
		if (tripOffer.hasTripReturnDate()) {
			tReturnDateTime = tripOffer.tReturnDateTime;
			tReturnDate = tReturnDateTime.get();
		}
		if (tripOffer.hasWeekDaysPeriodicity()) {
			tWeekDays = tripOffer.tWeekDays;
			tPeriodicity = tripOffer.tPeriodicity;
		}
		if (tripOffer.hasDistance())
			tripDistance = tripOffer.tripDistance;
		if (tripOffer.hasCharacteristics())
			tripCharacteristics = tripOffer.tripCharacteristics;
	}

	TripOffer(const TripOffer& to) {
		TripOffer& tripOffer = const_cast<TripOffer&>(to);
		tripId = tripOffer.tripId;
		fromPlace = tripOffer.fromPlace;
		toPlace = tripOffer.toPlace;
		tDate = tripOffer.tDateTime;
		tDateTime = tripOffer.tDateTime;
		nSeats = tripOffer.nSeats;
		driver = tripOffer.driver;
		waypoints = tripOffer.waypoints;
		tripOfferCar = tripOffer.tripOfferCar;
		passengers = tripOffer.passengers;
		price = tripOffer.price;
		allowed = tripOffer.allowed;
		if (tripOffer.hasTripReturnDate()) {
					tReturnDateTime = tripOffer.tReturnDateTime;
					tReturnDate = tReturnDateTime.get();
		}
		if (((super&) tripOffer).hasWeekDaysPeriodicity()) {
			tWeekDays = tripOffer.tWeekDays;
			tPeriodicity = tripOffer.tPeriodicity;
		}
		if (((super&) tripOffer).hasDistance())
			tripDistance = tripOffer.tripDistance;
		if (((super&) tripOffer).hasCharacteristics())
			tripCharacteristics = tripOffer.tripCharacteristics;
	}

	TripOffer(TripOffer&& tripOffer) {
		swap(tripId, tripOffer.tripId);
		swap(fromPlace, tripOffer.fromPlace);
		swap(toPlace, tripOffer.toPlace);
		swap(tDateTime, tripOffer.tDateTime);
		swap(nSeats, tripOffer.nSeats);
		swap(driver, tripOffer.driver);
		swap(waypoints, tripOffer.waypoints);
		swap(tripOfferCar, tripOffer.tripOfferCar);
		swap(passengers, tripOffer.passengers);
		swap(price, tripOffer.price);
		swap(allowed, tripOffer.allowed);
		if (tripOffer.hasTripReturnDate())
			swap(tReturnDateTime, tripOffer.tReturnDateTime);
		if (((super&&) tripOffer).hasWeekDaysPeriodicity()) {
			swap(tWeekDays, tripOffer.tWeekDays);
			swap(tPeriodicity, tripOffer.tPeriodicity);
		}
		if (((super&&) tripOffer).hasDistance())
			swap(tripDistance, tripOffer.tripDistance);
		if (((super&&) tripOffer).hasCharacteristics())
			swap(tripCharacteristics, tripOffer.tripCharacteristics);
	}

	~TripOffer() {
		delete fromPlace._ptr;
		delete toPlace._ptr;
		delete tDate._ptr;
		delete tDateTime._ptr;
		delete &driver;
		delete &waypoints;
		delete &tripOfferCar;
		delete &passengers;
		delete &allowed;
		if (hasTripReturnDate()) {
			delete &tReturnDate;
			delete &tReturnDateTime;
		}
		if (((super*) this)->hasWeekDaysPeriodicity()) {
			delete &tWeekDays;
			delete &tPeriodicity;
		}
		if (((super*) this)->hasDistance())
			delete &tripDistance;
		if (((super*) this)->hasCharacteristics())
			delete &tripCharacteristics;
	}

	TripOffer& operator=(TripOffer tripOffer) {
		tripId = tripOffer.tripId;
		fromPlace = tripOffer.fromPlace;
		toPlace = tripOffer.toPlace;
		tDate = tripOffer.tDateTime;
		tDateTime = tripOffer.tDateTime;
		nSeats = tripOffer.nSeats;
		driver = tripOffer.driver;
		waypoints = tripOffer.waypoints;
		tripOfferCar = tripOffer.tripOfferCar;
		passengers = tripOffer.passengers;
		price = tripOffer.price;
		allowed = tripOffer.allowed;
		if (tripOffer.hasTripReturnDate()) {
					tReturnDateTime = tripOffer.tReturnDateTime;
					tReturnDate = tReturnDateTime.get();
		}
		if (((super&) tripOffer).hasWeekDaysPeriodicity()) {
			tWeekDays = tripOffer.tWeekDays;
			tPeriodicity = tripOffer.tPeriodicity;
		}
		if (((super&) tripOffer).hasDistance())
			tripDistance = tripOffer.tripDistance;
		if (((super&) tripOffer).hasCharacteristics())
			tripCharacteristics = tripOffer.tripCharacteristics;
		return *this;
	}

	TripOffer& operator=(TripOffer& tripOffer) {
		swap(tripId, tripOffer.tripId);
		swap(fromPlace, tripOffer.fromPlace);
		swap(toPlace, tripOffer.toPlace);
		swap(tDateTime, tripOffer.tDateTime);
		swap(nSeats, tripOffer.nSeats);
		swap(driver, tripOffer.driver);
		swap(waypoints, tripOffer.waypoints);
		swap(tripOfferCar, tripOffer.tripOfferCar);
		swap(passengers, tripOffer.passengers);
		swap(price, tripOffer.price);
		swap(allowed, tripOffer.allowed);
		if (tripOffer.hasTripReturnDate())
			swap(tReturnDateTime, tripOffer.tReturnDateTime);
		if (((super&) tripOffer).hasWeekDaysPeriodicity()) {
			swap(tWeekDays, tripOffer.tWeekDays);
			swap(tPeriodicity, tripOffer.tPeriodicity);
		}
		if (((super&) tripOffer).hasDistance())
			swap(tripDistance, tripOffer.tripDistance);
		if (((super&) tripOffer).hasCharacteristics())
			swap(tripCharacteristics, tripOffer.tripCharacteristics);
		return *this;
	}

	/**
	 * @return An Ice Identity for this datatype category and the data provided
	 */
	static ::Ice::Identity createIdentity(int tripId) {
		::Ice::Identity id;
		id.category = "trip_offer";
		id.name = to_string(tripId);
		return id;
	}

	/**
	 * @return A proxy to an Ice Object incarnating the provided data, whose
	 * 			servant is added to adapter 's active servant map
	 */
	::cardroid::data::zerocice::TripOfferTypPrx createProxy(
			::cardroid::data::zerocice::TripOfferTyp& tOfferObject,
			::Ice::ObjectAdapterPtr adapter) {
		::cardroid::data::zerocice::TripOfferTypPrx tOfferPrx =
				::cardroid::data::zerocice::TripOfferTypPrx::uncheckedCast(
						adapter->createProxy(
								createIdentity(tOfferObject.getTripId())));
		fillProxyData(tOfferObject, tOfferPrx, adapter);
		return tOfferPrx;
	}

	/**
	 * @return A proxy to an Ice Object incarnating the provided data, whose
	 * 			servant is added to adapter 's active servant map
	 */
	static ::cardroid::data::zerocice::TripOfferTypPrx createProxy(
			ITripOffer& tOfferObject, ::Ice::ObjectAdapterPtr adapter) {
		::cardroid::data::zerocice::TripOfferTypPrx tOfferPrx =
				::cardroid::data::zerocice::TripOfferTypPrx::uncheckedCast(
						adapter->createProxy(
								createIdentity(tOfferObject.getTripId())));
		fillProxyData(tOfferObject, tOfferPrx, adapter);
		return tOfferPrx;
	}

	/**
	 * Fill the servant referenced by the given proxy with the data contained
	 * in the given object
	 */
	static void fillProxyData(
			const ::cardroid::data::zerocice::TripOfferTyp& object,
			::cardroid::data::zerocice::TripOfferTypPrx proxy,
			::Ice::ObjectAdapterPtr adapter) {
		parent::fillProxyData(object, proxy, adapter);
		proxy->setTripDriver(object.getTripDriver());
		proxy->setTripWaypoints(object.getTripWaypoints());
		proxy->setTripCar(object.getTripCar());
		proxy->setTripPassengers(object.getTripPassengers());
		proxy->setPrice(object.getPrice());
		proxy->setAllowedFeatures(object.getAllowedFeatures());
		proxy->setTripDateTime(object.getTripDateTime());
		if (object.hasTripReturnDate())
			proxy->setTripReturnDateTime(object.getTripReturnDateTime());
	}

	/**
	 * Fill the servant referenced by the given proxy with the data contained
	 * in the given object
	 */
	static void fillProxyData(ITripOffer& object,
			::cardroid::data::zerocice::TripOfferTypPrx proxy,
			::Ice::ObjectAdapterPtr adapter) {
		parent::fillProxyData(object, proxy, adapter);
		proxy->setTripDriver(User::createProxy(object.getDriver(), adapter));
		vector<IWaypoint*> objectWaypoints =
				object.getWaypointsCollection();
		vector< ::cardroid::data::zerocice::WaypointTypPtr> proxyWaypoints;
		for (vector<IWaypoint*>::const_iterator wp =
				objectWaypoints.cbegin(); wp != objectWaypoints.cend(); wp++)
			proxyWaypoints.push_back(
					(Waypoint*) (Waypoint().newInstance(
							shared_ptr<IWaypoint>(*wp)).get()));
		proxy->setTripWaypoints(proxyWaypoints);
		proxy->setTripCar(
				Car::createProxy(
						const_cast<IUser&>(object.getDriver()).getEmail(),
						object.getCar(), adapter));
		vector<IPassenger*> objectPassengers =
				object.getPassengersCollection();
		vector< ::cardroid::data::zerocice::PassengerTypPtr> proxyPassengers;
		for (vector<IPassenger*>::const_iterator p =
				objectPassengers.cbegin(); p != objectPassengers.cend(); p++)
			proxyPassengers.push_back(
					(Passenger*) (Passenger(adapter).newInstance(
							shared_ptr<IPassenger>(*p)).get()));
		proxy->setTripPassengers(proxyPassengers);
		proxy->setPrice(object.getPrice());
		array<bool, 4> a = object.getAllowed();
		vector<string> allowed;
		for (auto n = a.begin(); n != a.end(); n++)
			allowed.push_back(to_string(*n));
		proxy->setAllowedFeatures(allowed);
		proxy->setTripDateTime(
				static_pointer_cast<DateTime>(DateTime().newInstance(
						shared_ptr<IDateTime>(&(object.getDateTime())))).get());
		if (object.hasReturnDate())
			proxy->setTripReturnDateTime(
					static_pointer_cast<DateTime>(DateTime().newInstance(
							shared_ptr<IDateTime>(&(object.getReturnDateTime())))).get());
	}

	/**
	 * @param proxy A proxy to a remote object implementing a TripOffer
	 * @return A local TripOffer object containing the data of the remote
	 * 			object referenced by proxy
	 */
	static shared_ptr<TripOffer> extractObject(
			::cardroid::data::zerocice::TripOfferTypPrx proxy) {
		shared_ptr<TripOffer> tripOffer;

		if (!(proxy->hasTripReturnDate() && proxy->hasWeekDaysPeriodicity()
				&& proxy->hasDistance() && proxy->hasCharacteristics())) {
			tripOffer = make_shared<TripOffer>(
					proxy->getTripId(), proxy->getPlace1().get(),
					proxy->getPlace2().get(), proxy->getTripDateTime().get(),
					proxy->getNSeats(), proxy->getTripDriver(),
					proxy->getTripWaypoints(), proxy->getTripCar(),
					proxy->getTripPassengers(), proxy->getPrice(),
					proxy->getAllowedFeatures());
			if (!proxy->hasTripReturnDate() && !proxy->hasWeekDaysPeriodicity()
					&& !proxy->hasDistance() && !proxy->hasCharacteristics())
				return tripOffer;
		}

		::cardroid::data::zerocice::DateTimeTyp* returnDateTime =
				proxy->getTripReturnDateTime().get();
		vector<string> weekDays = proxy->getTripWeekDays();
		::cardroid::data::zerocice::Periodicity periodicity =
				proxy->getTripPeriodicity();
		int distance = proxy->getDistance();
		string characteristics = proxy->getCharacteristics();

		if (!(proxy->hasTripReturnDate() && proxy->hasWeekDaysPeriodicity()
				&& proxy->hasDistance() && proxy->hasCharacteristics())) {
			if (proxy->hasTripReturnDate())
				tripOffer->setTripReturnDateTime(returnDateTime);
			if (proxy->hasWeekDaysPeriodicity())
				tripOffer->setTripWeekDaysPeriodicity(weekDays,
						periodicity);
			if (proxy->hasDistance())
				static_pointer_cast<super>(tripOffer)->setDistance(distance);
			if (proxy->hasCharacteristics())
				static_pointer_cast<super>(tripOffer)->setCharacteristics(characteristics);
		} else
			tripOffer = make_shared<TripOffer>(
					proxy->getTripId(), proxy->getPlace1().get(),
					proxy->getPlace2().get(), proxy->getTripDateTime().get(),
					proxy->getNSeats(), returnDateTime, weekDays, periodicity,
					distance, characteristics, proxy->getTripDriver(),
					proxy->getTripWaypoints(), proxy->getTripCar(),
					proxy->getTripPassengers(), proxy->getPrice(),
					proxy->getAllowedFeatures());

		return tripOffer;
	}

	/* ITripOffer interface */

	shared_ptr<ITrip> newInstance(const shared_ptr<ITrip> tripObject) {
		return Trip().newInstance(tripObject);
	}

	shared_ptr<ITripOffer> newInstance(
			const shared_ptr<ITripOffer> tripOfferObject) {
		if (!tripOfferObject)
			return NULL;
		if (dynamic_cast<TripOffer*>(tripOfferObject.get()))
			return tripOfferObject;

		shared_ptr<TripOffer> tripOffer;
		Place* fromPlace = static_pointer_cast<Place>(Place().newInstance(
				shared_ptr<IPlace>(&const_cast<IPlace&>(tripOfferObject->getFromPlace())))).get();
		Place* toPlace = static_pointer_cast<Place>(Place().newInstance(
				shared_ptr<IPlace>(&const_cast<IPlace&>(tripOfferObject->getToPlace())))).get();
		DateTime* datetime = static_pointer_cast<DateTime>(DateTime().newInstance(
				shared_ptr<IDateTime>(&(tripOfferObject->getDateTime())))).get();
		::cardroid::data::zerocice::UserTypPrx driver = User::createProxy(
				tripOfferObject->getDriver(), _adapter);
		vector<IWaypoint*> wpc = tripOfferObject->getWaypointsCollection();
		vector< ::cardroid::data::zerocice::WaypointTypPtr> waypoints =
				vector< ::cardroid::data::zerocice::WaypointTypPtr>();
		for (vector<IWaypoint*>::iterator i = wpc.begin();
				i != wpc.end(); i++)
			waypoints.push_back(static_pointer_cast<Waypoint>(
					Waypoint().newInstance(
							shared_ptr<IWaypoint>(*i))).get());
		::cardroid::data::zerocice::CarTypPrx car = Car::createProxy(
				const_cast<IUser&>(tripOfferObject->getDriver()).getEmail(),
				tripOfferObject->getCar(), _adapter);
		vector<IPassenger*> pc =
				tripOfferObject->getPassengersCollection();
		vector< ::cardroid::data::zerocice::PassengerTypPtr> passengers =
				vector< ::cardroid::data::zerocice::PassengerTypPtr>();
		Passenger passenger(_adapter);
		for (vector<IPassenger*>::iterator i = pc.begin(); i != pc.end();
				i++)
			passengers.push_back(static_pointer_cast<Passenger>(
					passenger.newInstance(shared_ptr<IPassenger>(*i))).get());
		double price = tripOfferObject->getPrice();
		array<bool, 4> a = tripOfferObject->getAllowed();
		vector<string> allowed;
		for (auto n = a.begin(); n != a.end(); n++)
			allowed.push_back(to_string(*n));
		if (!(tripOfferObject->hasReturnDate()
				&& tripOfferObject->hasWeekDaysPeriodicity()
				&& tripOfferObject->hasDistance()
				&& tripOfferObject->hasCharacteristics())) {
			tripOffer = make_shared<TripOffer>(
					tripOfferObject->getTripId(), fromPlace,
					toPlace, datetime, tripOfferObject->getNSeats(), driver,
					waypoints, car, passengers, price, allowed);
			if (!tripOfferObject->hasReturnDate()
					&& !tripOfferObject->hasWeekDaysPeriodicity()
					&& !tripOfferObject->hasDistance()
					&& !tripOfferObject->hasCharacteristics())
				return static_pointer_cast<ITripOffer>(tripOffer);
		}

		DateTime* returnDatetime;
		if (tripOfferObject->hasReturnDate())
			returnDatetime = static_pointer_cast<DateTime>(
					DateTime().newInstance(shared_ptr<IDateTime>(
							&(tripOfferObject->getReturnDateTime())))).get();
		vector<string> weekDays = vector<string>();
		::cardroid::data::zerocice::Periodicity periodicity;
		if (tripOfferObject->hasWeekDaysPeriodicity()) {
			array<char, 7> wd = tripOfferObject->getWeekDays();
			for (auto n = wd.begin(); n != wd.end(); n++)
				weekDays.push_back(to_string(*n));
			periodicity =
					static_cast< ::cardroid::data::zerocice::Periodicity>(tripOfferObject->getPeriodicity());
		}
		int distance = tripOfferObject->getDistance();
		const string& characteristics = tripOfferObject->getCharacteristics();

		if (!(tripOfferObject->hasReturnDate()
				&& tripOfferObject->hasWeekDaysPeriodicity()
				&& tripOfferObject->hasDistance()
				&& tripOfferObject->hasCharacteristics())) {
			if (tripOfferObject->hasReturnDate())
				tripOffer->setTripReturnDateTime(returnDatetime);
			if (tripOfferObject->hasWeekDaysPeriodicity())
				tripOffer->setTripWeekDaysPeriodicity(weekDays, periodicity);
			if (tripOfferObject->hasDistance())
				static_pointer_cast<super>(tripOffer)->setDistance(distance);
			if (tripOfferObject->hasCharacteristics())
				static_pointer_cast<super>(tripOffer)->setCharacteristics(characteristics);
		} else
			tripOffer = make_shared<TripOffer>(
					tripOfferObject->getTripId(), fromPlace,
					toPlace, datetime, tripOfferObject->getNSeats(),
					returnDatetime, weekDays, periodicity, distance,
					characteristics, driver, waypoints, car, passengers, price,
					allowed);

		return static_pointer_cast<ITripOffer>(tripOffer);
	}

	void setTripId(int ti) {
		tripId = ti;
	}

	int getTripId() {
		return tripId;
	}

	void setFromPlace(const IPlace& from) {
		fromPlace = static_pointer_cast<Place>(
				Place().newInstance(shared_ptr<IPlace>(
						&const_cast<IPlace&>(from)))).get();
	}

	const IPlace& getFromPlace() {
		return *static_cast<Place*>(fromPlace.get());
	}

	void setToPlace(const IPlace& to) {
		toPlace = static_pointer_cast<Place>(
				Place().newInstance(shared_ptr<IPlace>(
						&const_cast<IPlace&>(to)))).get();
	}

	const IPlace& getToPlace() {
		return *static_cast<Place*>(toPlace.get());
	}

	void setDate(IDate& datetime) {
		tDate = static_pointer_cast<Date>(
				Date().newInstance(shared_ptr<IDate>(&datetime))).get();
	}

	IDate& getDate() {
		return *static_cast<DateTime*>(tDateTime.get());
	}

	void setNSeats(int ns) {
		nSeats = ns;
	}

	int getNSeats() {
		return nSeats;
	}

	void setReturnDate(IDate& returnDatetime) {
		tReturnDate = static_pointer_cast<Date>(
				Date().newInstance(shared_ptr<IDate>(&returnDatetime))).get();
	}

	IDate& getReturnDate() {
		return getReturnDateTime();
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
		vector<string> weekDays = tWeekDays.get();
		array<char,7> wd = array<char, 7>();
		if (hasWeekDaysPeriodicity())
			for (unsigned int n = 0; n < weekDays.size(); n++)
				wd[n] = weekDays[n].c_str()[0];
		return wd;
	}

	ITrip::Periodicity getPeriodicity() {
		return hasWeekDaysPeriodicity() ?
				static_cast<ITrip::Periodicity>(tPeriodicity.get()) :
				ITrip::Periodicity::EVERYWEEK;
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

	void setCharacteristics(const string& c) {
		tripCharacteristics = c;
	}

	const string& getCharacteristics() {
		return tripCharacteristics.get();
	}

	bool hasCharacteristics() {
		return tripCharacteristics;
	}

	void setTripType(int tt) {
		tripType = TRIP_OFFER_TYPE;
	}

	int getTripType() {
		return TRIP_OFFER_TYPE;
	}

	void setDateTime(IDateTime& datetime) {
		DateTime* dt = static_pointer_cast<DateTime>(
				DateTime().newInstance(shared_ptr<IDateTime>(&datetime))).get();
		tDate = dt;
		tDateTime = dt;
	}

	IDateTime& getDateTime() {
		return *static_cast<DateTime*>(tDateTime.get());
	}

	void setReturnDateTime(IDateTime& returnDateTime) {
		DateTime* rdt = static_pointer_cast<DateTime>(DateTime().newInstance(
				shared_ptr<IDateTime>(&returnDateTime))).get();
		tReturnDateTime = rdt;
		tReturnDate = rdt;
	}

	IDateTime& getReturnDateTime() {
		const IDateTime& rdt = tReturnDateTime ?
				*static_cast<DateTime*>(tReturnDateTime.get().get()) : DateTime();
		return const_cast<IDateTime&>(rdt);
	}

	void setDriver(const IUser& d) {
		driver = User::createProxy(d, _adapter);
	}

	const IUser& getDriver() {
		return *(User::extractObject(driver).get());
	}

	void setWaypointsCollection(vector<IWaypoint*> waypoints) {
		vector< ::cardroid::data::zerocice::WaypointTypPtr> wp =
				vector< ::cardroid::data::zerocice::WaypointTypPtr>();
		for (vector<IWaypoint*>::iterator i = waypoints.begin();
				i != waypoints.end(); i++)
			wp.push_back(
					static_pointer_cast<Waypoint>(Waypoint().newInstance(
							shared_ptr<IWaypoint>(*i))).get());
		setTripWaypoints(wp);
	}

	vector<IWaypoint*> getWaypointsCollection() {
		vector< ::cardroid::data::zerocice::WaypointTypPtr> waypoints =
				getTripWaypoints();
		vector<IWaypoint*> wp = vector<IWaypoint*>();
		for (::cardroid::data::zerocice::WaypointCollection::iterator i =
				waypoints.begin(); i != waypoints.end(); i++)
			wp.push_back(static_cast<Waypoint*>((*i).get()));
		return wp;
	}

	bool addWaypoint(const IPlace& waypoint) {
		return addTripWaypoint(static_pointer_cast<Place>(
				Place().newInstance(shared_ptr<IPlace>(
						&const_cast<IPlace&>(waypoint)))).get());
	}

	bool removeWaypoint(unsigned int pos) {
		return removeTripWaypoint(pos);
	}

	void clearWaypoints() {
		waypoints.clear();
	}

	int getNWaypoints() {
		return waypoints.size();
	}

	void setCar(const ICar& car) {
		tripOfferCar = Car::createProxy(driver->getEmail(), car, _adapter);
	}

	const ICar& getCar() {
		return *(Car::extractObject(tripOfferCar).get());
	}

	void setPassengersCollection(vector<IPassenger*> passenger) {
		vector< ::cardroid::data::zerocice::PassengerTypPtr> p;
		for (vector<IPassenger*>::iterator i = passenger.begin();
				i != passenger.end(); i++)
			p.push_back(
					(Passenger*) (Passenger(_adapter).newInstance(
							shared_ptr<IPassenger>(*i)).get()));
		passengers = p;
	}

	vector<IPassenger*> getPassengersCollection() {
		vector<IPassenger*> p = vector<IPassenger*>();
		for (::cardroid::data::zerocice::PassengerCollection::iterator i =
				passengers.begin(); i != passengers.end(); i++)
			p.push_back(static_cast<Passenger*>((*i).get()));
		return p;
	}

	bool addPassenger(const IPassenger& p) {
		IPassenger& passenger = const_cast<IPassenger&>(p);
		return addTripPassenger(
				User::createProxy(passenger.getUserPassenger(), _adapter),
				passenger.getNSeats());
	}

	bool removePassenger(IUser& passenger) {
		return removeTripPassenger(User::createProxy(passenger, _adapter));
	}

	void clearPassengers() {
		passengers.clear();
	}

	int getNPassengers() {
		return getNTripPassengers();
	}

	void setPrice(double p) {
		price = p;
	}

	double getPrice() {
		return price;
	}

	void setAllowed(array<bool, 4> allow) {
		vector<string> a;
		for (auto i = allow.begin(); i != allow.end(); i++)
			a.push_back(to_string(*i));
		allowed = a;
	}

	const array<bool, 4> getAllowed() {
		array<bool, 4> a = array<bool, 4>();
		for (unsigned int i = 0; i < allowed.size(); i++)
			a[i] = ::boost::lexical_cast<bool, string>(allowed[i]);
		return a;
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

	int getTripType(const ::Ice::Current& = ::Ice::Current()) const {
		return TRIP_OFFER_TYPE;
	}

	void setTripType(int tt, const ::Ice::Current& = ::Ice::Current()) {
		tripType = TRIP_OFFER_TYPE;
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

	void setTripDate(const ::cardroid::data::zerocice::DateTypPtr& date,
			const ::Ice::Current& = ::Ice::Current()) {
		tDate = date;
	}

	::cardroid::data::zerocice::DateTypPtr getTripDate(const ::Ice::Current& =
			::Ice::Current()) const {
		return tDate;
	}

	int getNSeats(const ::Ice::Current& = ::Ice::Current()) const {
		return nSeats;
	}

	void setNSeats(int ns, const ::Ice::Current& = ::Ice::Current()) {
		nSeats = ns;
	}

	void setTripReturnDate(
			const ::cardroid::data::zerocice::DateTypPtr& returnDate,
			const ::Ice::Current& = ::Ice::Current()) {
		tReturnDate = returnDate;
	}

	::cardroid::data::zerocice::DateTypPtr getTripReturnDate(
			const ::Ice::Current& = ::Ice::Current()) const {
		return static_cast< ::cardroid::data::zerocice::DateTyp*>(
						tReturnDateTime ? tReturnDateTime.get().get() : new DateTime());
	}

	bool hasTripReturnDate(const ::Ice::Current& = ::Ice::Current()) const {
		return tReturnDateTime;
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
		return tripCharacteristics ? tripCharacteristics.get() : "";
	}

	void setCharacteristics(const string& c, const ::Ice::Current& =
			::Ice::Current()) {
		tripCharacteristics = c;
	}

	bool hasCharacteristics(const ::Ice::Current& = ::Ice::Current()) const {
		return tripCharacteristics;
	}

	::cardroid::data::zerocice::DateTimeTypPtr getTripDateTime(
			const ::Ice::Current& = ::Ice::Current()) const {
		return tDateTime;
	}

	void setTripDateTime(const ::cardroid::data::zerocice::DateTimeTypPtr& td,
			const ::Ice::Current& = ::Ice::Current()) {
		tDate = td;
		tDateTime = td;
	}

	void setTripReturnDateTime(
			const ::cardroid::data::zerocice::DateTimeTypPtr& returnDatetime,
			const ::Ice::Current& = ::Ice::Current()) {
		tReturnDate = returnDatetime;
		tReturnDateTime = returnDatetime;
	}

	::cardroid::data::zerocice::DateTimeTypPtr getTripReturnDateTime(
			const ::Ice::Current& = ::Ice::Current()) const {
		return tReturnDateTime ? tReturnDateTime.get() : new DateTime();
	}

	::cardroid::data::zerocice::UserTypPrx getTripDriver(const ::Ice::Current& =
			::Ice::Current()) const {
		return driver;
	}

	void setTripDriver(const ::cardroid::data::zerocice::UserTypPrx& d,
			const ::Ice::Current& = ::Ice::Current()) {
		driver = d;
	}

	::cardroid::data::zerocice::WaypointCollection getTripWaypoints(
			const ::Ice::Current& = ::Ice::Current()) const {
		return waypoints;
	}

	void setTripWaypoints(
			const ::cardroid::data::zerocice::WaypointCollection& wp,
			const ::Ice::Current& = ::Ice::Current()) {
		waypoints = wp;
	}

	::cardroid::data::zerocice::CarTypPrx getTripCar(const ::Ice::Current& =
			::Ice::Current()) const {
		return tripOfferCar;
	}

	void setTripCar(const ::cardroid::data::zerocice::CarTypPrx& car,
			const ::Ice::Current& = ::Ice::Current()) {
		tripOfferCar = car;
	}

	::cardroid::data::zerocice::PassengerCollection getTripPassengers(
			const ::Ice::Current& = ::Ice::Current()) const {
		return passengers;
	}

	void setTripPassengers(
			const ::cardroid::data::zerocice::PassengerCollection& p,
			const ::Ice::Current& = ::Ice::Current()) {
		passengers = p;
	}

	double getPrice(const ::Ice::Current& = ::Ice::Current()) const {
		return price;
	}

	void setPrice(double p, const ::Ice::Current& = ::Ice::Current()) {
		price = p;
	}

	::cardroid::data::zerocice::AllowedV getAllowedFeatures(
			const ::Ice::Current& = ::Ice::Current()) const {
		return allowed;
	}

	void setAllowedFeatures(const ::cardroid::data::zerocice::AllowedV& a,
			const ::Ice::Current& = ::Ice::Current()) {
		allowed = a;
	}

	bool addTripWaypoint(
			const ::cardroid::data::zerocice::PlaceTypPtr& waypoint,
			const ::Ice::Current& = ::Ice::Current()) {
		for (::cardroid::data::zerocice::WaypointCollection::iterator n =
				waypoints.begin(); n != waypoints.end(); n++) {
			::cardroid::data::zerocice::PlaceTypPtr p =
					(*n)->getWaypointPlace();
			if (p->getCoords().latitude == waypoint->getCoords().latitude
					&& p->getCoords().longitude
							== waypoint->getCoords().longitude
					&& !p->getName().compare(waypoint->getName()))
				return false;
		}

		waypoints.push_back(new Waypoint(waypoints.size(), waypoint));
		return true;
	}

	bool removeTripWaypoint(int pos, const ::Ice::Current& = ::Ice::Current()) {
		if ((unsigned int)pos >= waypoints.size())
			return false;
		waypoints.erase(waypoints.begin() + pos);
		return true;
	}

	int getNTripWaypoints(const ::Ice::Current& = ::Ice::Current()) {
		return waypoints.size();
	}

	void clearTripWaypoints(const ::Ice::Current& = ::Ice::Current()) {
		waypoints.clear();
	}

	bool addTripPassenger(
			const ::cardroid::data::zerocice::UserTypPrx& passenger, int seats,
			const ::Ice::Current& = ::Ice::Current()) {
		if (seats > nSeats)
			return false;
		for (::cardroid::data::zerocice::PassengerCollection::iterator n =
				passengers.begin(); n != passengers.end(); n++)
			if (!(*n)->getPassengerUser()->getEmail().compare(
					passenger->getEmail()))
				return false;

		passengers.push_back(new Passenger(passenger, seats));
		return true;
	}

	bool removeTripPassenger(
			const ::cardroid::data::zerocice::UserTypPrx& passenger,
			const ::Ice::Current& = ::Ice::Current()) {
		bool remove = false;
		for (::cardroid::data::zerocice::PassengerCollection::iterator p =
				passengers.begin(); p != passengers.end(); p++) {
			if (!(*p)->getPassengerUser()->getEmail().compare(
					passenger->getEmail())) {
				passengers.erase(p);
				remove = true;
			}
		}
		return remove;
	}

	int getNTripPassengers(const ::Ice::Current& = ::Ice::Current()) {
		int count = 0;
		for (::cardroid::data::zerocice::PassengerCollection::iterator n =
				passengers.begin(); n != passengers.end(); n++)
			count += (*n)->getNSeats();
		return count;
	}

	void clearTripPassengers(const ::Ice::Current& = ::Ice::Current()) {
		passengers.clear();
	}

	string toString(const ::Ice::Current& = ::Ice::Current()) const {
		return fromPlace->getName() + " - " + toPlace->getName() + " ["
				+ tDate->toString() + "]";
	}

	/* Comparable<TripOffer> interface */

	bool operator==(const TripOffer& other) {
		return *(static_cast<DateTime*>(getTripDateTime().get())) ==
			*(static_cast<DateTime*>(const_cast<TripOffer&>(other).getTripDateTime().get()));
	}

	bool operator<(const TripOffer& other) {
		return *(static_cast<DateTime*>(getTripDateTime().get())) <
			*(static_cast<DateTime*>(const_cast<TripOffer&>(other).getTripDateTime().get()));
	}

	bool operator>(const TripOffer& other) {
		return *(static_cast<DateTime*>(getTripDateTime().get())) >
			*(static_cast<DateTime*>(const_cast<TripOffer&>(other).getTripDateTime().get()));
	}

	/* ObjectFactory interface */

	::Ice::ObjectPtr create(const string& type) {
		if (!type.compare(ice_staticId()))
			return new TripOffer(_adapter);
		return NULL;
	}

	void destroy() {
	}

private:

	::Ice::ObjectAdapterPtr _adapter;
	static const int TRIP_TYPE = 0, TRIP_OFFER_TYPE = 1, TRIP_REQUEST_TYPE = 2;
	static const long long int serialVersionUID = 6393694705313903949L;
	typedef ::cardroid::data::zerocice::TripOfferTyp super;
	typedef Trip parent;
};
}

namespace Oracle {

using namespace Cardroid::Data;

/**
 * \class TripOffer
 * Domain class implementing a TripOffer for its storage and retrieval from an
 * Oracle database
 */
class TripOffer: public ::TripOfferTyp, public ITripOffer {

public:

	TripOffer() {
	}

	TripOffer(void* ctxOCCI_) :
			super(ctxOCCI_) {
	}

	/**
	 * Short constructor
	 */
	TripOffer(Place& fromPlace, Place& toPlace, DateTime datetime, User& driver,
			Car& car, int availableSeats, double price, vector<string>& allow) {
		setFromPlace(fromPlace);
		setToPlace(toPlace);
		setDateTime(datetime);
		setDriver(driver);
		setCar(car);
		setNSeats(availableSeats);
		setPrice(price);
		setAllowed(allow);
	}

	/**
	 * Another constructor
	 */
	TripOffer(Place& from, Place& to, DateTime datetime,
			DateTime returnDatetime, User& driver, Car& car, int availableSeats,
			double price, vector<string>& allow) {
		setFromPlace(from);
		setToPlace(to);
		setDateTime(datetime);
		setReturnDateTime (returnDatetime);
		setDriver(driver);
		setCar(car);
		setNSeats(availableSeats);
		setPrice(price);
		setAllowed(allow);
	}

	/**
	 * Another constructor
	 */
	TripOffer(Place& from, Place& to, DateTime datetime, User& driver, Car& car,
			int availableSeats, double price, vector<string>& allow,
			vector<string>& weekDays, const string& periodicity) {
		setFromPlace(from);
		setToPlace(to);
		setDateTime(datetime);
		setDriver(driver);
		setCar(car);
		setNSeats(availableSeats);
		setPrice(price);
		setAllowed(allow);
		setWeekDaysPeriodicity(weekDays, periodicity);
	}

	/**
	 * Another constructor
	 */
	TripOffer(Place& from, Place& to, DateTime datetime,
			DateTime returnDatetime, User& driver, Car& car, int availableSeats,
			double price, vector<string>& allow, vector<string>& weekDays,
			const string& periodicity) {
		setFromPlace(from);
		setToPlace(to);
		setDateTime(datetime);
		setReturnDateTime(returnDatetime);
		setDriver(driver);
		setCar(car);
		setNSeats(availableSeats);
		setPrice(price);
		setAllowed(allow);
		setWeekDaysPeriodicity(weekDays, periodicity);
	}

	/**
	 * Long constructor
	 */
	TripOffer(Place& fromPlace, Place& toPlace, DateTime datetime, int nSeats,
			DateTime returnDatetime, vector<string>& weekDays,
			const string& periodicity, int distance,
			const string& characteristics, User& driver,
			vector<Waypoint*> waypoints, Car& car,
			vector<Passenger*> passengers, double price,
			vector<string>& allowed) {
		setFromPlace(fromPlace);
		setToPlace(toPlace);
		setDateTime(datetime);
		setNSeats(nSeats);
		setReturnDateTime(returnDatetime);
		setWeekDaysPeriodicity(weekDays, periodicity);
		setDistance(distance);
		setCharacteristics(characteristics);
		setDriver(driver);
		setWaypointsCollection(waypoints);
		setCar(car);
		setPassengersCollection(passengers);
		setPrice(price);
		setAllowed(allowed);
	}

	TripOffer(const ::TripOfferTyp& tripOffer) {
		setTrip_id(tripOffer.getTrip_id());
		setFrom_place(tripOffer.getFrom_place());
		setTo_place(tripOffer.getTo_place());
		setDatetime(dynamic_cast<DateTime*>(tripOffer.getDatetime()));
		setN_seats(tripOffer.getN_seats());
		setDriver(tripOffer.getDriver());
		setWaypoints(tripOffer.getWaypoints());
		setCar(tripOffer.getCar());
		setPassengers(tripOffer.getPassengers());
		setPrice(tripOffer.getPrice());
		setAllowed(tripOffer.getAllowed());
		if (tripOffer.getReturn_datetime())
			setReturn_datetime(dynamic_cast<DateTime*>(tripOffer.getReturn_datetime()));
		if (tripOffer.getWeek_days().size() > 0)
			setWeek_days(tripOffer.getWeek_days());
		if ((int)(tripOffer.getDistance()) > 0)
			setDistance((int)(tripOffer.getDistance()));
		if (tripOffer.getCharacteristics().length() > 0)
			setCharacteristics(tripOffer.getCharacteristics());
	}

	TripOffer(const TripOffer& to) {
		TripOffer& tripOffer = const_cast<TripOffer&>(to);
		setTrip_id(tripOffer.getTrip_id());
		setFrom_place(tripOffer.getFrom_place());
		setTo_place(tripOffer.getTo_place());
		setDatetime(tripOffer.getDatetime());
		setN_seats(tripOffer.getN_seats());
		setDriver(tripOffer.getDriver());
		setWaypoints(tripOffer.getWaypoints());
		setCar(tripOffer.getCar());
		setPassengers(tripOffer.getPassengers());
		setPrice(tripOffer.getPrice());
		setAllowed(((super&)tripOffer).getAllowed());
		if (tripOffer.hasReturnDate())
			setReturn_datetime(tripOffer.getReturn_datetime());
		if (tripOffer.hasWeekDaysPeriodicity())
			setWeek_days(tripOffer.getWeek_days());
		if (tripOffer.hasDistance())
			setDistance((int)(tripOffer.getDistance()));
		if (tripOffer.hasCharacteristics())
			setCharacteristics(tripOffer.getCharacteristics());
	}

	TripOffer(TripOffer&& tripOffer) {
		setTrip_id(tripOffer.getTrip_id());
		setFrom_place(tripOffer.getFrom_place());
		setTo_place(tripOffer.getTo_place());
		setDatetime(tripOffer.getDatetime());
		setN_seats(tripOffer.getN_seats());
		setDriver(tripOffer.getDriver());
		setWaypoints(tripOffer.getWaypoints());
		setCar(tripOffer.getCar());
		setPassengers(tripOffer.getPassengers());
		setPrice(tripOffer.getPrice());
		setAllowed(((super&&)tripOffer).getAllowed());
		if (tripOffer.hasReturnDate())
			setReturn_datetime(tripOffer.getReturn_datetime());
		if (tripOffer.hasWeekDaysPeriodicity())
			setWeek_days(tripOffer.getWeek_days());
		if (tripOffer.hasDistance())
			setDistance((int)(tripOffer.getDistance()));
		if (tripOffer.hasCharacteristics())
			setCharacteristics(tripOffer.getCharacteristics());
	}

	~TripOffer() {
	}

	TripOffer& operator=(TripOffer& tripOffer) {
		setTrip_id(tripOffer.getTrip_id());
		setFrom_place(tripOffer.getFrom_place());
		setTo_place(tripOffer.getTo_place());
		setDatetime(tripOffer.getDatetime());
		setN_seats(tripOffer.getN_seats());
		setDriver(tripOffer.getDriver());
		setWaypoints(tripOffer.getWaypoints());
		setCar(tripOffer.getCar());
		setPassengers(tripOffer.getPassengers());
		setPrice(tripOffer.getPrice());
		setAllowed(((super&)tripOffer).getAllowed());
		if (tripOffer.hasReturnDate())
			setReturn_datetime(tripOffer.getReturn_datetime());
		if (tripOffer.hasWeekDaysPeriodicity())
			setWeek_days(tripOffer.getWeek_days());
		if (tripOffer.hasDistance())
			setDistance((int)(tripOffer.getDistance()));
		if (tripOffer.hasCharacteristics())
			setCharacteristics(tripOffer.getCharacteristics());
		return *this;
	}

	/* ITrip interface */

	shared_ptr<ITrip> newInstance(const shared_ptr<ITrip> tripObject) {
		return ((parent*) this)->newInstance(tripObject);
	}

	void setTripId(int tripId) {
		((parent*) this)->setTripId(tripId);
	}

	int getTripId() {
		return ((parent*) this)->getTripId();
	}

	void setFromPlace(const IPlace& from) {
		((parent*) this)->setFromPlace(from);
	}

	const IPlace& getFromPlace() {
		return ((parent*) this)->getFromPlace();
	}

	void setToPlace(const IPlace& to) {
		((parent*) this)->setToPlace(to);
	}

	const IPlace& getToPlace() {
		return ((parent*) this)->getToPlace();
	}

	void setDate(IDate& date) {
		((parent*) this)->setDate(date);
	}

	IDate& getDate() {
		return ((parent*) this)->getDate();
	}

	void setNSeats(int nSeats) {
		((parent*) this)->setNSeats(nSeats);
	}

	int getNSeats() {
		return ((Trip) this).getNSeats();
	}

	void setReturnDate(IDate& returnDate) {
		((Trip) this).setReturnDate(returnDate);
	}

	IDate& getReturnDate() {
		return ((ITrip*) this)->getReturnDate();
	}

	bool hasReturnDate() {
		return ((ITrip*) this)->hasReturnDate();
	}

	void setWeekDaysPeriodicity(array<char, 7> weekDays,
			Periodicity periodicity) {
		((Trip) this).setWeekDaysPeriodicity(weekDays, periodicity);
	}

	array<char, 7> getWeekDays() {
		return ((Trip) this).getWeekDays();
	}

	Periodicity getPeriodicity() {
		return ((Trip) this).getPeriodicity();
	}

	bool hasWeekDaysPeriodicity() {
		return ((Trip) this).hasWeekDaysPeriodicity();
	}

	void setDistance(int distance) {
		((Trip) this).setDistance(distance);
	}

	int getDistance() {
		return ((Trip) this).getDistance();
	}

	bool hasDistance() {
		return ((Trip) this).hasDistance();
	}

	void setCharacteristics(const string& characteristics) {
		((Trip) this).setCharacteristics(characteristics);
	}

	const string& getCharacteristics() {
		return ((Trip) this).getCharacteristics();
	}

	bool hasCharacteristics() {
		return ((Trip) this).hasCharacteristics();
	}

	void setTripType(int tripType) {
		((Trip) this).setTripType(TRIP_OFFER_TYPE);
	}

	int getTripType() {
		return TRIP_OFFER_TYPE;
	}

	/* ITripOffer interface */

	shared_ptr<ITripOffer> newInstance(
			const shared_ptr<ITripOffer> tripOfferObject) {
		if (!tripOfferObject)
			return NULL;
		if (dynamic_cast<TripOffer*>(tripOfferObject.get()))
			return tripOfferObject;

		shared_ptr<TripOffer> tOffer;
		Place from = *(static_pointer_cast<Place>(Place().newInstance(
				shared_ptr<IPlace>(&const_cast<IPlace&>(tripOfferObject->getFromPlace())))).get());
		Place to = *(static_pointer_cast<Place>(Place().newInstance(
				shared_ptr<IPlace>(&const_cast<IPlace&>(tripOfferObject->getToPlace())))).get());
		DateTime datetime = *(static_pointer_cast<DateTime>(DateTime().newInstance(
				shared_ptr<IDateTime>(&(tripOfferObject->getDateTime())))).get());
		vector<string> weekDaysVarray = vector<string>();
		if (tripOfferObject->hasWeekDaysPeriodicity()) {
			array<char, 7> weekDays = tripOfferObject->getWeekDays();
			for (auto n = weekDays.begin(); n != weekDays.end(); n++)
				weekDaysVarray.push_back(to_string(*n));
		}
		User driver = *(static_pointer_cast<User>(User().newInstance(
				shared_ptr<IUser>(&const_cast<IUser&>(tripOfferObject->getDriver())))).get());
		vector<IWaypoint*> wpc =
				tripOfferObject->getWaypointsCollection();
		vector<Waypoint*> waypointsCollection = vector<Waypoint*>();
		for (vector<IWaypoint*>::iterator i = wpc.begin(); i != wpc.end(); i++)
			waypointsCollection.push_back(static_pointer_cast<Waypoint>(
					Waypoint().newInstance(shared_ptr<IWaypoint>(*i))).get());
		Car car = *(static_pointer_cast<Car>(Car().newInstance(
				shared_ptr<ICar>(&const_cast<ICar&>(tripOfferObject->getCar())))).get());
		vector<IPassenger*> passengersCollection =
				tripOfferObject->getPassengersCollection();
		array<bool, 4> allowed = tripOfferObject->getAllowed();
		vector<string> allowedVarray;
		for (auto n = allowed.cbegin(); n != allowed.cend(); n++)
			allowedVarray.push_back(to_string(*n));
		DateTime returnDatetime =
				tripOfferObject->hasReturnDate() ?
						*(static_pointer_cast<DateTime>(DateTime().newInstance(
								shared_ptr<IDateTime>(
										&(tripOfferObject->getDateTime())))).get()) :
						DateTime();
		::cardroid::data::zerocice::Periodicity periodicity = ::cardroid::data::zerocice::Periodicity::EVERYWEEK;
		string periodicityName;
		if (tripOfferObject->hasWeekDaysPeriodicity()) {
			array<char, 7> wd = tripOfferObject->getWeekDays();
			vector<string> weekDays = vector<string>();
			for (auto n = wd.begin(); n != wd.end(); n++)
				weekDays.push_back(to_string(*n).substr(0, 1));
			periodicity =
					static_cast< ::cardroid::data::zerocice::Periodicity>(tripOfferObject->getPeriodicity());
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
		}

		if (tripOfferObject->hasReturnDate()
				&& tripOfferObject->hasWeekDaysPeriodicity())
			tOffer = make_shared<TripOffer>(
					from, to, datetime, returnDatetime, driver,
					car, tripOfferObject->getNSeats(),
					tripOfferObject->getPrice(), allowedVarray, weekDaysVarray,
					periodicityName);
		if (!tripOfferObject->hasReturnDate()
				&& !tripOfferObject->hasWeekDaysPeriodicity())
			tOffer = make_shared<TripOffer>(
					from, to, datetime, driver, car,
					tripOfferObject->getNSeats(), tripOfferObject->getPrice(),
					allowedVarray);
		if (tripOfferObject->hasReturnDate()
				&& !tripOfferObject->hasWeekDaysPeriodicity())
			tOffer = make_shared<TripOffer>(
					from, to, datetime, returnDatetime, driver,
					car, tripOfferObject->getNSeats(),
					tripOfferObject->getPrice(), allowedVarray);
		if (!tripOfferObject->hasReturnDate()
				&& tripOfferObject->hasWeekDaysPeriodicity())
			tOffer = make_shared<TripOffer>(
					from, to, datetime, driver, car,
					tripOfferObject->getNSeats(), tripOfferObject->getPrice(),
					allowedVarray, weekDaysVarray, periodicityName);

		tOffer->setTripId(tripOfferObject->getTripId());
		tOffer->setWaypointsCollection(waypointsCollection);
		tOffer->setPassengersCollection(passengersCollection);
		if (tripOfferObject->hasDistance())
			tOffer->setDistance(tripOfferObject->getDistance());
		if (tripOfferObject->hasCharacteristics())
			tOffer->setCharacteristics(tripOfferObject->getCharacteristics());

		return static_pointer_cast<ITripOffer>(tOffer);
	}

	void setDateTime(IDateTime& dateTime) {
		((parent) this).setDate(dateTime);
	}

	IDateTime& getDateTime() {
		return dynamic_cast<IDateTime&>(((parent) this).getDate());
	}

	void setReturnDateTime(IDateTime& returnDateTime) {
		((parent) this).setReturnDate(returnDateTime);
	}

	IDateTime& getReturnDateTime() {
		return *getReturn_datetime();
	}

	void setDriver(const IUser& driver) {
		try {
			super::setDriver(static_pointer_cast<User>(
					User().newInstance(shared_ptr<IUser>(&const_cast<IUser&>(driver)))).get());
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	const IUser& getDriver() {
		const User& d = User();
		try {
			const User& driver = *(super::getDriver());
			return driver;
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return d;
	}

	void setWaypointsCollection(vector<IWaypoint*> waypoints) {
		vector<Waypoint*> wp = vector<Waypoint*>();
		for (vector<IWaypoint*>::const_iterator i = waypoints.cbegin();
				i != waypoints.cend(); i++)
			wp.push_back(
					static_pointer_cast<Waypoint>(Waypoint().newInstance(
							shared_ptr<IWaypoint>(*i))).get());
		try {
			super::setWaypoints(wp);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	vector<IWaypoint*> getWaypointsCollection() {
		vector<IWaypoint*> wp = vector<IWaypoint*>();
		try {
			vector<Waypoint*> waypoints = super::getWaypoints();
			for (vector<Waypoint*>::const_iterator i = waypoints.cbegin();
					i != waypoints.cend(); i++)
				wp.push_back(static_cast<IWaypoint*>(*i));
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return wp;
	}

	bool addWaypoint(const IPlace& wp) {
		IPlace& waypoint = const_cast<IPlace&>(wp);
		bool pushback = false;
		try {
			vector<Waypoint*>& waypoints = super::getWaypoints();
			for (vector<Waypoint*>::iterator i = waypoints.begin();
					i != waypoints.end(); i++) {
				if (!(*i)->getName().compare(waypoint.getName())
						&& (*i)->getCoordinates()->getLatitude()
								== waypoint.getCoordinates()->getLatitude()
						&& (*i)->getCoordinates()->getLongitude()
								== waypoint.getCoordinates()->getLongitude())
					return pushback;
				waypoints.push_back(
						new Waypoint(waypoints.size(),
								static_pointer_cast<Place>(Place().newInstance(
										shared_ptr<IPlace>(&waypoint))).get()));
				super::setWaypoints(waypoints);
			}
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return pushback;
	}

	bool removeWaypoint(unsigned int pos) {
		bool erase = false;
		try {
			vector<Waypoint*>& waypoints = super::getWaypoints();
			if (pos >= waypoints.size())
				return erase;
			waypoints.erase(waypoints.begin() + pos);
			super::setWaypoints(waypoints);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return erase;
	}

	void clearWaypoints() {
		try {
			vector<Waypoint*>& waypoints = super::getWaypoints();
			waypoints.clear();
			super::setWaypoints(waypoints);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	int getNWaypoints() {
		int nWaypoints = 0;
		try {
			nWaypoints = ((vector<Waypoint*> ) (super::getWaypoints())).size();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return nWaypoints;
	}

	void setCar(const ICar& car) {
		try {
			super::setCar(
					static_pointer_cast<Car>(Car().newInstance(
							shared_ptr<ICar>(&const_cast<ICar&>(car)))).get());
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	const ICar& getCar() {
		const Car& c = Car();
		try {
			const Car& car = *(super::getCar());
			return car;
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return c;
	}

	void setPassengersCollection(vector<IPassenger*> passengers) {
		vector<Passenger*> p = vector<Passenger*>();
		for (vector<IPassenger*>::const_iterator i = passengers.cbegin();
				i != passengers.cend(); i++)
			p.push_back(
					static_pointer_cast<Passenger>(Passenger().newInstance(
							shared_ptr<IPassenger>(*i))).get());
		try {
			super::setPassengers(p);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	vector<IPassenger*> getPassengersCollection() {
		vector<IPassenger*> p = vector<IPassenger*>();
		try {
			vector<Passenger*> passengers = super::getPassengers();
			for (vector<Passenger*>::const_iterator i = passengers.cbegin();
					i != passengers.cend(); i++)
				p.push_back(static_cast<IPassenger*>(*i));
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return p;
	}

	bool addPassenger(const IPassenger& p) {
		IPassenger& passenger = const_cast<IPassenger&>(p);
		bool pushback = false;
		if (passenger.getNSeats() > getNSeats())
			return pushback;
		try {
			pushback = true;
			vector<Passenger*>& passengers = super::getPassengers();
			for (vector<Passenger*>::iterator i = passengers.begin();
					i != passengers.end(); i++)
				if (!(*i)->getEmail().compare(passenger.getEmail()))
					pushback = false;
			if (pushback) {
				passengers.push_back(
						static_pointer_cast<Passenger>(Passenger().newInstance(
								shared_ptr<IPassenger>(&passenger))).get());
				super::setPassengers(passengers);
				setNSeats(getNSeats() - passenger.getNSeats());
			}
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return pushback;
	}

	bool removePassenger(IUser& passenger) {
		bool erase = false;
		try {
			vector<Passenger*>& passengers = super::getPassengers();
			for (vector<Passenger*>::iterator i = passengers.begin();
					i != passengers.end(); i++) {
				if (!(*i)->getEmail().compare(passenger.getEmail())) {
					passengers.erase(i);
					erase = true;
				}
			}
			if (erase)
				super::setPassengers(passengers);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return erase;
	}

	void clearPassengers() {
		try {
			vector<Passenger*>& passengers = super::getPassengers();
			passengers.clear();
			super::setPassengers(passengers);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	int getNPassengers() {
		int nPassengers = 0;
		try {
			nPassengers = ((vector<Passenger*> ) super::getPassengers()).size();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return nPassengers;
	}

	void setPrice(double price) {
		try {
			super::setPrice(price);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	double getPrice() {
		double price = 0.;
		try {
			price = super::getPrice();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return price;
	}

	void setAllowed(array<bool, 4> allowed) {
		vector<string> a;
		for (auto i = allowed.begin(); i != allowed.end(); i++)
			a.push_back(to_string(*i));
		try {
			super::setAllowed(a);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	const array<bool, 4> getAllowed() {
		array<bool, 4> a = array<bool, 4>();
		try {
			vector<string> allowed = super::getAllowed();
			for (unsigned int n = 0; n < allowed.size(); n++)
				a[n] = ::boost::lexical_cast<bool, string>(allowed[n]);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return a;
	}

	/* Superclass methods */

	User* getDriver() const {
		User* driver = NULL;
		try {
			driver = super::getDriver();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return driver;
	}

	void setDriver(User* value) {
		try {
			super::setDriver(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	vector<Waypoint*> getWaypointsCollection() const {
		vector<Waypoint*> waypoints = vector<Waypoint*>();
		try {
			waypoints = super::getWaypoints();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return waypoints;
	}

	void setWaypointsCollection(const vector<Waypoint*>& value) {
		try {
			super::setWaypoints(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	Car* getCar() const {
		Car* car = NULL;
		try {
			car = super::getCar();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return car;
	}

	void setCar(Car* value) {
		try {
			super::setCar(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	vector<Passenger*> getPassengersCollection() const {
		vector<Passenger*> passengers = vector<Passenger*>();
		try {
			passengers = super::getPassengers();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return passengers;
	}

	void setPassengersCollection(const vector<Passenger*>& value) {
		try {
			super::setPassengers(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	::oracle::occi::Number getPrice() const {
		double price = 0.;
		try {
			price = super::getPrice();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return price;
	}

	void setPrice(const ::oracle::occi::Number& value) {
		try {
			super::setPrice((double) value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	const vector<string>& getAllowed() const {
		const vector<string>& a = vector<string>();
		try {
			const vector<string>& allowed = super::getAllowed();
			return allowed;
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return a;
	}

	void setAllowed(vector<string>& value) {
		try {
			super::setAllowed(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	void setWeekDaysPeriodicity(vector<string>& weekDaysVarray,
			const string& periodicityName) {
		try {
			((Trip) this).setWeekDays(weekDaysVarray);
			((Trip) this).setPeriodicity(periodicityName);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
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

	DateTime* getDatetime() const {
		DateTime* datetime = NULL;
		try {
			datetime = (DateTime*) ((super*) this)->getDatetime();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return datetime;
	}

	void setDatetime(DateTime* value) {
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

	DateTime* getReturn_datetime() const {
		DateTime* rdatetime = NULL;
		try {
			rdatetime = (DateTime*) ((super*) this)->getReturn_datetime();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return rdatetime;
	}

	void setReturn_datetime(DateTime* value) {
		try {
			((super*) this)->setReturn_datetime(dynamic_cast<Date*>(value));
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	vector<string>& getWeek_days() const {
		const vector<string>& wd = vector<string>();
		try {
			const vector<string>& weekDays = ((super*) this)->getWeek_days();
			return const_cast<vector<string>&>(weekDays);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return const_cast<vector<string>&>(wd);
	}

	void setWeek_days(const vector<string>& value) {
		try {
			((super*) this)->setWeek_days(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	string getPeriodicity() const {
		string periodicity = "";
		try {
			periodicity = ((super*) this)->getPeriodicity();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return periodicity;
	}

	void setPeriodicity(const string& value) {
		try {
			((super*) this)->setPeriodicity(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	::oracle::occi::Number getDistance() const {
		int distance = 0;
		try {
			distance = ((super*) this)->getDistance();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return distance;
	}

	void setDistance(const ::oracle::occi::Number& value) {
		try {
			((super*) this)->setDistance(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	string getCharacteristics() const {
		string characteristics = "";
		try {
			characteristics = ((super*) this)->getCharacteristics();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return characteristics;
	}

	::oracle::occi::Number getTrip_type() const {
		return TRIP_OFFER_TYPE;
	}

	void setTrip_type(const ::oracle::occi::Number& value) {
		try {
			((super*) this)->setTrip_type(TRIP_OFFER_TYPE);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	vector<Waypoint*>& getWaypoints() {
		const vector<Waypoint*>& wp = vector<Waypoint*>();
		try {
			const vector<Waypoint*>& waypoints = super::getWaypoints();
			return const_cast<vector<Waypoint*>&>(waypoints);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return const_cast<vector<Waypoint*>&>(wp);
	}

	const vector<Waypoint*>& getWaypoints() const {
		const vector<Waypoint*>& wp = vector<Waypoint*>();
		try {
			const vector<Waypoint*>& waypoints = super::getWaypoints();
			return waypoints;
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return wp;
	}

	void setWaypoints(const vector<Waypoint*>& value) {
		try {
			super::setWaypoints(value);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	vector<Passenger*>& getPassengers() {
		const vector<Passenger*>& p = vector<Passenger*>();
		try {
			const vector<Passenger*>& passengers = super::getPassengers();
			return const_cast<vector<Passenger*>&>(passengers);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return const_cast<vector<Passenger*>&>(p);
	}

	const vector<Passenger*>& getPassengers() const {
		const vector<Passenger*>& p = vector<Passenger*>();
		try {
			const vector<Passenger*>& passengers = super::getPassengers();
			return passengers;
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return p;
	}

	void setPassengers(const vector<Passenger*>& value) {
		try {
			super::setPassengers(value);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	void setAllowed(const vector<string>& value) {
		try {
			super::setAllowed(value);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	static string getSqlTypeName() {
		return string("ANDROID.TRIP_OFFER_TYP");
	}

	/* Comparable<TripOffer> interface */

	bool operator==(const TripOffer& other) {
		return *getDatetime() ==
				*(const_cast<TripOffer&>(other).getDatetime());
	}

	bool operator<(const TripOffer& other) {
		return *getDatetime() <
				*(const_cast<TripOffer&>(other).getDatetime());
	}

	bool operator>(const TripOffer& other) {
		return *getDatetime() >
				*(const_cast<TripOffer&>(other).getDatetime());
	}

private:
	static const int TRIP_TYPE = 0, TRIP_OFFER_TYPE = 1, TRIP_REQUEST_TYPE = 2;
	typedef ::TripOfferTyp super;
	typedef Trip parent;
};
}
}
}

#endif /* ITRIPOFFER_H_ */
