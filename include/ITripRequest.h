#ifndef ITRIPREQUEST_H_
#define ITRIPREQUEST_H_

#include "IDateTimePrefs.h"
#include "ITrip.h"
#include "IUser.h"

namespace Cardroid {
namespace Data {

/**
 * \interface ITripRequest
 * Public operations interface for the implementation of a TripRequest object
 */
class ITripRequest: public ITrip {

public:

	/**
	 * Create an instance of the class implementing this interface,
	 * from the received parameter, which also implements it.
	 * This method acts as an Abstract Factory, for the sake of the
	 * implementation of the Abstract Factory pattern in the search of
	 * interoperability among the different implementations of this interface
	 * which could exist in the different subsystems of the platform
	 * @param tripRequestObject An object instance implementing this interface
	 * @return An instance of the class implementing this interface, containing
	 * 			exactly the same data of the received bitmapObject parameter,
	 * 			from the viewpoint of the operations defined in this interface
	 */
	virtual shared_ptr<ITripRequest> newInstance(
			const shared_ptr<ITripRequest> tripRequestObject) = 0;

	/**
	 * @param requester The new requester for this ITripRequest
	 */
	virtual void setRequester(const IUser& requester) = 0;

	/**
	 * @return The request for this ITripRequest
	 */
	virtual const IUser& getRequester() = 0;

	/**
	 * @param dateTimePrefsObject The new date and time preferences for this
	 * 								ITripRequest
	 */
	virtual void setDateTime(IDateTimePrefs& dateTimePrefsObject) = 0;

	/**
	 * @return the date and time preferences for this ITripRequest
	 */
	virtual IDateTimePrefs& getDateTime() = 0;

	/**
	 * @param dateTimePrefsObject The new return date and time preferences for
	 * 								this ITripRequest
	 */
	virtual void setReturnDateTime(IDateTimePrefs& dateTimePrefsObject) = 0;

