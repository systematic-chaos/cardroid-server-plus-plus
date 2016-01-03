#ifndef DBTYPES_ORACLE
# define DBTYPES_ORACLE

#include <occiCommon.h>
#include <occiData.h>
#include <occiObjects.h>
#include <stddef.h>
#include <string>
#include <vector>

#ifndef OCCI_ORACLE
# include <occi.h>
#endif

namespace Cardroid {
namespace Data {
namespace Oracle {
class Bitmap;
class Car;
class Date;
class DateTimePrefs;
class DateTime;
class LatLng;
class Place;
class User;
class Message;
class Passenger;
class Waypoint;
class Trip;
class TripOffer;
class TripRequest;
class UserActivity;
}
}
}

using namespace Cardroid::Data::Oracle;

/************************************************************/
//  generated declarations for the BITMAP_TYP object type.
/************************************************************/

class BitmapTyp: public oracle::occi::PObject {

private:

	oracle::occi::Blob BITMAP;OCCI_STD_NAMESPACE::string COMPRESS_FORMAT;OCCI_STD_NAMESPACE::string CONFIG;
	oracle::occi::Number DENSITY;

public:

	oracle::occi::Blob getBitmap() const;

	void setBitmap(const oracle::occi::Blob &value);

	OCCI_STD_NAMESPACE::string getCompress_format() const;

	void setCompress_format(const OCCI_STD_NAMESPACE::string &value);

	OCCI_STD_NAMESPACE::string getConfig() const;

	void setConfig(const OCCI_STD_NAMESPACE::string &value);

	oracle::occi::Number getDensity() const;

	void setDensity(const oracle::occi::Number &value);

	void *operator new(size_t size);

	void *operator new(size_t size, const oracle::occi::Connection * sess,
			const OCCI_STD_NAMESPACE::string& table);

	void *operator new(size_t, void *ctxOCCI_);

	void *operator new(size_t size, const oracle::occi::Connection *sess,
			const OCCI_STD_NAMESPACE::string &tableName,
			const OCCI_STD_NAMESPACE::string &typeName,
			const OCCI_STD_NAMESPACE::string &tableSchema,
			const OCCI_STD_NAMESPACE::string &typeSchema);

	OCCI_STD_NAMESPACE::string getSQLTypeName() const;

	void getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
			unsigned int &schemaNameLen, void **typeName,
			unsigned int &typeNameLen) const;

	BitmapTyp();

	BitmapTyp(void *ctxOCCI_) :
			oracle::occi::PObject(ctxOCCI_) {
	}
	;

	static void *readSQL(void *ctxOCCI_);

	virtual void readSQL(oracle::occi::AnyData& streamOCCI_);

	static void writeSQL(void *objOCCI_, void *ctxOCCI_);

	virtual void writeSQL(oracle::occi::AnyData& streamOCCI_);

	~BitmapTyp();

};

/************************************************************/
//  generated declarations for the CAR_TYP object type.
/************************************************************/

class CarTyp: public oracle::occi::PObject {

private:

	OCCI_STD_NAMESPACE::string BRAND;OCCI_STD_NAMESPACE::string MODEL;OCCI_STD_NAMESPACE::string FUEL;
	oracle::occi::Number CONSUMPTION_PER_KM;
	oracle::occi::Number N_SEATS;OCCI_STD_NAMESPACE::string COLOR;OCCI_STD_NAMESPACE::string PLATE;

public:

	OCCI_STD_NAMESPACE::string getBrand() const;

	void setBrand(const OCCI_STD_NAMESPACE::string &value);

	OCCI_STD_NAMESPACE::string getModel() const;

	void setModel(const OCCI_STD_NAMESPACE::string &value);

	OCCI_STD_NAMESPACE::string getFuel() const;

	void setFuel(const OCCI_STD_NAMESPACE::string &value);

	oracle::occi::Number getConsumption_per_km() const;

	void setConsumption_per_km(const oracle::occi::Number &value);

