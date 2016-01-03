#ifndef IPLACE_H_
#define IPLACE_H_

#include <Ice/Current.h>
#include <Ice/Identity.h>
#include <Ice/ObjectAdapter.h>
#include <Ice/ObjectFactory.h>
#include <Ice/ProxyHandle.h>
#include <occiCommon.h>
#include <occiControl.h>
#include <occiData.h>
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "Cardroid.h"
#include "DbTypes.h"
#include "ILatLng.h"
#include "Util.h"

using namespace std;

namespace Cardroid {
namespace Data {

/**
 * \interface IPlace
 * Public operations interface for the implementation of an Place object
 */
class IPlace {

public:

	/**
	 * Create an instance of the class implementing this interface,
	 * from the received parameter, which also implements it.
	 * This method acts as an Abstract Factory, for the sake of the
	 * implementation of the Abstract Factory pattern in the search of
	 * interoperability among the different implementations of this interface
	 * which could exist in the different subsystems of the platform
	 * @param placeObject An object instance implementing this interface
	 * @return An instance of the class implementing this interface, containing
	 * 			exactly the same data of the received bitmapObject parameter,
	 * 			from the viewpoint of the operations defined in this interface
	 */
	virtual shared_ptr<IPlace> newInstance(
			const shared_ptr<IPlace> placeObject) = 0;

	/**
	 * @param name The new name for this IPlace
	 */
	virtual void setName(const string& name) = 0;

	/**
	 * @return The name for this IPlace
	 */
	virtual const string& getName() = 0;

	/**
	 * @param coords The new coordinates for this IPlace
	 */
	virtual void setCoordinates(ILatLng* coords) = 0;

	/**
	 * @return The coordinates for this IPlace
	 */
	virtual ILatLng* getCoordinates() = 0;

	/**
	 * @param description The new description for this IPlace
	 */
	virtual void setDescription(const string& description) = 0;

	/**
	 * @return The description for this IPlace
	 */
	virtual const string& getDescription() = 0;

	/**
	 * @return Whether this IPlace has a description
	 */
	virtual bool hasDescription() = 0;

	/**
	 * @param bitmapObject The new snapshot picture for this IPlace, in the
	 * 						form of a Blob, which might need to be read by
	 * 						means of a binary stream
	 */
	virtual void setSnapshot(const oracle::occi::Blob& bitmapObject) = 0;

	/**
	 * @return The snapshot picture for this IPlace, in the form of a Blob,
	 * 			which might need to be read by means of a binary stream
	 */
	virtual const oracle::occi::Blob& getSnapshot() = 0;

	/**
	 * @return Whether this IPlace has a snapshot picture
	 */
	virtual bool hasSnapshot() = 0;

