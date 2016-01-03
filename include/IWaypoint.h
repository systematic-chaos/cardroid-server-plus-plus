#ifndef IWAYPOINT_H_
#define IWAYPOINT_H_

#include "IPlace.h"

namespace Cardroid {
namespace Data {

/**
 * \interface IWaypoint
 * Public operations interface for the implementation of a IWaypoint object
 */
class IWaypoint: public IPlace {

public:

	/**
	 * Create an instance of the class implementing this interface,
	 * from the received parameter, which also implements it.
	 * This method acts as an Abstract Factory, for the sake of the
	 * implementation of the Abstract Factory pattern in the search of
	 * interoperability among the different implementations of this interface
	 * which could exist in the different subsystems of the platform
	 * @param waypointObject An object instance implementing this interface
	 * @return An instance of the class implementing this interface, containing
	 * 			exactly the same data of the received bitmapObject parameter,
	 * 			from the viewpoint of the operations defined in this interface
	 */
	virtual shared_ptr<IWaypoint> newInstance(
			const shared_ptr<IWaypoint> waypointObject) = 0;

	/**
	 * @param nOrder The new order of this IWaypoint in the waypoint
	 * 					collection that contains it
	 */
	virtual void setNOrder(int nOrder) = 0;

	/**
	 * @return The order of this IWaypoint in the waypoint collection that
	 * 			contains it
	 */
	virtual int getNOrder() = 0;

	/**
	 * @param placeWaypoint The new place this IWaypoint refers to
	 */
	virtual void setPlaceWaypoint(const IPlace& placeWaypoint) = 0;

	/**
	 * @return The place this IWaypoint refers to
	 */
	virtual IPlace& getPlaceWaypoint() = 0;

	virtual ~IWaypoint() {
	}
};

namespace Ice {

using namespace IceProxy;
using namespace Cardroid::Data;

/**
 * \class Waypoint
 * Domain class implementing a waypoint for its transmission between systems
 * communicating across an Ice network infrastructure
 */
class Waypoint: public ::cardroid::data::zerocice::WaypointTyp,
		public IWaypoint, public ::Ice::ObjectFactory {

public:

	Waypoint() {
	}

	/**
	 * Default constructor
	 */
	Waypoint(int no, const ::cardroid::data::zerocice::PlaceTypPtr& wp) {
		nOrder = no;
		waypointPlace = const_cast< ::cardroid::data::zerocice::PlaceTypPtr&>(wp);
	}

	Waypoint(const WaypointTyp& waypoint) {
		nOrder = waypoint.nOrder;
		waypointPlace = waypoint.waypointPlace;
	}

	Waypoint(const Waypoint& waypoint) {
		nOrder = waypoint.nOrder;
				waypointPlace = waypoint.waypointPlace;
	}

	Waypoint(Waypoint&& waypoint) {
		swap(nOrder, waypoint.nOrder);
		swap(waypointPlace, waypoint.waypointPlace);
	}

	~Waypoint() {
		delete waypointPlace._ptr;
	}

	Waypoint& operator=(Waypoint waypoint) {
		nOrder = waypoint.nOrder;
		waypointPlace = waypoint.waypointPlace;
		return *this;
	}

	Waypoint& operator=(Waypoint& waypoint) {
		swap(nOrder, waypoint.nOrder);
		swap(waypointPlace, waypoint.waypointPlace);
		return *this;
	}

	/* IWaypoint interface */

	shared_ptr<IPlace> newInstance(const shared_ptr<IPlace> placeObject) {
		return Place().newInstance(placeObject);
	}

	void setNOrder(int no) {
		nOrder = no;
	}

	int getNOrder() {
		return nOrder;
	}

	void setName(const string& name) {
		waypointPlace->setName(name);
	}

	const string& getName() {
		const string& n = waypointPlace->getName();
		return n;
	}

	void setCoordinates(ILatLng* coords) {
		waypointPlace->setCoords(*static_cast<LatLng*>(
				LatLng().newInstance(shared_ptr<ILatLng>(coords)).get()));
	}

	ILatLng* getCoordinates() {
		return new LatLng(waypointPlace->getCoords());
	}

	void setDescription(const string& description) {
		waypointPlace->setDescription(description);
	}

	const string& getDescription() {
		const string& d = waypointPlace->getDescription();
		return d;
	}

	bool hasDescription() {
		return waypointPlace->hasDescription();
	}

	void setSnapshot(const ::oracle::occi::Blob& bitmapObject) {
		waypointPlace->setSnapshotBytes(
				::Cardroid::occi::blobToByteArray(bitmapObject));
	}

	const ::oracle::occi::Blob& getSnapshot() {
		return ::Cardroid::occi::byteArrayToBlob(
				waypointPlace->getSnapshotBytes());
	}

	bool hasSnapshot() {
		return waypointPlace->hasSnapshot();
	}

	shared_ptr<IWaypoint> newInstance(
			const shared_ptr<IWaypoint> waypointObject) {
		if (!waypointObject)
			return NULL;
		if (dynamic_cast<Waypoint*>(waypointObject.get()))
			return waypointObject;

		return make_shared<Waypoint>(
			waypointObject->getNOrder(),
					static_pointer_cast<Place>(newInstance(shared_ptr<IPlace>(
							&(waypointObject->getPlaceWaypoint())))).get());
	}

	void setPlaceWaypoint(const IPlace& placeWaypoint) {
		setWaypointPlace(static_cast<Place*>(
				Place().newInstance(shared_ptr<IPlace>(&const_cast<IPlace&>(placeWaypoint))).get()));
	}

	IPlace& getPlaceWaypoint() {
		const Place& pwp = dynamic_cast<Place*>(waypointPlace.get()) ?
				*static_cast<Place*>(waypointPlace.get()) : *(new Place(*(waypointPlace.get())));
		return const_cast<Place&>(pwp);
	}

	/* Overriding superclass */

	const string& ice_staticId() {
		return super::ice_staticId();
	}

	int getNOrder(const ::Ice::Current& = ::Ice::Current()) const {
		return nOrder;
	}

	void setNOrder(int no, const ::Ice::Current& = ::Ice::Current()) {
		nOrder = no;
	}

	::cardroid::data::zerocice::PlaceTypPtr getWaypointPlace(
			const ::Ice::Current& = ::Ice::Current()) const {
		return waypointPlace;
	}

	void setWaypointPlace(const ::cardroid::data::zerocice::PlaceTypPtr& wp,
			const ::Ice::Current& = ::Ice::Current()) {
		waypointPlace = wp;
	}

	string toString(const ::Ice::Current& = ::Ice::Current()) const {
		return nOrder + " " + waypointPlace->getName();
	}

	/* ObjectFactory interface */

	::Ice::ObjectPtr create(const string& type) {
		if (!type.compare(ice_staticId()))
			return new Waypoint();
		return NULL;
	}

	void destroy() {
	}

private:

	static const long long int serialVersionUID = -939224739039674266L;
	typedef ::cardroid::data::zerocice::WaypointTyp super;
};
}

namespace Oracle {

using namespace Cardroid::Data;

/**
 * \class TripOffer_Waypoint
 * Domain class implementing a Waypoint for its storage and retrieval
 * from an Oracle database
 */
class Waypoint: public ::WaypointTyp, public IWaypoint {

public:

	Waypoint() {
	}

	Waypoint(void* ctxOCCI_) :
			super(ctxOCCI_) {
	}

	/**
	 * Default constructor
	 */
	Waypoint(int nOrder, Place* place) {
		setNOrder(nOrder);
		setPlace(place);
	}

	Waypoint(const Waypoint& wp) {
		Waypoint& waypoint = const_cast<Waypoint&>(wp);
		setNOrder(waypoint.getNOrder());
		setPlaceWaypoint(waypoint.getPlaceWaypoint());
	}

	Waypoint(Waypoint&& waypoint) {
		setNOrder(waypoint.getNOrder());
		setPlaceWaypoint(waypoint.getPlaceWaypoint());
	}

	~Waypoint() {
	}

	Waypoint& operator=(Waypoint waypoint) {
		setNOrder(waypoint.getNOrder());
		setPlaceWaypoint(waypoint.getPlaceWaypoint());
		return *this;
	}

	Waypoint& operator=(Waypoint& waypoint) {
		setNOrder(waypoint.getNOrder());
		setPlaceWaypoint(waypoint.getPlaceWaypoint());
		return *this;
	}

	/* superclass accessors */

	void setNOrder(int nOrder) {
		setN_order(nOrder);
	}

	int getNOrder() {
		return getN_order();
	}

	void setPlaceWaypoint(const IPlace& placeWaypoint) {
		setPlace(static_pointer_cast<Place>(newInstance(
					shared_ptr<IPlace>(&const_cast<IPlace&>(placeWaypoint)))).get());
	}

	IPlace& getPlaceWaypoint() {
		const Place& place = getPlace();
		return const_cast<Place&>(place);
	}

	/* IPlace interface */

	shared_ptr<IPlace> newInstance(const shared_ptr<IPlace> placeObject) {
		return Place().newInstance(placeObject);
	}

	shared_ptr<IWaypoint> newInstance(
			const shared_ptr<IWaypoint> waypointObject) {
		if (!waypointObject)
			return NULL;
		if (dynamic_cast<Waypoint*>(waypointObject.get()))
			return waypointObject;

		return static_pointer_cast<IWaypoint>(make_shared<Waypoint>(
				waypointObject->getNOrder(), static_pointer_cast<Place>(
				newInstance(shared_ptr<IPlace>(&(waypointObject->getPlaceWaypoint())))).get()));
	}

	void setName(const string& name) {
		getPlaceWaypoint().setName(name);
	}

	const string& getName() {
		return getPlaceWaypoint().getName();
	}

	void setCoordinates(ILatLng* coords) {
		getPlaceWaypoint().setCoordinates(coords);
	}

	ILatLng* getCoordinates() {
		return getPlaceWaypoint().getCoordinates();
	}

	void setDescription(const string& description) {
		getPlaceWaypoint().setDescription(description);
	}

	const string& getDescription() {
		return getPlaceWaypoint().getDescription();
	}

	bool hasDescription() {
		return getPlaceWaypoint().hasDescription();
	}

	void setSnapshot(const ::oracle::occi::Blob& bitmapObject) {
		getPlaceWaypoint().setSnapshot(bitmapObject);
	}

	const ::oracle::occi::Blob& getSnapshot() {
		return getPlaceWaypoint().getSnapshot();
	}

	bool hasSnapshot() {
		return getPlaceWaypoint().hasSnapshot();
	}

	/* superclass methods */

	::oracle::occi::Number getN_order() const {
		::oracle::occi::Number nOrder = 0.;
		try {
			nOrder = super::getN_order();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return nOrder;
	}

	void setN_order(const ::oracle::occi::Number& value) {
		try {
			super::setN_order(value);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	Place* getPlace() const {
		Place* place = NULL;
		try {
			place = super::getPlace();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return place;
	}

	void setPlace(Place* value) {
		try {
			super::setPlace(value);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	static string getSqlTypeName() {
		return string("ANDROID.WAYPOINT_TYP");
	}

private:

	typedef ::WaypointTyp super;

};
}
}
}

#endif /* IWAYPOINT_H_ */