	oracle::occi::Number getN_seats() const;

	void setN_seats(const oracle::occi::Number &value);

	OCCI_STD_NAMESPACE::string getColor() const;

	void setColor(const OCCI_STD_NAMESPACE::string &value);

	OCCI_STD_NAMESPACE::string getPlate() const;

	void setPlate(const OCCI_STD_NAMESPACE::string &value);

	void *operator new(size_t size);

	void *operator new(size_t size, const oracle::occi::Connection * sess,
			const OCCI_STD_NAMESPACE::string& table);

	void *operator new(size_t, void *ctxOCCI_);

	void *operator new(size_t size, const oracle::occi::Connection *sess,
			const OCCI_STD_NAMESPACE::string &tableName,
			const OCCI_STD_NAMESPACE::string &typeName,
			const OCCI_STD_NAMESPACE::string &tableSchema,
			const OCCI_STD_NAMESPACE::string &typeSchema);

	OCCI_STD_NAMESPACE::string getSQLTypeName() const;

	void getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
			unsigned int &schemaNameLen, void **typeName,
			unsigned int &typeNameLen) const;

	CarTyp();

	CarTyp(void *ctxOCCI_) :
			oracle::occi::PObject(ctxOCCI_) {
	}
	;

	static void *readSQL(void *ctxOCCI_);

	virtual void readSQL(oracle::occi::AnyData& streamOCCI_);

	static void writeSQL(void *objOCCI_, void *ctxOCCI_);

	virtual void writeSQL(oracle::occi::AnyData& streamOCCI_);

	~CarTyp();

};

/************************************************************/
//  generated declarations for the DATE_TYP object type.
/************************************************************/

class DateTyp: public oracle::occi::PObject {

private:

	oracle::occi::Timestamp DATETIME;

public:

	oracle::occi::Timestamp getDatetime() const;

	void setDatetime(const oracle::occi::Timestamp &value);

	void *operator new(size_t size);

	void *operator new(size_t size, const oracle::occi::Connection * sess,
			const OCCI_STD_NAMESPACE::string& table);

	void *operator new(size_t, void *ctxOCCI_);

	void *operator new(size_t size, const oracle::occi::Connection *sess,
			const OCCI_STD_NAMESPACE::string &tableName,
			const OCCI_STD_NAMESPACE::string &typeName,
			const OCCI_STD_NAMESPACE::string &tableSchema,
			const OCCI_STD_NAMESPACE::string &typeSchema);

	OCCI_STD_NAMESPACE::string getSQLTypeName() const;

	void getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
			unsigned int &schemaNameLen, void **typeName,
			unsigned int &typeNameLen) const;

	DateTyp();

	DateTyp(void *ctxOCCI_) :
			oracle::occi::PObject(ctxOCCI_) {
	}
	;

	static void *readSQL(void *ctxOCCI_);

	virtual void readSQL(oracle::occi::AnyData& streamOCCI_);

	static void writeSQL(void *objOCCI_, void *ctxOCCI_);

	virtual void writeSQL(oracle::occi::AnyData& streamOCCI_);

	~DateTyp();

};

/************************************************************/
//  generated declarations for the DATE_TIME_PREFS_TYP object type.
/************************************************************/

class DateTimePrefsTyp: public DateTyp {

private:

	oracle::occi::Number TOLERANCE_DAYS;OCCI_STD_NAMESPACE::string TIME_PREFERENCES;

public:

	oracle::occi::Number getTolerance_days() const;

	void setTolerance_days(const oracle::occi::Number &value);

	OCCI_STD_NAMESPACE::string getTime_preferences() const;

	void setTime_preferences(const OCCI_STD_NAMESPACE::string &value);

	void *operator new(size_t size);

	void *operator new(size_t size, const oracle::occi::Connection * sess,
			const OCCI_STD_NAMESPACE::string& table);

	void *operator new(size_t, void *ctxOCCI_);