	/**
	 * @return the return date and time preferences for this ITripRequest
	 */
	virtual IDateTimePrefs& getReturnDateTime() = 0;
};

namespace Ice {

using namespace IceProxy;
using namespace Cardroid::Data;

/**
 * \class TripRequest
 * Domain class implementing a TripRequest for its transmission between systems
 * communicating across an Ice network infrastructure
 */
class TripRequest: public ::cardroid::data::zerocice::TripRequestTyp,
		public ITripRequest, public ::Ice::ObjectFactory {

public:

	TripRequest(::Ice::ObjectAdapterPtr adapter) {
		_adapter = adapter;
	}

	/**
	 * Short constructor
	 */
	TripRequest(::cardroid::data::zerocice::PlaceTyp* fp,
			::cardroid::data::zerocice::PlaceTyp* tp,
			::cardroid::data::zerocice::DateTimePrefsTyp* dtp, int ns,
			::cardroid::data::zerocice::UserTypPrx r) {
		tripId = -1;
		fromPlace = fp;
		toPlace = tp;
		tDate = dtp;
		tDateTimePrefs = dtp;
		nSeats = ns;
		tripType = TRIP_REQUEST_TYPE;
		requester = r;
	}

	TripRequest(::cardroid::data::zerocice::PlaceTyp* fp,
			::cardroid::data::zerocice::PlaceTyp* tp,
			::cardroid::data::zerocice::DateTimePrefsTyp* dtp, int ns,
			::cardroid::data::zerocice::DateTimePrefsTyp* rdtp,
			vector<string> wd, ::cardroid::data::zerocice::Periodicity p, int d,
			const string& c, ::cardroid::data::zerocice::UserTypPrx r) {
		tripId = -1;
		fromPlace = fp;
		toPlace = tp;
		tDate = dtp;
		tDateTimePrefs = dtp;
		nSeats = ns;
		tripType = TRIP_REQUEST_TYPE;
		tReturnDate = rdtp;
		tReturnDateTimePrefs = rdtp;
		tWeekDays = wd;
		tPeriodicity = p;
		tripDistance = d;
		tripCharacteristics = c;
		requester = r;
	}

	TripRequest(int ti, ::cardroid::data::zerocice::PlaceTyp* fp,
			::cardroid::data::zerocice::PlaceTyp* tp,
			::cardroid::data::zerocice::DateTimePrefsTyp* dtp, int ns,
			::cardroid::data::zerocice::UserTypPrx r) {
		tripId = ti;
		fromPlace = fp;
		toPlace = tp;
		tDate = dtp;
		tDateTimePrefs = dtp;
		nSeats = ns;
		tripType = TRIP_REQUEST_TYPE;
		requester = r;
	}

	TripRequest(int ti, ::cardroid::data::zerocice::PlaceTyp* fp,
			::cardroid::data::zerocice::PlaceTyp* tp,
			::cardroid::data::zerocice::DateTimePrefsTyp* dtp, int ns,
			::cardroid::data::zerocice::DateTimePrefsTyp* rdtp,
			vector<string> wd, ::cardroid::data::zerocice::Periodicity p, int d,
			const string& c, ::cardroid::data::zerocice::UserTypPrx r) {
		tripId = ti;
		fromPlace = fp;
		toPlace = tp;
		tDate = dtp;
		tDateTimePrefs = dtp;
		nSeats = ns;
		tReturnDate = rdtp;
		tReturnDateTimePrefs = rdtp;
		tWeekDays = wd;
		tPeriodicity = p;
		tripDistance = d;
		tripCharacteristics = c;
		tripType = TRIP_REQUEST_TYPE;
		requester = r;
	}

	TripRequest(const ::cardroid::data::zerocice::TripRequestTyp& tripRequest) {
		tripId = tripRequest.tripId;
		fromPlace = tripRequest.fromPlace;
		toPlace = tripRequest.toPlace;
		tDate = tripRequest.tDateTimePrefs;
		tDateTimePrefs = tripRequest.tDateTimePrefs;
		nSeats = tripRequest.nSeats;
		tripType = tripRequest.tripType;
		requester = tripRequest.requester;
		if (tripRequest.hasTripReturnDate()) {
			tReturnDateTimePrefs =
					tripRequest.tReturnDateTimePrefs;
			tReturnDate = static_cast< ::cardroid::data::zerocice::DateTimePrefsTyp*>(
					tripRequest.tReturnDateTimePrefs.get().get());
		}
		if (tripRequest.hasWeekDaysPeriodicity()) {
			tWeekDays = tripRequest.tWeekDays;
			tPeriodicity = tripRequest.tPeriodicity;
		}
		if (tripRequest.hasDistance())
			tripDistance = tripRequest.tripDistance;
		if (tripRequest.hasCharacteristics())
			tripCharacteristics = tripRequest.tripCharacteristics;
	}

	TripRequest(const TripRequest& tr) {
		TripRequest& tripRequest = const_cast<TripRequest&>(tr);
		tripId = tripRequest.tripId;
		fromPlace = tripRequest.fromPlace;
		toPlace = tripRequest.toPlace;
		tDate = tripRequest.tDateTimePrefs;
		tDateTimePrefs = tripRequest.tDateTimePrefs;
		nSeats = tripRequest.nSeats;
		tripType = tripRequest.tripType;
		requester = tripRequest.requester;
		if (tripRequest.hasTripReturnDate()) {
			tReturnDateTimePrefs =
					tripRequest.tReturnDateTimePrefs;
			tReturnDate = static_cast< ::cardroid::data::zerocice::DateTimePrefsTyp*>(
					tripRequest.tReturnDateTimePrefs.get().get());
		}
		if (((super&) tripRequest).hasWeekDaysPeriodicity()) {
			tWeekDays = tripRequest.tWeekDays;
			tPeriodicity = tripRequest.tPeriodicity;
		}
		if (((super&) tripRequest).hasDistance())
			tripDistance = tripRequest.tripDistance;
		if (((super&) tripRequest).hasCharacteristics())
			tripCharacteristics = tripRequest.tripCharacteristics;
	}

	TripRequest(TripRequest&& tripRequest) {
		swap(tripId, tripRequest.tripId);
		swap(fromPlace, tripRequest.fromPlace);
		swap(toPlace, tripRequest.toPlace);
		swap(tDateTimePrefs, tripRequest.tDateTimePrefs);
		swap(nSeats, tripRequest.nSeats);
		swap(tripType, tripRequest.tripType);
		swap(requester, tripRequest.requester);
		if (tripRequest.hasTripReturnDate())
			swap(tReturnDateTimePrefs, tripRequest.tReturnDateTimePrefs);
		if (((super&&) tripRequest).hasWeekDaysPeriodicity()) {
			swap(tWeekDays, tripRequest.tWeekDays);
			swap(tPeriodicity, tripRequest.tPeriodicity);
		}
		if (((super&&) tripRequest).hasDistance())
			swap(tripDistance, tripRequest.tripDistance);
		if (((super&&) tripRequest).hasCharacteristics())
			swap(tripCharacteristics, tripRequest.tripCharacteristics);
	}

	~TripRequest() {
		delete fromPlace._ptr;
		delete toPlace._ptr;
		delete tDate._ptr;
		delete tDateTimePrefs._ptr;
		delete &requester;
		if (hasTripReturnDate()) {
			delete &tReturnDate;
			delete &tReturnDateTimePrefs;
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

	TripRequest& operator=(TripRequest tripRequest) {
		tripId = tripRequest.tripId;
		fromPlace = tripRequest.fromPlace;
		toPlace = tripRequest.toPlace;
		tDate = tripRequest.tDateTimePrefs;
		tDateTimePrefs = tripRequest.tDateTimePrefs;
		nSeats = tripRequest.nSeats;
		tripType = tripRequest.tripType;
		requester = tripRequest.requester;
		if (tripRequest.hasTripReturnDate()) {
			tReturnDateTimePrefs =
					tripRequest.tReturnDateTimePrefs;
			tReturnDate = static_cast< ::cardroid::data::zerocice::DateTimePrefsTyp*>(
					tripRequest.tReturnDateTimePrefs.get().get());
		}
		if (((super&) tripRequest).hasWeekDaysPeriodicity()) {
			tWeekDays = tripRequest.tWeekDays;
			tPeriodicity = tripRequest.tPeriodicity;
		}
		if (((super&) tripRequest).hasDistance())
			tripDistance = tripRequest.tripDistance;
		if (((super&) tripRequest).hasCharacteristics())
			tripCharacteristics = tripRequest.tripCharacteristics;
		return *this;
	}

	TripRequest& operator=(TripRequest& tripRequest) {
		swap(tripId, tripRequest.tripId);
		swap(fromPlace, tripRequest.fromPlace);
		swap(toPlace, tripRequest.toPlace);
		swap(tDateTimePrefs, tripRequest.tDateTimePrefs);
		swap(nSeats, tripRequest.nSeats);
		swap(tripType, tripRequest.tripType);
		swap(requester, tripRequest.requester);
		if (tripRequest.hasTripReturnDate())
			swap(tReturnDateTimePrefs, tripRequest.tReturnDateTimePrefs);
		if (((super&) tripRequest).hasWeekDaysPeriodicity()) {
			swap(tWeekDays, tripRequest.tWeekDays);
			swap(tPeriodicity, tripRequest.tPeriodicity);
		}
		if (((super&) tripRequest).hasDistance())
			swap(tripDistance, tripRequest.tripDistance);
		if (((super&) tripRequest).hasCharacteristics())
			swap(tripCharacteristics, tripRequest.tripCharacteristics);
		return *this;
	}

	/**
	 * @return An Ice Identity for this datatype category and the data provided
	 */
	static ::Ice::Identity createIdentity(int tripId) {
		::Ice::Identity id;
		id.category = "trip_request";
		id.name = to_string(tripId);
		return id;
	}

	/**
	 * @return A proxy to an Ice Object incarnating the provided data, whose
	 * 			servant is added to adapter 's active servant map
	 */
	static ::cardroid::data::zerocice::TripRequestTypPrx createProxy(
			::cardroid::data::zerocice::TripRequestTyp& tRequestObject,
			::Ice::ObjectAdapterPtr adapter) {
		::cardroid::data::zerocice::TripRequestTypPrx tRequestPrx =
				::cardroid::data::zerocice::TripRequestTypPrx::uncheckedCast(
						adapter->createProxy(
								createIdentity(tRequestObject.getTripId())));
			fillProxyData(tRequestObject, tRequestPrx, adapter);
		return tRequestPrx;
	}

	/**
	 * @return A proxy to an Ice Object incarnating the provided data, whose
	 * 			servant is added to adapter 's active servant map
	 */
	static ::cardroid::data::zerocice::TripRequestTypPrx createProxy(
			ITripRequest& tRequestObject, ::Ice::ObjectAdapterPtr adapter) {
		::cardroid::data::zerocice::TripRequestTypPrx tRequestPrx =
				::cardroid::data::zerocice::TripRequestTypPrx::uncheckedCast(
						adapter->createProxy(
								createIdentity(tRequestObject.getTripId())));
			fillProxyData(tRequestObject, tRequestPrx, adapter);
		return tRequestPrx;
	}

	/**
	 * Fill the servant referenced by the given proxy with the data contained
	 * in the given object
	 */
	static void fillProxyData(
			::cardroid::data::zerocice::TripRequestTyp& object,
			::cardroid::data::zerocice::TripRequestTypPrx proxy,
			::Ice::ObjectAdapterPtr adapter) {
		parent::fillProxyData(object, proxy, adapter);
		proxy->setTripRequester(object.getTripRequester());
		proxy->setTripDateTimePrefs(object.getTripDateTimePrefs());
		if (object.hasTripReturnDate())
			proxy->setTripReturnDateTimePrefs(
					object.getTripReturnDateTimePrefs());
	}

	/**
	 * Fill the servant referenced by the given proxy with the data contained
	 * in the given object
	 */
	static void fillProxyData(ITripRequest& object,
			::cardroid::data::zerocice::TripRequestTypPrx proxy,
			::Ice::ObjectAdapterPtr adapter) {
		parent::fillProxyData(object, proxy, adapter);
		proxy->setTripRequester(
				User::createProxy(object.getRequester(), adapter));
		proxy->setTripDateTimePrefs(
				static_pointer_cast<DateTimePrefs>(DateTimePrefs().newInstance(
						shared_ptr<IDateTimePrefs>(&(object.getDateTime())))).get());
		if (object.hasReturnDate())
			proxy->setTripReturnDateTimePrefs(
					static_pointer_cast<DateTimePrefs>(
							DateTimePrefs().newInstance(shared_ptr<IDateTimePrefs>(
									&(object.getReturnDateTime())))).get());
	}

	/**
	 * @param proxy A proxy to a remote object implementing a TripRequest
	 * @return A local TripRequest object containing the data of the remote
	 * 			object referenced by proxy
	 */
	static shared_ptr<TripRequest> extractObject(
			::cardroid::data::zerocice::TripRequestTypPrx proxy) {
		shared_ptr<TripRequest> tripRequest;
		if (!(proxy->hasTripReturnDate() && proxy->hasWeekDaysPeriodicity()
				&& proxy->hasDistance() && proxy->hasCharacteristics())) {
			tripRequest = make_shared<TripRequest>
					(proxy->getTripId(),
					proxy->getPlace1().get(), proxy->getPlace2().get(),
					proxy->getTripDateTimePrefs().get(), proxy->getNSeats(),
					proxy->getTripRequester());
			if (!proxy->hasTripReturnDate() && !proxy->hasWeekDaysPeriodicity()
					&& !proxy->hasDistance() && !proxy->hasCharacteristics())
				return tripRequest;
		}

		::cardroid::data::zerocice::DateTimePrefsTyp* returnDateTimePrefs =
				proxy->getTripReturnDateTimePrefs().get();
		const vector<string>& weekDays = proxy->getTripWeekDays();
		::cardroid::data::zerocice::Periodicity periodicity =
				proxy->getTripPeriodicity();
		int distance = proxy->getDistance();
		string characteristics = proxy->getCharacteristics();

		if (!(proxy->hasTripReturnDate() && proxy->hasWeekDaysPeriodicity()
				&& proxy->hasDistance() && proxy->hasCharacteristics())) {
			if (proxy->hasTripReturnDate()) {
				tripRequest->setTripReturnDate(returnDateTimePrefs);
				tripRequest->setTripReturnDateTimePrefs(returnDateTimePrefs);
			}
			if (proxy->hasWeekDaysPeriodicity())
				tripRequest->setTripWeekDaysPeriodicity(weekDays, periodicity);
			if (proxy->hasDistance())
				static_pointer_cast<super>(tripRequest)->setDistance(distance);
			if (proxy->hasCharacteristics())
				static_pointer_cast<super>(tripRequest)->setCharacteristics(characteristics);
		} else
			tripRequest = make_shared<TripRequest>(
					proxy->getTripId(),
					proxy->getPlace1().get(), proxy->getPlace2().get(),
					proxy->getTripDateTimePrefs().get(), proxy->getNSeats(),
					returnDateTimePrefs, weekDays, periodicity, distance,
					characteristics, proxy->getTripRequester());
		return tripRequest;
	}

	/* ITripRequest interface */

	shared_ptr<ITripRequest> newInstance(
			const shared_ptr<ITripRequest> tripRequestObject) {
		if (!tripRequestObject)
			return NULL;
		if (dynamic_cast<TripRequest*>(tripRequestObject.get()))
			return tripRequestObject;

		shared_ptr<TripRequest> tripRequest;
		Place* fromPlace = static_pointer_cast<Place>(Place().newInstance(
				shared_ptr<IPlace>(&const_cast<IPlace&>(tripRequestObject->getFromPlace())))).get();
		Place* toPlace = static_pointer_cast<Place>(Place().newInstance(
				shared_ptr<IPlace>(&const_cast<IPlace&>(tripRequestObject->getToPlace())))).get();
		DateTimePrefs* datetime = static_pointer_cast<DateTimePrefs>(
				DateTimePrefs().newInstance(
						shared_ptr<IDateTimePrefs>(
								&(tripRequestObject->getDateTime())))).get();
		::cardroid::data::zerocice::UserTypPrx requester = User::createProxy(
				tripRequestObject->getRequester(), _adapter);
		if (!(tripRequestObject->hasReturnDate()
				&& tripRequestObject->hasWeekDaysPeriodicity()
				&& tripRequestObject->hasDistance()
				&& tripRequestObject->hasCharacteristics())) {
			tripRequest = make_shared<TripRequest>(tripRequestObject->getTripId(),
					fromPlace, toPlace, datetime,
					tripRequestObject->getNSeats(), requester);
			if (!tripRequestObject->hasReturnDate()
					&& !tripRequestObject->hasWeekDaysPeriodicity()
					&& !tripRequestObject->hasDistance()
					&& !tripRequestObject->hasCharacteristics())
				return shared_ptr<TripRequest>(tripRequest);
		}

		DateTimePrefs* returnDatetime = NULL;
		if (tripRequestObject->hasReturnDate())
			returnDatetime = static_pointer_cast<DateTimePrefs>(
					DateTimePrefs().newInstance(
					shared_ptr<IDateTimePrefs>(
							&(tripRequestObject->getReturnDateTime())))).get();
		::cardroid::data::zerocice::Periodicity periodicity;
		vector<string> weekDays;
		if (tripRequestObject->hasWeekDaysPeriodicity()) {
			periodicity =
					static_cast< ::cardroid::data::zerocice::Periodicity>(tripRequestObject->getPeriodicity());
			array<char, 7> wd = tripRequestObject->getWeekDays();
			weekDays = vector<string>();
			for (auto i = wd.begin(); i != wd.end(); i++)
				weekDays.push_back(to_string(*i));
		}
		int distance = tripRequestObject->getDistance();
		const string& characteristics = tripRequestObject->getCharacteristics();

		if (!(tripRequestObject->hasReturnDate()
				&& tripRequestObject->hasWeekDaysPeriodicity()
				&& tripRequestObject->hasDistance()
				&& tripRequestObject->hasCharacteristics())) {
			if (tripRequestObject->hasReturnDate()) {
				tripRequest->tReturnDateTimePrefs = returnDatetime;
				tripRequest->tReturnDate = static_cast< ::cardroid::data::zerocice::DateTyp*>(returnDatetime);
			}
			if (tripRequestObject->hasWeekDaysPeriodicity()) {
				tripRequest->tWeekDays = weekDays;
				tripRequest->tPeriodicity = periodicity;
			}
			if (tripRequestObject->hasDistance())
				tripRequest->tripDistance = distance;
			if (tripRequestObject->hasCharacteristics())
				tripRequest->tripCharacteristics = characteristics;
		} else
			tripRequest = make_shared<TripRequest>(tripRequestObject->getTripId(),
					fromPlace, toPlace, datetime,
					tripRequestObject->getNSeats(), returnDatetime, weekDays,
					periodicity, distance, characteristics, requester);
		return static_pointer_cast<ITripRequest>(tripRequest);
	}

	void setRequester(const IUser& r) {
		requester = User::createProxy(r, _adapter);
	}

	const IUser& getRequester() {
		return *(User::extractObject(requester).get());
	}

	void setDateTime(IDateTimePrefs& dateTimePrefsObject) {
		tDate = tDateTimePrefs = static_pointer_cast<DateTimePrefs>(
				DateTimePrefs().newInstance(shared_ptr<IDateTimePrefs>(
						&dateTimePrefsObject))).get();
	}

	IDateTimePrefs& getDateTime() {
		return *static_cast<DateTimePrefs*>(tDateTimePrefs.get());
	}

	void setReturnDateTime(IDateTimePrefs& dateTimePrefsObject) {
		tReturnDateTimePrefs = static_pointer_cast<DateTimePrefs>(
				DateTimePrefs().newInstance(shared_ptr<IDateTimePrefs>(
						&dateTimePrefsObject))).get();
		tReturnDate = tReturnDateTimePrefs.get();
	}

	IDateTimePrefs& getReturnDateTime() {
		return *(tReturnDateTimePrefs ?
				static_cast<DateTimePrefs*>(tReturnDateTimePrefs.get().get())
				: new DateTimePrefs());
	}

	shared_ptr<ITrip> newInstance(const shared_ptr<ITrip> tripObject) {
		return ((parent*)this)->newInstance(tripObject);
	}

	void setTripId(int tid) {
		tripId = tid;
	}

	int getTripId() {
		return tripId;
	}

	void setFromPlace(const IPlace& from) {
		fromPlace = static_pointer_cast<Place>(Place().newInstance(shared_ptr<IPlace>(&const_cast<IPlace&>(from)))).get();
	}

	const IPlace& getFromPlace() {
		return *static_cast<Place*>(fromPlace.get());
	}

	void setToPlace(const IPlace& to) {
		toPlace = static_pointer_cast<Place>(Place().newInstance(shared_ptr<IPlace>(&const_cast<IPlace&>(to)))).get();
	}

	const IPlace& getToPlace() {
		return *static_cast<Place*>(toPlace.get());
	}

	void setDate(IDate& datetime) {
		tDate = static_pointer_cast<Date>(DateTimePrefs().newInstance(shared_ptr<IDate>(&datetime))).get();
	}

	IDate& getDate() {
		return static_cast<IDate&>(
				*static_cast<DateTimePrefs*>(tDateTimePrefs.get()));
	}

	void setNSeats(int ns) {
		nSeats = ns;
	}

	int getNSeats() {
		return nSeats;
	}

	void setReturnDate(IDate& returnDateTime) {
		tReturnDate = static_pointer_cast<Date>(
				DateTimePrefs().newInstance(shared_ptr<IDate>(
						&returnDateTime))).get();
	}

	IDate& getReturnDate() {
		return static_cast<IDate&>(*dynamic_cast<Date*>(tReturnDateTimePrefs ?
				static_cast<DateTimePrefs*>(tReturnDateTimePrefs.get().get()) :
				new DateTimePrefs()));
	}

	bool hasReturnDate() {
		return tReturnDateTimePrefs && tReturnDate;
	}

	void setWeekDaysPeriodicity(array<char, 7> wd, ITrip::Periodicity p) {
		vector<string> weekDays;
		for (auto i = wd.begin(); i != wd.end(); i++)
			weekDays.push_back(to_string(*i));
		tWeekDays = weekDays;
		tPeriodicity = static_cast< ::cardroid::data::zerocice::Periodicity>(p);
	}

	array<char, 7> getWeekDays() {
		array<char, 7> wd = array<char, 7>();
		if (tWeekDays) {
			for (unsigned int n = 0; n < tWeekDays->size(); n++)
				wd[n] = tWeekDays.get()[n].c_str()[0];
		}
		return wd;
	}

	ITrip::Periodicity getPeriodicity() {
		return tPeriodicity ?
				static_cast<ITrip::Periodicity>(tPeriodicity.get()) : ITrip::Periodicity::EVERYWEEK;
	}

	bool hasWeekDaysPeriodicity() {
		return tWeekDays && tPeriodicity;
	}

	void setDistance(int distance) {
		tripDistance = distance;
	}

	int getDistance() {
		return tripDistance ? tripDistance.get() : 0;
	}

	bool hasDistance() {
		return tripDistance;
	}

	void setCharacteristics(const string& characteristics) {
		tripCharacteristics = characteristics;
	}

	const string& getCharacteristics() {
		const string& characteristics = tripCharacteristics ? tripCharacteristics.get() : "";
		return characteristics;
	}

	bool hasCharacteristics() {
		return tripCharacteristics;
	}

	void setTripType(int tt) {
		tripType = TRIP_REQUEST_TYPE;
	}

	int getTripType() {
		return TRIP_REQUEST_TYPE;
	}

	/* Overriding superclass */

	const string& ice_staticId() {
		return super::ice_staticId();
	}

	int getTripId(const ::Ice::Current& = ::Ice::Current()) const {
		return tripId;
	}

	void setTripId(int tid, const ::Ice::Current& = ::Ice::Current()) {
		tripId = tid;
	}

	::cardroid::data::zerocice::PlaceTypPtr getPlace1(const ::Ice::Current& =
			::Ice::Current()) const {
		return fromPlace;
	}

	void setPlace1(const ::cardroid::data::zerocice::PlaceTypPtr& place1,
			const ::Ice::Current& = ::Ice::Current()) {
		fromPlace = place1;
	}

	::cardroid::data::zerocice::PlaceTypPtr getPlace2(const ::Ice::Current& =
			::Ice::Current()) const {
		return toPlace;
	}

	void setPlace2(const ::cardroid::data::zerocice::PlaceTypPtr& place2,
			const ::Ice::Current& = ::Ice::Current()) {
		toPlace = place2;
	}

	::cardroid::data::zerocice::DateTypPtr getTripDate(const ::Ice::Current& =
			::Ice::Current()) const {
		return tDateTimePrefs;
	}

	void setTripDate(const ::cardroid::data::zerocice::DateTypPtr& tripDate,
			const ::Ice::Current& = ::Ice::Current()) {
		tDate = tripDate;
	}

	int getNSeats(const ::Ice::Current& = ::Ice::Current()) const {
		return nSeats;
	}

	void setNSeats(int ns, const ::Ice::Current& = ::Ice::Current()) {
		nSeats = ns;
	}

	::cardroid::data::zerocice::DateTypPtr getTripReturnDate(
			const ::Ice::Current& = ::Ice::Current()) const {
		return static_cast< ::cardroid::data::zerocice::DateTyp*>(
				tReturnDateTimePrefs ? tReturnDateTimePrefs.get().get()
				: static_cast< ::cardroid::data::zerocice::DateTimePrefsTyp*>(new DateTimePrefs()));
	}

	void setTripReturnDate(const ::cardroid::data::zerocice::DateTypPtr& trd,
			const ::Ice::Current& = ::Ice::Current()) {
		tReturnDate = trd;
	}

	bool hasTripReturnDate(const ::Ice::Current& = ::Ice::Current()) const {
		return tReturnDateTimePrefs;
	}

	::cardroid::data::zerocice::WeekdaysV getTripWeekDays(
			const ::Ice::Current& = ::Ice::Current()) const {
		return tWeekDays ? tWeekDays.get() : vector<string>();
	}

	::cardroid::data::zerocice::Periodicity getTripPeriodicity(
			const ::Ice::Current& = ::Ice::Current()) const {
		return tPeriodicity ? tPeriodicity.get() : ::cardroid::data::zerocice::Periodicity::EVERYWEEK;
	}

	void setTripWeekDaysPeriodicity(
			const ::cardroid::data::zerocice::WeekdaysV& weekDays,
			::cardroid::data::zerocice::Periodicity periodicity,
			const ::Ice::Current& = ::Ice::Current()) {
		tWeekDays = weekDays;
		tPeriodicity = periodicity;
	}

	bool hasWeekDaysPeriodicity(const ::Ice::Current& =
			::Ice::Current()) const {
		return tWeekDays && tPeriodicity;
	}

	int getDistance(const ::Ice::Current& = ::Ice::Current()) const {
		return tripDistance ? tripDistance.get() : NULL;
	}

	void setDistance(int distance, const ::Ice::Current& = ::Ice::Current()) {
		tripDistance = distance;
	}

	bool hasDistance(const ::Ice::Current& = ::Ice::Current()) const {
		return tripDistance;
	}

	string getCharacteristics(const ::Ice::Current& = ::Ice::Current()) const {
		return tripCharacteristics ? tripCharacteristics.get() : NULL;
	}

	void setCharacteristics(const string& characteristics,
			const ::Ice::Current& = ::Ice::Current()) {
		tripCharacteristics = characteristics;
	}

	bool hasCharacteristics(const ::Ice::Current& = ::Ice::Current()) const {
		return tripCharacteristics;
	}

	void setTripType(int tt, const ::Ice::Current& = ::Ice::Current()) {
		tripType = TRIP_REQUEST_TYPE;
	}

	int getTripType(const ::Ice::Current& = ::Ice::Current()) const {
		return TRIP_REQUEST_TYPE;
	}

	string toString(const ::Ice::Current& = ::Ice::Current()) const {
		return fromPlace->getName() + " - " + toPlace->getName() + " ["
				+ tDateTimePrefs->toString() + "]";
	}

	::cardroid::data::zerocice::DateTimePrefsTypPtr getTripDateTimePrefs(
			const ::Ice::Current& = ::Ice::Current()) const {
		return tDateTimePrefs.get();
	}

	void setTripDateTimePrefs(
			const ::cardroid::data::zerocice::DateTimePrefsTypPtr& dtp,
			const ::Ice::Current& = ::Ice::Current()) {
		tDateTimePrefs = dtp;
	}

	::cardroid::data::zerocice::DateTimePrefsTypPtr getTripReturnDateTimePrefs(
			const ::Ice::Current& = ::Ice::Current()) const {
		return tReturnDateTimePrefs.get().get() ? tReturnDateTimePrefs.get().get() :
				static_cast< ::cardroid::data::zerocice::DateTimePrefsTyp*>(new DateTimePrefs());
	}

	void setTripReturnDateTimePrefs(
			const ::cardroid::data::zerocice::DateTimePrefsTypPtr& rdtp,
			const ::Ice::Current& = ::Ice::Current()) {
		tReturnDateTimePrefs = rdtp;
	}

	::cardroid::data::zerocice::UserTypPrx getTripRequester(
			const ::Ice::Current& = ::Ice::Current()) const {
		return requester;
	}

	void setTripRequester(const ::cardroid::data::zerocice::UserTypPrx& r,
			const ::Ice::Current& = ::Ice::Current()) {
		requester = r;
	}

	/* Comparable<TripRequest> interface */

	bool operator==(const TripRequest& other) {
		return *(static_cast<DateTimePrefs*>(getTripDateTimePrefs().get())) ==
			*(static_cast<DateTimePrefs*>(const_cast<TripRequest&>(other).getTripDateTimePrefs().get()));
	}

	bool operator<(const TripRequest& other) {
		return *(static_cast<DateTimePrefs*>(getTripDateTimePrefs().get())) <
			*(static_cast<DateTimePrefs*>(const_cast<TripRequest&>(other).getTripDateTimePrefs().get()));
	}

	bool operator>(const TripRequest& other) {
		return *(static_cast<DateTimePrefs*>(getTripDateTimePrefs().get())) >
			*(static_cast<DateTimePrefs*>(const_cast<TripRequest&>(other).getTripDateTimePrefs().get()));
	}

	/* ObjectFactory interface */

	::Ice::ObjectPtr create(const string& type) {
		if (!type.compare(ice_staticId()))
			return new TripRequest(_adapter);
		return NULL;
	}

	void destroy() {
	}

private:

	::Ice::ObjectAdapterPtr _adapter;
	static const int TRIP_TYPE = 0, TRIP_OFFER_TYPE = 1, TRIP_REQUEST_TYPE = 2;
	static const long long int serialVersionUID = 4965726405855943632L;
	typedef ::cardroid::data::zerocice::TripRequestTyp super;
	typedef Trip parent;
};
}

namespace Oracle {

using namespace Cardroid::Data;

/**
 * \class TripRequest
 * Domain class implementing a TripRequest for its storage and retrieval from an
 * Oracle database
 */
class TripRequest: public ::TripRequestTyp, public ITripRequest {

public:

	TripRequest() {
	}

	TripRequest(void* ctxOCCI_) :
			super(ctxOCCI_) {
	}

	/**
	 * Short constructor
	 */
	TripRequest(Place& from, Place& to, DateTimePrefs datetime, User& requester,
			int requestedSeats) {
		setFromPlace(from);
		setToPlace(to);
		setDateTime(datetime);
		setRequester(requester);
		setNSeats(requestedSeats);
	}

	/**
	 * Another constructor
	 */
	TripRequest(Place& from, Place& to, DateTimePrefs datetime,
			DateTimePrefs& returnDatetime, User& requester,
			int requestedSeats) {
		setFromPlace(from);
		setToPlace(to);
		setDateTime(datetime);
		setRequester(requester);
		setNSeats(requestedSeats);
	}

	/**
	 * Another constructor
	 */
	TripRequest(Place& from, Place& to, DateTimePrefs datetime, User& requester,
			int requestedSeats, vector<string>& weekDays,
			const string& periodicity) {
		setFromPlace(from);
		setToPlace(to);
		setDateTime(datetime);
		setRequester(requester);
		setNSeats(requestedSeats);
		setWeek_days(weekDays);
		setPeriodicity(periodicity);
	}

	/**
	 * Another constructor
	 */
	TripRequest(Place& from, Place& to, DateTimePrefs datetime,
			DateTimePrefs returnDatetime, User& requester, int requestedSeats,
			vector<string>& weekDays, const string& periodicity) {
		setFromPlace(from);
		setToPlace(to);
		setDateTime(datetime);
		setReturnDateTime(returnDatetime);
		setRequester(requester);
		setNSeats(requestedSeats);
		setWeek_days(weekDays);
		setPeriodicity(periodicity);
	}

	/**
	 * Long constructor
	 */
	TripRequest(Place& fromPlace, Place& toPlace, DateTimePrefs datetime,
			int nSeats, DateTimePrefs returnDatetime, vector<string>& weekDays,
			const string& periodicity, int distance,
			const string& characteristics, User& requester) {
		setFromPlace(fromPlace);
		setToPlace(toPlace);
		setDateTime(datetime);
		setNSeats(nSeats);
		setReturnDateTime(returnDatetime);
		setWeek_days(weekDays);
		setPeriodicity(periodicity);
		setDistance(distance);
		setCharacteristics(characteristics);
		setRequester(requester);
	}

	TripRequest(const ::TripRequestTyp& tripRequest) {
		setTrip_id(tripRequest.getTrip_id());
		setFrom_place(tripRequest.getFrom_place());
		setTo_place(tripRequest.getTo_place());
		setDatetime(dynamic_cast<DateTimePrefs*>(tripRequest.getDatetime()));
		setN_seats(tripRequest.getN_seats());
		setRequester(tripRequest.getRequester());
		if (tripRequest.getReturn_datetime())
			setReturn_datetime(dynamic_cast<DateTimePrefs*>(tripRequest.getReturn_datetime()));
		if (tripRequest.getWeek_days().size() > 0 && tripRequest.getPeriodicity().length() > 0)
			setWeek_days(tripRequest.getWeek_days());
		if (((int)tripRequest.getDistance()) > 0)
			setDistance(tripRequest.getDistance());
		if (tripRequest.getCharacteristics().length() > 0)
			setCharacteristics(tripRequest.getCharacteristics());
	}

	TripRequest(const TripRequest& tr) {
		TripRequest& tripRequest = const_cast<TripRequest&>(tr);
		setTrip_id(tripRequest.getTrip_id());
		setFrom_place(tripRequest.getFrom_place());
		setTo_place(tripRequest.getTo_place());
		setDatetime(tripRequest.getDatetime());
		setN_seats(tripRequest.getN_seats());
		setRequester(tripRequest.getRequester());
		if (tripRequest.hasReturnDate())
			setReturn_datetime(tripRequest.getReturn_datetime());
		if (tripRequest.hasWeekDaysPeriodicity())
			setWeek_days(tripRequest.getWeek_days());
		if (tripRequest.hasDistance())
			setDistance(tripRequest.getDistance());
		if (tripRequest.hasCharacteristics())
			setCharacteristics(tripRequest.getCharacteristics());
	}

	TripRequest(TripRequest&& tripRequest) {
		setTrip_id(tripRequest.getTrip_id());
		setFrom_place(tripRequest.getFrom_place());
		setTo_place(tripRequest.getTo_place());
		setDatetime(tripRequest.getDatetime());
		setN_seats(tripRequest.getN_seats());
		setRequester(tripRequest.getRequester());
		if (tripRequest.hasReturnDate())
			setReturn_datetime(tripRequest.getReturn_datetime());
		if (tripRequest.hasWeekDaysPeriodicity())
			setWeek_days(tripRequest.getWeek_days());
		if (tripRequest.hasDistance())
			setDistance(tripRequest.getDistance());
		if (tripRequest.hasCharacteristics())
			setCharacteristics(tripRequest.getCharacteristics());
	}

	~TripRequest() {
	}

	TripRequest& operator=(TripRequest tripRequest) {
		setTrip_id(tripRequest.getTrip_id());
		setFrom_place(tripRequest.getFrom_place());
		setTo_place(tripRequest.getTo_place());
		setDatetime(tripRequest.getDatetime());
		setN_seats(tripRequest.getN_seats());
		setRequester(tripRequest.getRequester());
		if (tripRequest.hasReturnDate())
			setReturn_datetime(tripRequest.getReturn_datetime());
		if (tripRequest.hasWeekDaysPeriodicity())
			setWeek_days(tripRequest.getWeek_days());
		if (tripRequest.hasDistance())
			setDistance(tripRequest.getDistance());
		if (tripRequest.hasCharacteristics())
			setCharacteristics(tripRequest.getCharacteristics());
		return *this;
	}

	TripRequest& operator=(TripRequest& tripRequest) {
		setTrip_id(tripRequest.getTrip_id());
		setFrom_place(tripRequest.getFrom_place());
		setTo_place(tripRequest.getTo_place());
		setDatetime(tripRequest.getDatetime());
		setN_seats(tripRequest.getN_seats());
		setRequester(tripRequest.getRequester());
		if (tripRequest.hasReturnDate())
			setReturn_datetime(tripRequest.getReturn_datetime());
		if (tripRequest.hasWeekDaysPeriodicity())
			setWeek_days(tripRequest.getWeek_days());
		if (tripRequest.hasDistance())
			setDistance(tripRequest.getDistance());
		if (tripRequest.hasCharacteristics())
			setCharacteristics(tripRequest.getCharacteristics());
		return *this;
	}

	/* ITripRequest interface */

	shared_ptr<ITripRequest> newInstance(
			const shared_ptr<ITripRequest> tripRequestObject) {
		if (!tripRequestObject)
			return NULL;
		if (dynamic_cast<TripRequest*>(tripRequestObject.get()))
			return tripRequestObject;

		shared_ptr<TripRequest> tRequest;
		Place from = *(static_pointer_cast<Place>(Place().newInstance(
				shared_ptr<IPlace>(&const_cast<IPlace&>(
						tripRequestObject->getFromPlace())))).get());
		Place to = *(static_pointer_cast<Place>(Place().newInstance(
				shared_ptr<IPlace>(&const_cast<IPlace&>(
						tripRequestObject->getToPlace())))).get());
		DateTimePrefs datetime =
				*(static_pointer_cast<DateTimePrefs>(DateTimePrefs().newInstance(
						shared_ptr<IDateTimePrefs>(
								&(tripRequestObject->getDateTime())))).get());
		DateTimePrefs returnDatetime =
				tripRequestObject->hasReturnDate() ?
						*(static_pointer_cast<DateTimePrefs>(DateTimePrefs().newInstance(
								shared_ptr<IDateTimePrefs>(
										&(tripRequestObject->getReturnDateTime())))).get()) :
						DateTimePrefs();
		vector<string> weekDaysVarray = vector<string>();
		string periodicityName;
		if (tripRequestObject->hasWeekDaysPeriodicity()) {
			array<char, 7> weekDays = tripRequestObject->getWeekDays();
			for (auto i = weekDays.begin(); i != weekDays.end(); i++)
				weekDaysVarray.push_back(to_string(*i));
			switch (tripRequestObject->getPeriodicity()) {
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
		User& requester = *(static_pointer_cast<User>(User().newInstance(
				shared_ptr<IUser>(&const_cast<IUser&>(
						tripRequestObject->getRequester())))).get());

		if (tripRequestObject->hasReturnDate()
				&& tripRequestObject->hasWeekDaysPeriodicity())
			tRequest = make_shared<TripRequest>(from, to, datetime, returnDatetime,
					requester, tripRequestObject->getNSeats(), weekDaysVarray,
					periodicityName);
		if (!tripRequestObject->hasReturnDate()
				& !tripRequestObject->hasWeekDaysPeriodicity())
			tRequest = make_shared<TripRequest>(from, to, datetime, requester,
					tripRequestObject->getNSeats());
		if (tripRequestObject->hasReturnDate()
				&& !tripRequestObject->hasWeekDaysPeriodicity())
			tRequest = make_shared<TripRequest>(from, to, datetime, returnDatetime,
					requester, tripRequestObject->getNSeats());
		if (!tripRequestObject->hasReturnDate()
				&& tripRequestObject->hasWeekDaysPeriodicity())
			tRequest = make_shared<TripRequest>(from, to, datetime, requester,
					tripRequestObject->getNSeats(), weekDaysVarray,
					periodicityName);

		return static_pointer_cast<ITripRequest>(tRequest);
	}

	void setRequester(const IUser& requester) {
		setRequester(static_pointer_cast<User>(User().newInstance(
				shared_ptr<IUser>(&const_cast<IUser&>(requester)))).get());
	}

	const IUser& getRequester() {
		return *(const_cast<const TripRequest&>(*this).getRequester());
	}

	void setDateTime(IDateTimePrefs& dateTimePrefsObject) {
		setDatetime(
				static_pointer_cast<DateTimePrefs>(DateTimePrefs().newInstance(
						shared_ptr<IDateTimePrefs>(&dateTimePrefsObject))).get());
	}

	IDateTimePrefs& getDateTime() {
		return *getDatetime();
	}

	void setReturnDateTime(IDateTimePrefs& dateTimePrefsObject) {
		setReturn_datetime(
				static_pointer_cast<DateTimePrefs>(DateTimePrefs().newInstance(
						shared_ptr<IDateTimePrefs>(&dateTimePrefsObject))).get());
	}

	IDateTimePrefs& getReturnDateTime() {
		return *getReturn_datetime();
	}

	shared_ptr<ITrip> newInstance(const shared_ptr<ITrip> tripObject) {
		return ((parent*) this)->newInstance(tripObject);
	}

	void setTripId(int tripId) {
		setTrip_id(tripId);
	}

	int getTripId() {
		return getTrip_id();
	}

	void setFromPlace(const IPlace& from) {
		setFrom_place(static_pointer_cast<Place>(Place().newInstance(
				shared_ptr<IPlace>(&const_cast<IPlace&>(from)))).get());
	}

	const IPlace& getFromPlace() {
		return *getFrom_place();
	}

	void setToPlace(const IPlace& to) {
		setTo_place(static_pointer_cast<Place>(Place().newInstance(
				shared_ptr<IPlace>(&const_cast<IPlace&>(to)))).get());
	}

	const IPlace& getToPlace() {
		return *getTo_place();
	}

	void setDate(IDate& dateTime) {
		setDatetime(
			static_pointer_cast<DateTimePrefs>(DateTimePrefs().newInstance(
						shared_ptr<IDateTimePrefs>(&static_cast<IDateTimePrefs&>(
								dateTime)))).get());
	}

	IDate& getDate() {
		return *getDatetime();
	}

	void setNSeats(int nSeats) {
		setN_seats(nSeats);
	}

	int getNSeats() {
		return getN_seats();
	}

	void setReturnDate(IDate& returnDateTime) {
		setReturn_datetime(
				static_pointer_cast<DateTimePrefs>(DateTimePrefs().newInstance(
						shared_ptr<IDateTimePrefs>(&static_cast<IDateTimePrefs&>(
								returnDateTime)))).get());
	}

	IDate& getReturnDate() {
		return *getReturn_datetime();
	}

	bool hasReturnDate() {
		return getReturnDate().getTimeInMillis() > 0;
	}

	void setWeekDaysPeriodicity(array<char, 7> weekDays,
			ITrip::Periodicity periodicity) {
		vector<string> wd;
		for (auto i = weekDays.begin(); i != weekDays.end(); i++)
			wd.push_back(to_string(*i));
		setWeek_days(wd);
		string p;
		switch (periodicity) {
		case EVERYWEEK:
			p = "EVERYWEEK";
			break;
		case EVENWEEKS:
			p = "EVENWEEKS";
			break;
		case ODDWEEKS:
			p = "ODDWEEKS";
			break;
		}
		setPeriodicity(p);
	}

	array<char, 7> getWeekDays() {
		vector<string>& weekDays = getWeek_days();
		array<char, 7> wd = array<char, 7>();
		for (unsigned int n = 0; n < weekDays.size(); n++)
			wd[n] = weekDays[n].c_str()[0];
		return wd;
	}

	ITrip::Periodicity getPeriodicity() {
		const string& periodicityName =
				const_cast<const TripRequest&>(*this).getPeriodicity();
		ITripRequest::Periodicity periodicity = EVERYWEEK;
		if (!periodicityName.compare("EVERYWEEK"))
			return periodicity = ITripRequest::Periodicity::EVERYWEEK;
		if (!periodicityName.compare("EVENWEEKS"))
			return periodicity = ITripRequest::Periodicity::EVENWEEKS;
		if (!periodicityName.compare("ODDWEEKS"))
			return periodicity = ITripRequest::Periodicity::ODDWEEKS;
		else
			return periodicity;
	}

	bool hasWeekDaysPeriodicity() {
		return getWeek_days().size() > 0;
	}

	void setDistance(int distance) {
		setDistance(
				(::oracle::occi::Number) distance);
	}

	int getDistance() {
		return (int) (const_cast<const TripRequest&>(*this).getDistance());
	}

	bool hasDistance() {
		return getDistance() > 0;
	}

	const string& getCharacteristics() {
		const string& characteristics = const_cast<const TripRequest&>(*this).getCharacteristics();
		return characteristics;
	}

	bool hasCharacteristics() {
		return getCharacteristics().length() > 0;
	}

	void setTripType(int tripType) {
		setTrip_type(
				(::oracle::occi::Number) TRIP_REQUEST_TYPE);
	}

	int getTripType() {
		return TRIP_REQUEST_TYPE;
	}

	/* Overriding superclass */

	User* getRequester() const {
		User* requester = NULL;
		try {
			requester = super::getRequester();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return requester;
	}

	void setRequester(User* value) {
		try {
			super::setRequester(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

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

	DateTimePrefs* getDatetime() const {
		DateTimePrefs* datetime = NULL;
		try {
			datetime = (DateTimePrefs*) ((super*) this)->getDatetime();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return datetime;
	}

	void setDatetime(DateTimePrefs* value) {
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

	DateTimePrefs* getReturn_datetime() const {
		DateTimePrefs* rdatetime = NULL;
		try {
			rdatetime = (DateTimePrefs*) ((super*) this)->getReturn_datetime();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return rdatetime;
	}

	void setReturn_datetime(DateTimePrefs* value) {
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

	void setCharacteristics(const string& value) {
		try {
			((super*) this)->setCharacteristics(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	::oracle::occi::Number getTrip_type() const {
		return TRIP_REQUEST_TYPE;
	}

	void setTrip_type(const ::oracle::occi::Number& value) {
		try {
			((super*) this)->setTrip_type(TRIP_REQUEST_TYPE);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	static string getSqlTypeName() {
		return string("ANDROID.TRIP_REQUEST_TYP");
	}

	/* Comparable<TripRequest> interface */

	bool operator==(const TripRequest& other) {
		return *getDatetime() ==
				*(const_cast<TripRequest&>(other).getDatetime());
	}
	bool operator<(const TripRequest& other) {
		return *getDatetime() <
				*(const_cast<TripRequest&>(other).getDatetime());
	}

	bool operator>(const TripRequest& other) {
		return *getDatetime() >
				*(const_cast<TripRequest&>(other).getDatetime());
	}

private:

	static const int TRIP_TYPE = 0, TRIP_OFFER_TYPE = 1, TRIP_REQUEST_TYPE = 2;
	typedef ::TripRequestTyp super;
	typedef Trip parent;
};
}
}
}

#endif /* ITRIPREQUEST_H_ */