	virtual ~IPlace() {
	}
};

namespace Ice {

using namespace IceProxy;
using namespace Cardroid::Data;

/**
 * \class Place
 * Domain class implementing a Place for its transmission between systems
 * communicating across an Ice network infrastructure
 */
class Place: public ::cardroid::data::zerocice::PlaceTyp,
		public IPlace, public ::Ice::ObjectFactory {

public:

	Place() {
	}

	/**
	 *  Short constructor
	 */
	Place(const string& n, const ::cardroid::data::zerocice::LatLngTyp& c) {
		name = n;
		coords = c;
	}

	/**
	 * Long constructor
	 */
	Place(const string& n, const ::cardroid::data::zerocice::LatLngTyp& c,
			const string& d, const vector<unsigned char> snapshot) {
		name = n;
		coords = c;
		placeDescription = d;
		placeSnapshotBytes = snapshot;
	}

	Place(const ::cardroid::data::zerocice::PlaceTyp& place) {
		name = place.name;
		coords = place.coords;
		if (place.hasDescription())
			placeDescription = place.getDescription();
		if (place.hasSnapshot())
			placeSnapshotBytes = place.getSnapshotBytes();
	}

	Place(const Place& p) {
		Place& place = const_cast<Place&>(p);
		name = place.name;
		coords = place.coords;
		if (place.hasDescription())
			placeDescription = place.placeDescription.get();
		if (place.hasSnapshot())
			placeSnapshotBytes = place.placeSnapshotBytes.get();
	}

	Place(Place&& place) {
		swap(name, place.name);
		swap(coords, place.coords);
		if (place.hasDescription())
			placeDescription = place.placeDescription.get();
		if (place.hasSnapshot())
			placeSnapshotBytes = place.placeSnapshotBytes.get();
	}

	~Place() {
		delete &coords;
		if (hasSnapshot())
			delete &placeSnapshotBytes;
	}

	Place& operator=(Place place) {
		name = place.name;
		coords = place.coords;
		if (place.hasDescription())
			placeDescription = place.getDescription();
		if (place.hasSnapshot())
			placeSnapshotBytes = place.getSnapshotBytes();
		return *this;
	}

	Place& operator=(Place& place) {
		swap(name, place.name);
		swap(coords, place.coords);
		if (place.hasDescription())
			placeDescription = place.getDescription();
		if (place.hasSnapshot())
			placeSnapshotBytes = place.getSnapshotBytes();
		return *this;
	}

	/**
	 * @return An Ice Identity for this datatype category and the data provided
	 */
	static ::Ice::Identity createIdentity(const string& name, double latitude,
			double longitude) {
		::Ice::Identity id;
		id.category = "place";
		id.name = name + " [" + to_string(latitude) + ", " + to_string(longitude) + "]";
		return id;
	}

	/**
	 * @return A proxy to an Ice Object incarnating the provided data, whose
	 * 			servant is added to adapter 's active servant map
	 */
	static ::cardroid::data::zerocice::PlaceTypPrx createProxy(
			const ::cardroid::data::zerocice::PlaceTyp& placeObject,
			::Ice::ObjectAdapterPtr adapter) {
		::cardroid::data::zerocice::PlaceTypPrx placePrx =
				::cardroid::data::zerocice::PlaceTypPrx::uncheckedCast(
						adapter->createProxy(
								createIdentity(placeObject.getName(),
										placeObject.getCoords().latitude,
										placeObject.getCoords().longitude)));
		fillProxyData(placeObject, placePrx, adapter);
		return placePrx;
	}

	/**
	 * @return A proxy to an Ice Object incarnating the provided data, whose
	 * 			servant is added to adapter 's active servant map
	 */
	static ::cardroid::data::zerocice::PlaceTypPrx createProxy(const IPlace& po,
			::Ice::ObjectAdapterPtr adapter) {
		IPlace& placeObject = const_cast<IPlace&>(po);
		::cardroid::data::zerocice::PlaceTypPrx placePrx =
				::cardroid::data::zerocice::PlaceTypPrx::uncheckedCast(
						adapter->createProxy(
								createIdentity(placeObject.getName(),
										placeObject.getCoordinates()->getLatitude(),
										placeObject.getCoordinates()->getLongitude())));
		fillProxyData(placeObject, placePrx, adapter);
		return placePrx;
	}

	/**
	 * Fill the servant referenced by the given proxy with the data contained
	 * in the given object
	 */
	static void fillProxyData(
			const ::cardroid::data::zerocice::PlaceTyp& object,
			::cardroid::data::zerocice::PlaceTypPrx proxy,
			::Ice::ObjectAdapterPtr adapter) {
		proxy->setName(object.getName());
		proxy->setCoords(object.getCoords());
		if (object.hasDescription())
			proxy->setDescription(object.getDescription());
		if (object.hasSnapshot())
			proxy->setSnapshotBytes(object.getSnapshotBytes());
	}

	/**
	 * Fill the servant referenced by the given proxy with the data contained
	 * in the given object
	 */
	static void fillProxyData(const IPlace& o,
			::cardroid::data::zerocice::PlaceTypPrx proxy,
			::Ice::ObjectAdapterPtr adapter) {
		IPlace& object = const_cast<IPlace&>(o);
		proxy->setName(object.getName());
		proxy->setCoords(const_cast<const LatLng&>
				(*static_cast<LatLng*>(LatLng().newInstance(
						shared_ptr<ILatLng>(object.getCoordinates())).get())));
		if (object.hasDescription())
			proxy->setDescription(object.getDescription());
		if (object.hasSnapshot())
			proxy->setSnapshotBytes(
					::Cardroid::occi::blobToByteArray(object.getSnapshot()));
	}

	/**
	 * @param proxy A proxy to a remote object implementing a Place
	 * @return A local Place object containing the data of the remote object
	 * 			referenced by proxy
	 */
	static shared_ptr<Place> extractObject(
			::cardroid::data::zerocice::PlaceTypPrx proxy) {
		shared_ptr<Place> place = NULL;

		if (!(proxy->hasDescription() && proxy->hasSnapshot())) {
			place = shared_ptr<Place>(
					new Place(proxy->getName(), proxy->getCoords()));
			if (!proxy->hasDescription() && !proxy->hasSnapshot())
				return place;
		}

		if (!(proxy->hasDescription() && proxy->hasSnapshot())) {
			if (proxy->hasDescription())
				place->super::setDescription(proxy->getDescription());
			if (proxy->hasSnapshot())
				place->setSnapshotBytes(proxy->getSnapshotBytes());
		} else
			place = shared_ptr<Place>(
					new Place(proxy->getName(), proxy->getCoords(),
							proxy->getDescription(),
							proxy->getSnapshotBytes()));
		return place;
	}

	/* IPlace interface */

	shared_ptr<IPlace> newInstance(const shared_ptr<IPlace> placeObject) {
		if (!placeObject)
			return NULL;
		if (dynamic_cast<Place*>(placeObject.get()))
			return placeObject;

		shared_ptr<IPlace> place = NULL;
		const string& name = placeObject->getName();
		const LatLng& coords =
				const_cast<const LatLng&>(*static_cast<LatLng*>(LatLng().newInstance(
				shared_ptr<ILatLng>(placeObject->getCoordinates())).get()));
		if (!(placeObject->hasDescription() && placeObject->hasSnapshot())) {
			place = static_pointer_cast<IPlace>(make_shared<Place>(name, coords));
			if (!placeObject->hasDescription() && !placeObject->hasSnapshot())
				return place;
		}

		const string& description = placeObject->getDescription();
		::oracle::occi::Blob snapshot;
		if (placeObject->hasSnapshot()) {
			snapshot = place->getSnapshot();
		}

		if (!(placeObject->hasDescription() && placeObject->hasSnapshot())) {
			if (placeObject->hasDescription())
				place->setDescription(placeObject->getDescription());
			if (placeObject->hasSnapshot())
				place->setSnapshot(placeObject->getSnapshot());
		} else
			place = static_pointer_cast<IPlace>(make_shared<Place>(
					name, coords, description, ::Cardroid::occi::blobToByteArray(snapshot)));
		return place;
	}

	void setName(const string& n) {
		name = n;
	}

	const string& getName() {
		return name;
	}

	void setCoordinates(ILatLng* c) {
		coords = (::cardroid::data::zerocice::LatLngTyp)*static_cast<LatLng*>(LatLng().newInstance(shared_ptr<ILatLng>(c)).get());
	}

	ILatLng* getCoordinates() {
		LatLng* c;
		if (!(c = (LatLng*)&coords))
			c = new LatLng(coords);
		return static_cast<ILatLng*>(c);
	}

	void setDescription(const string& d) {
		placeDescription = d;
	}

	const string& getDescription() {
		return placeDescription.get();
	}

	bool hasDescription() {
		return placeDescription;
	}

	void setSnapshot(const ::oracle::occi::Blob& bitmapObject) {
		setSnapshotBytes(::Cardroid::occi::blobToByteArray(bitmapObject));
	}

	const ::oracle::occi::Blob& getSnapshot() {
		return ::Cardroid::occi::byteArrayToBlob(getSnapshotBytes());
	}

	bool hasSnapshot() {
		return placeSnapshotBytes;
	}

	/* Overriding superclass */

	static const string& ice_staticId() {
		return super::ice_staticId();
	}

	string getName(const ::Ice::Current& = ::Ice::Current()) const {
		return name;
	}

	void setName(const string& n, const ::Ice::Current& = ::Ice::Current()) {
		name = n;
	}

	::cardroid::data::zerocice::LatLngTyp getCoords(const ::Ice::Current& =
			::Ice::Current()) const {
		return coords;
	}

	void setCoords(const ::cardroid::data::zerocice::LatLngTyp& c,
			const ::Ice::Current& = ::Ice::Current()) {
		coords = c;
	}

	string getDescription(const ::Ice::Current& = ::Ice::Current()) const {
		return hasDescription() ? placeDescription.get() : NULL;
	}

	void setDescription(const string& description, const ::Ice::Current& =
			::Ice::Current()) {
		placeDescription = description;
	}

	bool hasDescription(const ::Ice::Current& = ::Ice::Current()) const {
		return placeDescription;
	}

	vector<unsigned char> getSnapshotBytes(const ::Ice::Current& =
			::Ice::Current()) const {
		return hasSnapshot() ? placeSnapshotBytes.get() : vector<unsigned char>();
	}

	void setSnapshotBytes(const vector<unsigned char> &snapshotBytes,
			const ::Ice::Current& = ::Ice::Current()) {
		placeSnapshotBytes = snapshotBytes;
	}

	bool hasSnapshot(const ::Ice::Current& = ::Ice::Current()) const {
		return placeSnapshotBytes && placeSnapshotBytes.get().size() > 0;
	}

	string toString(const ::Ice::Current& = ::Ice::Current()) const {
		string builder = name;
		builder += (" [" + to_string(coords.latitude));
		builder += (", " + to_string(coords.longitude) + "]");
		return builder;
	}

	/* ObjectFactory interface */

	::Ice::ObjectPtr create(const string& type) {
		if (!type.compare(ice_staticId()))
			return new Place;
		return NULL;
	}

	void destroy() {
	}

private:

	typedef ::cardroid::data::zerocice::PlaceTyp super;
	static const long long int serialVersionUID = -3644730415012574777L;
};
}

namespace Oracle {

using namespace Cardroid::Data;

/**
 * \class Place
 * Domain class implementing a Place for its storage and retrieval from an
 * Oracle database
 */
class Place: public ::PlaceTyp, public IPlace {

public:

	Place() {
	}

	Place(void* ctxOCCI_) :
			super(ctxOCCI_) {
	}

	/**
	 * Long constructor
	 */
	Place(const string& name, LatLng* coords, const string& description,
			const ::oracle::occi::Blob& snapshot) {
		super::setName(name);
		super::setCoords(coords);
		super::setDescription(description);
		super::setSnapshot(snapshot);
	}

	/**
	 * Short constructor
	 */
	Place(const string& name, LatLng* coords) {
		super::setName(name);
		super::setCoords(coords);
	}

	/**
	 * Another constructor
	 */
	Place(const string& name, LatLng* coords,
			const ::oracle::occi::Blob& snapshot) {
		super::setName(name);
		super::setCoords(coords);
		super::setSnapshot(snapshot);
	}

	/**
	 * Another constructor
	 */
	Place(const string& name, LatLng* coords, const string& description) {
		super::setName(name);
		super::setCoords(coords);
		super::setDescription(description);
	}

	Place(const ::PlaceTyp& place) {
		super::setName(place.getName());
		super::setCoords(place.getCoords());
		if (place.getDescription().length() > 0)
			super::setDescription(place.getDescription());
		if (!place.getSnapshot().isNull())
			super::setSnapshot(place.getSnapshot());
	}

	Place(const Place& place) {
		super::setName(((super) place).getName());
		super::setCoords(place.getCoords());
		if (((super) place).getDescription().length() > 0)
			super::setDescription(((super) place).getDescription());
		if (!((super) place).getSnapshot().isNull())
			super::setSnapshot(((super) place).getSnapshot());
	}