	void *operator new(size_t size, const oracle::occi::Connection *sess,
			const OCCI_STD_NAMESPACE::string &tableName,
			const OCCI_STD_NAMESPACE::string &typeName,
			const OCCI_STD_NAMESPACE::string &tableSchema,
			const OCCI_STD_NAMESPACE::string &typeSchema);

	OCCI_STD_NAMESPACE::string getSQLTypeName() const;

	void getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
			unsigned int &schemaNameLen, void **typeName,
			unsigned int &typeNameLen) const;

	DateTimePrefsTyp();

	DateTimePrefsTyp(void *ctxOCCI_) :
			DateTyp(ctxOCCI_) {
	}
	;

	static void *readSQL(void *ctxOCCI_);

	virtual void readSQL(oracle::occi::AnyData& streamOCCI_);

	static void writeSQL(void *objOCCI_, void *ctxOCCI_);

	virtual void writeSQL(oracle::occi::AnyData& streamOCCI_);

	~DateTimePrefsTyp();

};

/************************************************************/
//  generated declarations for the DATE_TIME_TYP object type.
/************************************************************/

class DateTimeTyp: public DateTyp {

private:

public:

	void *operator new(size_t size);

	void *operator new(size_t size, const oracle::occi::Connection * sess,
			const OCCI_STD_NAMESPACE::string& table);

	void *operator new(size_t, void *ctxOCCI_);

	void *operator new(size_t size, const oracle::occi::Connection *sess,
			const OCCI_STD_NAMESPACE::string &tableName,
			const OCCI_STD_NAMESPACE::string &typeName,
			const OCCI_STD_NAMESPACE::string &tableSchema,
			const OCCI_STD_NAMESPACE::string &typeSchema);

	OCCI_STD_NAMESPACE::string getSQLTypeName() const;

	void getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
			unsigned int &schemaNameLen, void **typeName,
			unsigned int &typeNameLen) const;

	DateTimeTyp();

	DateTimeTyp(void *ctxOCCI_) :
			DateTyp(ctxOCCI_) {
	}
	;

	static void *readSQL(void *ctxOCCI_);

	virtual void readSQL(oracle::occi::AnyData& streamOCCI_);

	static void writeSQL(void *objOCCI_, void *ctxOCCI_);

	virtual void writeSQL(oracle::occi::AnyData& streamOCCI_);

	~DateTimeTyp();

};

/************************************************************/
//  generated declarations for the LATLNG_TYP object type.
/************************************************************/

class LatLngTyp: public oracle::occi::PObject {

private:

	oracle::occi::Number LATITUDE;
	oracle::occi::Number LONGITUDE;

public:

	oracle::occi::Number getLatitude() const;

	void setLatitude(const oracle::occi::Number &value);

	oracle::occi::Number getLongitude() const;

	void setLongitude(const oracle::occi::Number &value);

	void *operator new(size_t size);

	void *operator new(size_t size, const oracle::occi::Connection * sess,
			const OCCI_STD_NAMESPACE::string& table);

	void *operator new(size_t, void *ctxOCCI_);

	void *operator new(size_t size, const oracle::occi::Connection *sess,
			const OCCI_STD_NAMESPACE::string &tableName,
			const OCCI_STD_NAMESPACE::string &typeName,
			const OCCI_STD_NAMESPACE::string &tableSchema,
			const OCCI_STD_NAMESPACE::string &typeSchema);

	OCCI_STD_NAMESPACE::string getSQLTypeName() const;

	void getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
			unsigned int &schemaNameLen, void **typeName,
			unsigned int &typeNameLen) const;

	LatLngTyp();

	LatLngTyp(void *ctxOCCI_) :
			oracle::occi::PObject(ctxOCCI_) {
	}
	;

	static void *readSQL(void *ctxOCCI_);

	virtual void readSQL(oracle::occi::AnyData& streamOCCI_);

	static void writeSQL(void *objOCCI_, void *ctxOCCI_);

	virtual void writeSQL(oracle::occi::AnyData& streamOCCI_);

	~LatLngTyp();

};

