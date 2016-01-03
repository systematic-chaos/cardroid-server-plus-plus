#ifndef ILATLNG_H_
#define ILATLNG_H_

#include <occiControl.h>
#include <occiData.h>
#include <stddef.h>
#include <algorithm>
#include <iostream>
#include <memory>

#include "Cardroid.h"
#include "DbTypes.h"

using namespace std;

namespace Cardroid {
namespace Data {

/**
 * \interface ILatLng
 * Public operations interface for the implementation of a LatLng object
 */
class ILatLng {

public:

	/**
	 * Create an instance of the class implementing this interface,
	 * from the received parameter, which also implements it.
	 * This method acts as an Abstract Factory, for the sake of the
	 * implementation of the Abstract Factory pattern in the search of
	 * interoperability among the different implementations of this interface
	 * which could exist in the different subsystems of the platform
	 * @param latLngObject An object instance implementing this interface
	 * @return An instance of the class implementing this interface, containing
	 * 			exactly the same data of the received bitmapObject parameter,
	 * 			from the viewpoint of the operations defined in this interface
	 */
	virtual shared_ptr<ILatLng> newInstance(
			const shared_ptr<ILatLng> latLngObject) = 0;

	/**
	 * @param latitude The new latitude for this ILatLng
	 */
	virtual void setLatitude(double latitude) = 0;

	/**
	 * @return The latitude for this ILatLng
	 */
	virtual double getLatitude() = 0;

	/**
	 * @param longitude The new longitude for this ILatLng
	 */
	virtual void setLongitude(double longitude) = 0;

	/**
	 * @return The longitude for this ILatLng
	 */
	virtual double getLongitude() = 0;

	virtual ~ILatLng() {
	}
};

namespace Ice {

using namespace IceProxy;
using namespace Cardroid::Data;

/**
 * \class LatLng
 * Domain class implementing a LatLng for its transmission between systems
 * communicating across an Ice network infrastructure
 */
class LatLng: public ::cardroid::data::zerocice::LatLngTyp, public ILatLng {

public:

	LatLng() {
	}

	/**
	 * Default constructor
	 */
	LatLng(double lat, double lng) {
		latitude = lat;
		longitude = lng;
	}

	LatLng(const ::cardroid::data::zerocice::LatLngTyp& latLng) {
		latitude = latLng.latitude;
		longitude = latLng.longitude;
	}

	LatLng(const LatLng& latLng) {
		latitude = latLng.latitude;
		longitude = latLng.longitude;
	}

	LatLng(LatLng&& latLng) {
		swap(latitude, latLng.latitude);
		swap(longitude, latLng.longitude);
	}

	~LatLng() {
	}

	LatLng& operator=(LatLng latLng) {
		latitude = latLng.latitude;
		longitude = latLng.longitude;
		return *this;
	}

	LatLng& operator=(LatLng& latLng) {
		swap(latitude, latLng.latitude);
		swap(longitude, latLng.longitude);
		return *this;
	}

	/* ILatLng interface */

	shared_ptr<ILatLng> newInstance(const shared_ptr<ILatLng> latLngObject) {
		if (!latLngObject)
			return NULL;
		if (dynamic_cast< ::cardroid::data::zerocice::LatLngTyp*>(latLngObject.get()))
			return latLngObject;
		double latitude = latLngObject->getLatitude();
		double longitude = latLngObject->getLongitude();
		return static_pointer_cast<ILatLng>(make_shared<LatLng>(latitude, longitude));
	}

	void setLatitude(double lat) {
		latitude = lat;
	}

	double getLatitude() {
		return latitude;
	}

	void setLongitude(double lng) {
		longitude = lng;
	}

	double getLongitude() {
		return longitude;
	}

private:

	typedef ::cardroid::data::zerocice::LatLngTyp super;
	static const long long int serialVersionUID = 1665309767560539369L;
};
}

namespace Oracle {

using namespace Cardroid::Data;

/**
 * \class LatLng
 * Domain class implementing a LatLng for its storage and retrieval from an
 * Oracle database
 */
class LatLng: private ::LatLngTyp, public ILatLng {

public:

	LatLng() {
	}

	/**
	 * Default constructor
	 */
	LatLng(double latitude, double longitude) {
		setLatitude(latitude);
		setLongitude(longitude);
	}

	LatLng(const ::LatLngTyp& latLng) {
		super::setLatitude(latLng.getLatitude());
		super::setLongitude(latLng.getLongitude());
	}

	LatLng(const LatLng& latLng) {
		LatLng& ll = const_cast<LatLng&>(latLng);
		setLatitude(ll.getLatitude());
		setLongitude(ll.getLongitude());
	}

	LatLng(LatLng&& latLng) {
		setLatitude(latLng.getLatitude());
		setLongitude(latLng.getLongitude());
	}

	~LatLng() {
	}

	LatLng& operator=(LatLng latLng) {
		setLatitude(latLng.getLatitude());
		setLongitude(latLng.getLongitude());
		return *this;
	}

	LatLng& operator=(LatLng& latLng) {
		setLatitude(latLng.getLatitude());
		setLongitude(latLng.getLongitude());
		return *this;
	}

	/* ILatLng interface */

	shared_ptr<ILatLng> newInstance(const shared_ptr<ILatLng> latLngObject) {
		if (!latLngObject)
			return NULL;
		if (dynamic_cast<LatLng*>(latLngObject.get()))
			return latLngObject;
		return static_pointer_cast<ILatLng>(make_shared<LatLng>(latLngObject->getLatitude(), latLngObject->getLongitude()));
	}

	void setLatitude(double latitude) {
		setLatitude((::oracle::occi::Number) latitude);
	}

	double getLatitude() {
		double latitude = 0.;
		try {
			latitude = super::getLatitude();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return latitude;
	}

	void setLongitude(double longitude) {
		setLongitude((::oracle::occi::Number) longitude);
	}

	double getLongitude() {
		double longitude = 0.;
		try {
			longitude = super::getLongitude();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return longitude;
	}

	/* Overriding superclass */

	::oracle::occi::Number getlatitude() const {
		::oracle::occi::Number latitude = 0.;
		try {
			latitude = super::getLatitude();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return latitude;
	}

	void setLatitude(const ::oracle::occi::Number& value) {
		try {
			super::setLatitude(value);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	::oracle::occi::Number getLongitude() const {
		::oracle::occi::Number longitude = 0.;
		try {
			longitude = super::getLongitude();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return longitude;
	}

	void setLongitude(const ::oracle::occi::Number& value) {
		try {
			super::setLongitude(value);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	static string getSqlTypeName() {
		return string("ANDROID.LATLNG_TYP");
	}

private:

	typedef ::LatLngTyp super;
};
}
}
}

#endif /* ILATLNG_H_ */