	Place(Place&& place) {
		super::setName(((super) place).getName());
		super::setCoords(place.getCoords());
		if (((super) &place).getDescription().length() > 0)
			super::setDescription(((super) &place).getDescription());
		if (!((super) &place).getSnapshot().isNull())
			super::setSnapshot(((super) place).getSnapshot());
	}

	~Place() {
	}

	Place& operator=(Place place) {
		super::setName(((super) place).getName());
		super::setCoords(place.getCoords());
		super::setDescription(
				((super) place).getDescription().length() > 0 ?
						((super) place).getDescription() : NULL);
		if (!((super) place).getSnapshot().isNull())
			super::setSnapshot(((super) place).getSnapshot());
		return *this;
	}

	Place& operator=(Place& place) {
		super::setName(((super) place).getName());
		super::setCoords(place.getCoords());
		super::setDescription(
				((super) place).getDescription().length() > 0 ?
						((super) place).getDescription() : NULL);
		if (((super) place).getSnapshot().length() > 0)
			super::setSnapshot(((super) place).getSnapshot());
		return *this;
	}

	/* IPlace interface */

	shared_ptr<IPlace> newInstance(const shared_ptr<IPlace> placeObject) {
		if (!placeObject)
			return NULL;
		if (dynamic_cast<Place*>(placeObject.get()))
			return placeObject;

		shared_ptr<IPlace> place = NULL;
		LatLng* coords =
				static_cast<LatLng*>(LatLng().newInstance(
				shared_ptr<ILatLng>(placeObject->getCoordinates())).get());

		if (!placeObject->hasDescription() && !placeObject->hasSnapshot())
			return place = static_pointer_cast<IPlace>(make_shared<Place>(
					 placeObject->getName(), coords));
		if (placeObject->hasDescription() && !placeObject->hasSnapshot())
			return static_pointer_cast<IPlace>(place = make_shared<Place>(
					placeObject->getName(), coords,
							placeObject->getDescription()));
		if (!placeObject->hasDescription() && placeObject->hasSnapshot())
			return place = static_pointer_cast<IPlace>(make_shared<Place>(
					placeObject->getName(), coords,
							placeObject->getSnapshot()));
		if (placeObject->hasDescription() && placeObject->hasSnapshot())
			return place = static_pointer_cast<IPlace>(make_shared<Place>(
					placeObject->getName(), coords,
							placeObject->getDescription(),
							placeObject->getSnapshot()));
		return place;
	}