/************************************************************/
//  generated declarations for the PLACE_TYP object type.
/************************************************************/

class PlaceTyp: public oracle::occi::PObject {

private:

	OCCI_STD_NAMESPACE::string NAME;
	LatLng * COORDS;OCCI_STD_NAMESPACE::string DESCRIPTION;
	oracle::occi::Blob SNAPSHOT;

public:

	OCCI_STD_NAMESPACE::string getName() const;

	void setName(const OCCI_STD_NAMESPACE::string &value);

	LatLng * getCoords() const;

	void setCoords(LatLng * value);

	OCCI_STD_NAMESPACE::string getDescription() const;

	void setDescription(const OCCI_STD_NAMESPACE::string &value);

	oracle::occi::Blob getSnapshot() const;

	void setSnapshot(const oracle::occi::Blob &value);

	void *operator new(size_t size);

	void *operator new(size_t size, const oracle::occi::Connection * sess,
			const OCCI_STD_NAMESPACE::string& table);

	void *operator new(size_t, void *ctxOCCI_);

	void *operator new(size_t size, const oracle::occi::Connection *sess,
			const OCCI_STD_NAMESPACE::string &tableName,
			const OCCI_STD_NAMESPACE::string &typeName,
			const OCCI_STD_NAMESPACE::string &tableSchema,
			const OCCI_STD_NAMESPACE::string &typeSchema);

	OCCI_STD_NAMESPACE::string getSQLTypeName() const;

	void getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
			unsigned int &schemaNameLen, void **typeName,
			unsigned int &typeNameLen) const;

	PlaceTyp();

	PlaceTyp(void *ctxOCCI_) :
			oracle::occi::PObject(ctxOCCI_) {
	}
	;

	static void *readSQL(void *ctxOCCI_);

	virtual void readSQL(oracle::occi::AnyData& streamOCCI_);

	static void writeSQL(void *objOCCI_, void *ctxOCCI_);

	virtual void writeSQL(oracle::occi::AnyData& streamOCCI_);

	~PlaceTyp();

};

/************************************************************/
//  generated declarations for the USER_TYP object type.
/************************************************************/

class UserTyp: public oracle::occi::PObject {

private:

	OCCI_STD_NAMESPACE::string NAME;OCCI_STD_NAMESPACE::string SURNAME;
	oracle::occi::Blob AVATAR;
	Place * HOME;
	oracle::occi::Number TELEPHONE;OCCI_STD_NAMESPACE::string EMAIL;
	oracle::occi::Number REPUTATION;OCCI_STD_NAMESPACE::vector<Car *> CARS;

public:

	OCCI_STD_NAMESPACE::string getName() const;

	void setName(const OCCI_STD_NAMESPACE::string &value);

	OCCI_STD_NAMESPACE::string getSurname() const;

	void setSurname(const OCCI_STD_NAMESPACE::string &value);

	oracle::occi::Blob getAvatar() const;

	void setAvatar(const oracle::occi::Blob &value);

	Place * getHome() const;

	void setHome(Place * value);

	oracle::occi::Number getTelephone() const;

	void setTelephone(const oracle::occi::Number &value);

	OCCI_STD_NAMESPACE::string getEmail() const;

	void setEmail(const OCCI_STD_NAMESPACE::string &value);

	oracle::occi::Number getReputation() const;

	void setReputation(const oracle::occi::Number &value);

	OCCI_STD_NAMESPACE::vector<Car *>& getCars();

	const OCCI_STD_NAMESPACE::vector<Car *>& getCars() const;

	void setCars(const OCCI_STD_NAMESPACE::vector<Car *> &value);

	void *operator new(size_t size);

	void *operator new(size_t size, const oracle::occi::Connection * sess,
			const OCCI_STD_NAMESPACE::string& table);

	void *operator new(size_t, void *ctxOCCI_);

	void *operator new(size_t size, const oracle::occi::Connection *sess,
			const OCCI_STD_NAMESPACE::string &tableName,
			const OCCI_STD_NAMESPACE::string &typeName,
			const OCCI_STD_NAMESPACE::string &tableSchema,
			const OCCI_STD_NAMESPACE::string &typeSchema);

	OCCI_STD_NAMESPACE::string getSQLTypeName() const;

	void getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
			unsigned int &schemaNameLen, void **typeName,
			unsigned int &typeNameLen) const;

	UserTyp();

	UserTyp(void *ctxOCCI_) :
			oracle::occi::PObject(ctxOCCI_) {
	}
	;

	static void *readSQL(void *ctxOCCI_);

	virtual void readSQL(oracle::occi::AnyData& streamOCCI_);

	static void writeSQL(void *objOCCI_, void *ctxOCCI_);

	virtual void writeSQL(oracle::occi::AnyData& streamOCCI_);

	~UserTyp();

};

/************************************************************/
//  generated declarations for the MESSAGE_TYP object type.
/************************************************************/

class MessageTyp: public oracle::occi::PObject {

private:

	User * USER1;
	User * USER2;OCCI_STD_NAMESPACE::string MESSAGE;
	oracle::occi::Timestamp TIME_STAMP;

public:

	User * getUser1() const;

	void setUser1(User * value);

	User * getUser2() const;

	void setUser2(User * value);

	OCCI_STD_NAMESPACE::string getMessage() const;

	void setMessage(const OCCI_STD_NAMESPACE::string &value);

	oracle::occi::Timestamp getTime_stamp() const;

	void setTime_stamp(const oracle::occi::Timestamp &value);

	void *operator new(size_t size);

	void *operator new(size_t size, const oracle::occi::Connection * sess,
			const OCCI_STD_NAMESPACE::string& table);

	void *operator new(size_t, void *ctxOCCI_);

	void *operator new(size_t size, const oracle::occi::Connection *sess,
			const OCCI_STD_NAMESPACE::string &tableName,
			const OCCI_STD_NAMESPACE::string &typeName,
			const OCCI_STD_NAMESPACE::string &tableSchema,
			const OCCI_STD_NAMESPACE::string &typeSchema);

	OCCI_STD_NAMESPACE::string getSQLTypeName() const;

	void getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
			unsigned int &schemaNameLen, void **typeName,
			unsigned int &typeNameLen) const;

	MessageTyp();

	MessageTyp(void *ctxOCCI_) :
			oracle::occi::PObject(ctxOCCI_) {
	}
	;

	static void *readSQL(void *ctxOCCI_);

	virtual void readSQL(oracle::occi::AnyData& streamOCCI_);

	static void writeSQL(void *objOCCI_, void *ctxOCCI_);

	virtual void writeSQL(oracle::occi::AnyData& streamOCCI_);

	~MessageTyp();

};

/************************************************************/
//  generated declarations for the PASSENGER_TYP object type.
/************************************************************/

class PassengerTyp: public oracle::occi::PObject {

private:

	User * USR;
	oracle::occi::Number N_SEATS;

public:

	User * getUsr() const;

	void setUsr(User * value);

	oracle::occi::Number getN_seats() const;

	void setN_seats(const oracle::occi::Number &value);

	void *operator new(size_t size);

	void *operator new(size_t size, const oracle::occi::Connection * sess,
			const OCCI_STD_NAMESPACE::string& table);

	void *operator new(size_t, void *ctxOCCI_);

	void *operator new(size_t size, const oracle::occi::Connection *sess,
			const OCCI_STD_NAMESPACE::string &tableName,
			const OCCI_STD_NAMESPACE::string &typeName,
			const OCCI_STD_NAMESPACE::string &tableSchema,
			const OCCI_STD_NAMESPACE::string &typeSchema);

	OCCI_STD_NAMESPACE::string getSQLTypeName() const;