	void setName(const string& name) {
		try {
			super::setName(name);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	const string& getName() {
		const string& n = "";
		try {
			const string& name = super::getName();
			return name;
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return n;
	}

	void setCoordinates(ILatLng* coords) {
		setCoords(static_pointer_cast<LatLng>(
				LatLng().newInstance(shared_ptr<ILatLng>(coords))).get());
	}

	ILatLng* getCoordinates() {
		return static_cast<ILatLng*>(getCoords());
	}

	void setDescription(const string& description) {
		try {
			super::setDescription(description);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	const string& getDescription() {
		const string& d = "";
		try {
			const string& description = super::getDescription();
			return description;
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return d;
	}

	bool hasDescription() {
		return getDescription().length() > 0;
	}

	void setSnapshot(const ::oracle::occi::Blob& snapshot) {
		try {
			super::setSnapshot(snapshot);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	const ::oracle::occi::Blob& getSnapshot() {
		::oracle::occi::Blob s = ::oracle::occi::Blob();
		s.setNull();
		const ::oracle::occi::Blob& ss = const_cast<const ::oracle::occi::Blob&>(s);
		try {
			const ::oracle::occi::Blob& snapshot = super::getSnapshot();
			return snapshot;
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return ss;
	}

	bool hasSnapshot() {
		return getSnapshot() != NULL;
	}

	/* Overriding superclass */

	string getName() const {
		string name = "";
		try {
			name = super::getName();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return name;
	}

	LatLng* getCoords() const {
		LatLng* coords = NULL;
		try {
			coords = super::getCoords();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return coords;
	}

	void setCoords(LatLng* value) {
		try {
			super::setCoords(value);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	string getDescription() const {
		string description = "";
		try {
			description = super::getDescription();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return description;
	}

	::oracle::occi::Blob getSnapshot() const {
		::oracle::occi::Blob snapshot = ::oracle::occi::Blob();
		snapshot.setNull();
		try {
			snapshot = super::getSnapshot();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return snapshot;
	}

	static string getSqlTypeName() {
		return string("ANDROID.PLACE_TYP");
	}

private:

	typedef ::PlaceTyp super;
};
}
}
}

#endif /* IPLACE_H_ */