	void getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
			unsigned int &schemaNameLen, void **typeName,
			unsigned int &typeNameLen) const;

	PassengerTyp();

	PassengerTyp(void *ctxOCCI_) :
			oracle::occi::PObject(ctxOCCI_) {
	}
	;

	static void *readSQL(void *ctxOCCI_);

	virtual void readSQL(oracle::occi::AnyData& streamOCCI_);

	static void writeSQL(void *objOCCI_, void *ctxOCCI_);

	virtual void writeSQL(oracle::occi::AnyData& streamOCCI_);

	~PassengerTyp();

};

/************************************************************/
//  generated declarations for the WAYPOINT_TYP object type.
/************************************************************/

class WaypointTyp: public oracle::occi::PObject {

private:

	oracle::occi::Number N_ORDER;
	Place * PLACE;

public:

	oracle::occi::Number getN_order() const;

	void setN_order(const oracle::occi::Number &value);

	Place * getPlace() const;

	void setPlace(Place * value);

	void *operator new(size_t size);

	void *operator new(size_t size, const oracle::occi::Connection * sess,
			const OCCI_STD_NAMESPACE::string& table);

	void *operator new(size_t, void *ctxOCCI_);

	void *operator new(size_t size, const oracle::occi::Connection *sess,
			const OCCI_STD_NAMESPACE::string &tableName,
			const OCCI_STD_NAMESPACE::string &typeName,
			const OCCI_STD_NAMESPACE::string &tableSchema,
			const OCCI_STD_NAMESPACE::string &typeSchema);

	OCCI_STD_NAMESPACE::string getSQLTypeName() const;

	void getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
			unsigned int &schemaNameLen, void **typeName,
			unsigned int &typeNameLen) const;

	WaypointTyp();

	WaypointTyp(void *ctxOCCI_) :
			oracle::occi::PObject(ctxOCCI_) {
	}
	;

	static void *readSQL(void *ctxOCCI_);

	virtual void readSQL(oracle::occi::AnyData& streamOCCI_);

	static void writeSQL(void *objOCCI_, void *ctxOCCI_);

	virtual void writeSQL(oracle::occi::AnyData& streamOCCI_);

	~WaypointTyp();

};

/************************************************************/
//  generated declarations for the TRIP_TYP object type.
/************************************************************/

class TripTyp: public oracle::occi::PObject {

private:

	oracle::occi::Number TRIP_ID;
	Place * FROM_PLACE;
	Place * TO_PLACE;
	Date * DATETIME;
	oracle::occi::Number N_SEATS;
	Date * RETURN_DATETIME;OCCI_STD_NAMESPACE::vector<
	OCCI_STD_NAMESPACE::string> WEEK_DAYS;OCCI_STD_NAMESPACE::string PERIODICITY;
	oracle::occi::Number DISTANCE;OCCI_STD_NAMESPACE::string CHARACTERISTICS;
	oracle::occi::Number TRIP_TYPE;

public:

	oracle::occi::Number getTrip_id() const;

	void setTrip_id(const oracle::occi::Number &value);

	Place * getFrom_place() const;

	void setFrom_place(Place * value);

	Place * getTo_place() const;

	void setTo_place(Place * value);

	Date * getDatetime() const;

	void setDatetime(Date * value);

	oracle::occi::Number getN_seats() const;

	void setN_seats(const oracle::occi::Number &value);

	Date * getReturn_datetime() const;

	void setReturn_datetime(Date * value);

	OCCI_STD_NAMESPACE::vector< OCCI_STD_NAMESPACE::string>& getWeek_days();

	const OCCI_STD_NAMESPACE::vector< OCCI_STD_NAMESPACE::string>& getWeek_days() const;

	void setWeek_days(
			const OCCI_STD_NAMESPACE::vector< OCCI_STD_NAMESPACE::string> &value);

	OCCI_STD_NAMESPACE::string getPeriodicity() const;

	void setPeriodicity(const OCCI_STD_NAMESPACE::string &value);

	oracle::occi::Number getDistance() const;

	void setDistance(const oracle::occi::Number &value);

	OCCI_STD_NAMESPACE::string getCharacteristics() const;

	void setCharacteristics(const OCCI_STD_NAMESPACE::string &value);

	oracle::occi::Number getTrip_type() const;

	void setTrip_type(const oracle::occi::Number &value);

	void *operator new(size_t size);

	void *operator new(size_t size, const oracle::occi::Connection * sess,
			const OCCI_STD_NAMESPACE::string& table);

	void *operator new(size_t, void *ctxOCCI_);

	void *operator new(size_t size, const oracle::occi::Connection *sess,
			const OCCI_STD_NAMESPACE::string &tableName,
			const OCCI_STD_NAMESPACE::string &typeName,
			const OCCI_STD_NAMESPACE::string &tableSchema,
			const OCCI_STD_NAMESPACE::string &typeSchema);

	OCCI_STD_NAMESPACE::string getSQLTypeName() const;

	void getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
			unsigned int &schemaNameLen, void **typeName,
			unsigned int &typeNameLen) const;

	TripTyp();

	TripTyp(void *ctxOCCI_) :
			oracle::occi::PObject(ctxOCCI_) {
	}
	;

	static void *readSQL(void *ctxOCCI_);

	virtual void readSQL(oracle::occi::AnyData& streamOCCI_);

	static void writeSQL(void *objOCCI_, void *ctxOCCI_);

	virtual void writeSQL(oracle::occi::AnyData& streamOCCI_);

	~TripTyp();

};

/************************************************************/
//  generated declarations for the TRIP_OFFER_TYP object type.
/************************************************************/

class TripOfferTyp: public TripTyp {

private:

	User * DRIVER;OCCI_STD_NAMESPACE::vector<Waypoint *> WAYPOINTS;
	Car * CAR;OCCI_STD_NAMESPACE::vector<Passenger *> PASSENGERS;
	oracle::occi::Number PRICE;OCCI_STD_NAMESPACE::vector<
	OCCI_STD_NAMESPACE::string> ALLOWED;

public:

	User * getDriver() const;

	void setDriver(User * value);

	OCCI_STD_NAMESPACE::vector<Waypoint *>& getWaypoints();

	const OCCI_STD_NAMESPACE::vector<Waypoint *>& getWaypoints() const;

	void setWaypoints(const OCCI_STD_NAMESPACE::vector<Waypoint *> &value);

	Car * getCar() const;

	void setCar(Car * value);

	OCCI_STD_NAMESPACE::vector<Passenger *>& getPassengers();

	const OCCI_STD_NAMESPACE::vector<Passenger *>& getPassengers() const;

	void setPassengers(const OCCI_STD_NAMESPACE::vector<Passenger *> &value);

	oracle::occi::Number getPrice() const;

	void setPrice(const oracle::occi::Number &value);

	OCCI_STD_NAMESPACE::vector< OCCI_STD_NAMESPACE::string>& getAllowed();

	const OCCI_STD_NAMESPACE::vector< OCCI_STD_NAMESPACE::string>& getAllowed() const;

	void setAllowed(
			const OCCI_STD_NAMESPACE::vector< OCCI_STD_NAMESPACE::string> &value);

	void *operator new(size_t size);

	void *operator new(size_t size, const oracle::occi::Connection * sess,
			const OCCI_STD_NAMESPACE::string& table);

	void *operator new(size_t, void *ctxOCCI_);

	void *operator new(size_t size, const oracle::occi::Connection *sess,
			const OCCI_STD_NAMESPACE::string &tableName,
			const OCCI_STD_NAMESPACE::string &typeName,
			const OCCI_STD_NAMESPACE::string &tableSchema,
			const OCCI_STD_NAMESPACE::string &typeSchema);

	OCCI_STD_NAMESPACE::string getSQLTypeName() const;

	void getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
			unsigned int &schemaNameLen, void **typeName,
			unsigned int &typeNameLen) const;

	TripOfferTyp();

	TripOfferTyp(void *ctxOCCI_) :
			TripTyp(ctxOCCI_) {
	}
	;

	static void *readSQL(void *ctxOCCI_);

	virtual void readSQL(oracle::occi::AnyData& streamOCCI_);

	static void writeSQL(void *objOCCI_, void *ctxOCCI_);

	virtual void writeSQL(oracle::occi::AnyData& streamOCCI_);

	~TripOfferTyp();

};

/************************************************************/
//  generated declarations for the TRIP_REQUEST_TYP object type.
/************************************************************/

class TripRequestTyp: public TripTyp {

private:

	User * REQUESTER;

public:

	User * getRequester() const;

	void setRequester(User * value);

	void *operator new(size_t size);

	void *operator new(size_t size, const oracle::occi::Connection * sess,
			const OCCI_STD_NAMESPACE::string& table);

	void *operator new(size_t, void *ctxOCCI_);

	void *operator new(size_t size, const oracle::occi::Connection *sess,
			const OCCI_STD_NAMESPACE::string &tableName,
			const OCCI_STD_NAMESPACE::string &typeName,
			const OCCI_STD_NAMESPACE::string &tableSchema,
			const OCCI_STD_NAMESPACE::string &typeSchema);

	OCCI_STD_NAMESPACE::string getSQLTypeName() const;

	void getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
			unsigned int &schemaNameLen, void **typeName,
			unsigned int &typeNameLen) const;

	TripRequestTyp();

	TripRequestTyp(void *ctxOCCI_) :
		TripTyp(ctxOCCI_) {
	}
	;

	static void *readSQL(void *ctxOCCI_);

	virtual void readSQL(oracle::occi::AnyData& streamOCCI_);

	static void writeSQL(void *objOCCI_, void *ctxOCCI_);

	virtual void writeSQL(oracle::occi::AnyData& streamOCCI_);

	~TripRequestTyp();

};

/************************************************************/
//  generated declarations for the USER_ACTIVITY_TYP object type.
/************************************************************/

class UserActivityTyp: public oracle::occi::PObject {

private:

	User * ACTIVITY_USER;
	TripOffer * ACTIVITY_TRIP;OCCI_STD_NAMESPACE::string ACTIVITY_TYPE;
	oracle::occi::Timestamp TIME_STAMP;

public:

	User * getActivity_user() const;

	void setActivity_user(User * value);

	TripOffer * getActivity_trip() const;

	void setActivity_trip(TripOffer * value);

	OCCI_STD_NAMESPACE::string getActivity_type() const;

	void setActivity_type(const OCCI_STD_NAMESPACE::string &value);

	oracle::occi::Timestamp getTime_stamp() const;

	void setTime_stamp(const oracle::occi::Timestamp &value);

	void *operator new(size_t size);

	void *operator new(size_t size, const oracle::occi::Connection * sess,
			const OCCI_STD_NAMESPACE::string& table);

	void *operator new(size_t, void *ctxOCCI_);

	void *operator new(size_t size, const oracle::occi::Connection *sess,
			const OCCI_STD_NAMESPACE::string &tableName,
			const OCCI_STD_NAMESPACE::string &typeName,
			const OCCI_STD_NAMESPACE::string &tableSchema,
			const OCCI_STD_NAMESPACE::string &typeSchema);

	OCCI_STD_NAMESPACE::string getSQLTypeName() const;

	void getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
			unsigned int &schemaNameLen, void **typeName,
			unsigned int &typeNameLen) const;

	UserActivityTyp();

	UserActivityTyp(void *ctxOCCI_) :
			oracle::occi::PObject(ctxOCCI_) {
	}
	;

	static void *readSQL(void *ctxOCCI_);

	virtual void readSQL(oracle::occi::AnyData& streamOCCI_);

	static void writeSQL(void *objOCCI_, void *ctxOCCI_);

	virtual void writeSQL(oracle::occi::AnyData& streamOCCI_);

	~UserActivityTyp();

};

#endif
