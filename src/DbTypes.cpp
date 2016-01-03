#ifndef DBTYPES_ORACLE
# include "DbTypes.h"
#endif

/*****************************************************************/
//  generated method implementations for the BITMAP_TYP object type.
/*****************************************************************/

#include <occiCommon.h>
#include <occiControl.h>
#include <occiData.h>
#include <occiObjects.h>
#include <stddef.h>
#include <string>
#include <vector>

using namespace Cardroid::Data::Oracle;

oracle::occi::Blob BitmapTyp::getBitmap() const {
	return BITMAP;
}

void BitmapTyp::setBitmap(const oracle::occi::Blob &value) {
	BITMAP = value;
}

OCCI_STD_NAMESPACE::string BitmapTyp::getCompress_format() const {
	return COMPRESS_FORMAT;
}

void BitmapTyp::setCompress_format(const OCCI_STD_NAMESPACE::string &value) {
	COMPRESS_FORMAT = value;
}

OCCI_STD_NAMESPACE::string BitmapTyp::getConfig() const {
	return CONFIG;
}

void BitmapTyp::setConfig(const OCCI_STD_NAMESPACE::string &value) {
	CONFIG = value;
}

oracle::occi::Number BitmapTyp::getDensity() const {
	return DENSITY;
}

void BitmapTyp::setDensity(const oracle::occi::Number &value) {
	DENSITY = value;
}

void *BitmapTyp::operator new(size_t size) {
	return oracle::occi::PObject::operator new(size);
}

void *BitmapTyp::operator new(size_t size,
		const oracle::occi::Connection * sess,
		const OCCI_STD_NAMESPACE::string& table) {
	return oracle::occi::PObject::operator new(size, sess, table,
			(char *) "ANDROID.BITMAP_TYP");
}

void *BitmapTyp::operator new(size_t size, void *ctxOCCI_) {
	return oracle::occi::PObject::operator new(size, ctxOCCI_);
}

void *BitmapTyp::operator new(size_t size, const oracle::occi::Connection *sess,
		const OCCI_STD_NAMESPACE::string &tableName,
		const OCCI_STD_NAMESPACE::string &typeName,
		const OCCI_STD_NAMESPACE::string &tableSchema,
		const OCCI_STD_NAMESPACE::string &typeSchema) {
	return oracle::occi::PObject::operator new(size, sess, tableName, typeName,
			tableSchema, typeSchema);
}

OCCI_STD_NAMESPACE::string BitmapTyp::getSQLTypeName() const {
	return OCCI_STD_NAMESPACE::string("ANDROID.BITMAP_TYP");
}

void BitmapTyp::getSQLTypeName(oracle::occi::Environment *env,
		void **schemaName, unsigned int &schemaNameLen, void **typeName,
		unsigned int &typeNameLen) const {
	PObject::getSQLTypeName(env, &BitmapTyp::readSQL, schemaName, schemaNameLen,
			typeName, typeNameLen);
}

BitmapTyp::BitmapTyp() {
}

void *BitmapTyp::readSQL(void *ctxOCCI_) {
	BitmapTyp *objOCCI_ = new (ctxOCCI_) BitmapTyp(ctxOCCI_);
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (streamOCCI_.isNull())
			objOCCI_->setNull();
		else
			objOCCI_->readSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		delete objOCCI_;
		excep.setErrorCtx(ctxOCCI_);
		return (void *) NULL;
	}
	return (void *) objOCCI_;
}

void BitmapTyp::readSQL(oracle::occi::AnyData& streamOCCI_) {
	BITMAP = streamOCCI_.getBlob();
	COMPRESS_FORMAT = streamOCCI_.getString();
	CONFIG = streamOCCI_.getString();
	DENSITY = streamOCCI_.getNumber();
}

void BitmapTyp::writeSQL(void *objectOCCI_, void *ctxOCCI_) {
	BitmapTyp *objOCCI_ = (BitmapTyp*) objectOCCI_;
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (objOCCI_->isNull())
			streamOCCI_.setNull();
		else
			objOCCI_->writeSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		excep.setErrorCtx(ctxOCCI_);
	}
	return;
}

void BitmapTyp::writeSQL(oracle::occi::AnyData& streamOCCI_) {
	streamOCCI_.setBlob(BITMAP);
	streamOCCI_.setString(COMPRESS_FORMAT);
	streamOCCI_.setString(CONFIG);
	streamOCCI_.setNumber(DENSITY);
}

BitmapTyp::~BitmapTyp() {
}

/*****************************************************************/
//  generated method implementations for the CAR_TYP object type.
/*****************************************************************/

OCCI_STD_NAMESPACE::string CarTyp::getBrand() const {
	return BRAND;
}

void CarTyp::setBrand(const OCCI_STD_NAMESPACE::string &value) {
	BRAND = value;
}

OCCI_STD_NAMESPACE::string CarTyp::getModel() const {
	return MODEL;
}

void CarTyp::setModel(const OCCI_STD_NAMESPACE::string &value) {
	MODEL = value;
}

OCCI_STD_NAMESPACE::string CarTyp::getFuel() const {
	return FUEL;
}

void CarTyp::setFuel(const OCCI_STD_NAMESPACE::string &value) {
	FUEL = value;
}

oracle::occi::Number CarTyp::getConsumption_per_km() const {
	return CONSUMPTION_PER_KM;
}

void CarTyp::setConsumption_per_km(const oracle::occi::Number &value) {
	CONSUMPTION_PER_KM = value;
}

oracle::occi::Number CarTyp::getN_seats() const {
	return N_SEATS;
}

void CarTyp::setN_seats(const oracle::occi::Number &value) {
	N_SEATS = value;
}

OCCI_STD_NAMESPACE::string CarTyp::getColor() const {
	return COLOR;
}

void CarTyp::setColor(const OCCI_STD_NAMESPACE::string &value) {
	COLOR = value;
}

OCCI_STD_NAMESPACE::string CarTyp::getPlate() const {
	return PLATE;
}

void CarTyp::setPlate(const OCCI_STD_NAMESPACE::string &value) {
	PLATE = value;
}

void *CarTyp::operator new(size_t size) {
	return oracle::occi::PObject::operator new(size);
}

void *CarTyp::operator new(size_t size, const oracle::occi::Connection * sess,
		const OCCI_STD_NAMESPACE::string& table) {
	return oracle::occi::PObject::operator new(size, sess, table,
			(char *) "ANDROID.CAR_TYP");
}

void *CarTyp::operator new(size_t size, void *ctxOCCI_) {
	return oracle::occi::PObject::operator new(size, ctxOCCI_);
}

void *CarTyp::operator new(size_t size, const oracle::occi::Connection *sess,
		const OCCI_STD_NAMESPACE::string &tableName,
		const OCCI_STD_NAMESPACE::string &typeName,
		const OCCI_STD_NAMESPACE::string &tableSchema,
		const OCCI_STD_NAMESPACE::string &typeSchema) {
	return oracle::occi::PObject::operator new(size, sess, tableName, typeName,
			tableSchema, typeSchema);
}

OCCI_STD_NAMESPACE::string CarTyp::getSQLTypeName() const {
	return OCCI_STD_NAMESPACE::string("ANDROID.CAR_TYP");
}

void CarTyp::getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
		unsigned int &schemaNameLen, void **typeName,
		unsigned int &typeNameLen) const {
	PObject::getSQLTypeName(env, &CarTyp::readSQL, schemaName, schemaNameLen,
			typeName, typeNameLen);
}

CarTyp::CarTyp() {
}

void *CarTyp::readSQL(void *ctxOCCI_) {
	CarTyp *objOCCI_ = new (ctxOCCI_) CarTyp(ctxOCCI_);
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (streamOCCI_.isNull())
			objOCCI_->setNull();
		else
			objOCCI_->readSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		delete objOCCI_;
		excep.setErrorCtx(ctxOCCI_);
		return (void *) NULL;
	}
	return (void *) objOCCI_;
}

void CarTyp::readSQL(oracle::occi::AnyData& streamOCCI_) {
	BRAND = streamOCCI_.getString();
	MODEL = streamOCCI_.getString();
	FUEL = streamOCCI_.getString();
	CONSUMPTION_PER_KM = streamOCCI_.getNumber();
	N_SEATS = streamOCCI_.getNumber();
	COLOR = streamOCCI_.getString();
	PLATE = streamOCCI_.getString();
}

void CarTyp::writeSQL(void *objectOCCI_, void *ctxOCCI_) {
	CarTyp *objOCCI_ = (CarTyp*) objectOCCI_;
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (objOCCI_->isNull())
			streamOCCI_.setNull();
		else
			objOCCI_->writeSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		excep.setErrorCtx(ctxOCCI_);
	}
	return;
}

void CarTyp::writeSQL(oracle::occi::AnyData& streamOCCI_) {
	streamOCCI_.setString(BRAND);
	streamOCCI_.setString(MODEL);
	streamOCCI_.setString(FUEL);
	streamOCCI_.setNumber(CONSUMPTION_PER_KM);
	streamOCCI_.setNumber(N_SEATS);
	streamOCCI_.setString(COLOR);
	streamOCCI_.setString(PLATE);
}

CarTyp::~CarTyp() {
}

/*****************************************************************/
//  generated method implementations for the DATE_TYP object type.
/*****************************************************************/

oracle::occi::Timestamp DateTyp::getDatetime() const {
	return DATETIME;
}

void DateTyp::setDatetime(const oracle::occi::Timestamp &value) {
	DATETIME = value;
}

void *DateTyp::operator new(size_t size) {
	return oracle::occi::PObject::operator new(size);
}

void *DateTyp::operator new(size_t size, const oracle::occi::Connection * sess,
		const OCCI_STD_NAMESPACE::string& table) {
	return oracle::occi::PObject::operator new(size, sess, table,
			(char *) "ANDROID.DATE_TYP");
}

void *DateTyp::operator new(size_t size, void *ctxOCCI_) {
	return oracle::occi::PObject::operator new(size, ctxOCCI_);
}

void *DateTyp::operator new(size_t size, const oracle::occi::Connection *sess,
		const OCCI_STD_NAMESPACE::string &tableName,
		const OCCI_STD_NAMESPACE::string &typeName,
		const OCCI_STD_NAMESPACE::string &tableSchema,
		const OCCI_STD_NAMESPACE::string &typeSchema) {
	return oracle::occi::PObject::operator new(size, sess, tableName, typeName,
			tableSchema, typeSchema);
}

OCCI_STD_NAMESPACE::string DateTyp::getSQLTypeName() const {
	return OCCI_STD_NAMESPACE::string("ANDROID.DATE_TYP");
}

void DateTyp::getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
		unsigned int &schemaNameLen, void **typeName,
		unsigned int &typeNameLen) const {
	PObject::getSQLTypeName(env, &DateTyp::readSQL, schemaName, schemaNameLen,
			typeName, typeNameLen);
}

DateTyp::DateTyp() {
}

void *DateTyp::readSQL(void *ctxOCCI_) {
	DateTyp *objOCCI_ = new (ctxOCCI_) DateTyp(ctxOCCI_);
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (streamOCCI_.isNull())
			objOCCI_->setNull();
		else
			objOCCI_->readSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		delete objOCCI_;
		excep.setErrorCtx(ctxOCCI_);
		return (void *) NULL;
	}
	return (void *) objOCCI_;
}

void DateTyp::readSQL(oracle::occi::AnyData& streamOCCI_) {
	DATETIME = streamOCCI_.getTimestamp();
}

void DateTyp::writeSQL(void *objectOCCI_, void *ctxOCCI_) {
	DateTyp *objOCCI_ = (DateTyp*) objectOCCI_;
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (objOCCI_->isNull())
			streamOCCI_.setNull();
		else
			objOCCI_->writeSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		excep.setErrorCtx(ctxOCCI_);
	}
	return;
}

void DateTyp::writeSQL(oracle::occi::AnyData& streamOCCI_) {
	streamOCCI_.setTimestamp(DATETIME);
}

DateTyp::~DateTyp() {
}

/*****************************************************************/
//  generated method implementations for the DATE_TIME_PREFS_TYP object type.
/*****************************************************************/

oracle::occi::Number DateTimePrefsTyp::getTolerance_days() const {
	return TOLERANCE_DAYS;
}

void DateTimePrefsTyp::setTolerance_days(const oracle::occi::Number &value) {
	TOLERANCE_DAYS = value;
}

OCCI_STD_NAMESPACE::string DateTimePrefsTyp::getTime_preferences() const {
	return TIME_PREFERENCES;
}

void DateTimePrefsTyp::setTime_preferences(
		const OCCI_STD_NAMESPACE::string &value) {
	TIME_PREFERENCES = value;
}

void *DateTimePrefsTyp::operator new(size_t size) {
	return oracle::occi::PObject::operator new(size);
}

void *DateTimePrefsTyp::operator new(size_t size,
		const oracle::occi::Connection * sess,
		const OCCI_STD_NAMESPACE::string& table) {
	return oracle::occi::PObject::operator new(size, sess, table,
			(char *) "ANDROID.DATE_TIME_PREFS_TYP");
}

void *DateTimePrefsTyp::operator new(size_t size, void *ctxOCCI_) {
	return oracle::occi::PObject::operator new(size, ctxOCCI_);
}

void *DateTimePrefsTyp::operator new(size_t size,
		const oracle::occi::Connection *sess,
		const OCCI_STD_NAMESPACE::string &tableName,
		const OCCI_STD_NAMESPACE::string &typeName,
		const OCCI_STD_NAMESPACE::string &tableSchema,
		const OCCI_STD_NAMESPACE::string &typeSchema) {
	return oracle::occi::PObject::operator new(size, sess, tableName, typeName,
			tableSchema, typeSchema);
}

OCCI_STD_NAMESPACE::string DateTimePrefsTyp::getSQLTypeName() const {
	return OCCI_STD_NAMESPACE::string("ANDROID.DATE_TIME_PREFS_TYP");
}

void DateTimePrefsTyp::getSQLTypeName(oracle::occi::Environment *env,
		void **schemaName, unsigned int &schemaNameLen, void **typeName,
		unsigned int &typeNameLen) const {
	PObject::getSQLTypeName(env, &DateTimePrefsTyp::readSQL, schemaName,
			schemaNameLen, typeName, typeNameLen);
}

DateTimePrefsTyp::DateTimePrefsTyp() {
}

void *DateTimePrefsTyp::readSQL(void *ctxOCCI_) {
	DateTimePrefsTyp *objOCCI_ = new (ctxOCCI_) DateTimePrefsTyp(ctxOCCI_);
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (streamOCCI_.isNull())
			objOCCI_->setNull();
		else
			objOCCI_->readSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		delete objOCCI_;
		excep.setErrorCtx(ctxOCCI_);
		return (void *) NULL;
	}
	return (void *) objOCCI_;
}

void DateTimePrefsTyp::readSQL(oracle::occi::AnyData& streamOCCI_) {
	DateTyp::readSQL(streamOCCI_);
	TOLERANCE_DAYS = streamOCCI_.getNumber();
	TIME_PREFERENCES = streamOCCI_.getString();
}

void DateTimePrefsTyp::writeSQL(void *objectOCCI_, void *ctxOCCI_) {
	DateTimePrefsTyp *objOCCI_ = (DateTimePrefsTyp*) objectOCCI_;
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (objOCCI_->isNull())
			streamOCCI_.setNull();
		else
			objOCCI_->writeSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		excep.setErrorCtx(ctxOCCI_);
	}
	return;
}

void DateTimePrefsTyp::writeSQL(oracle::occi::AnyData& streamOCCI_) {
	DateTyp::writeSQL(streamOCCI_);
	streamOCCI_.setNumber(TOLERANCE_DAYS);
	streamOCCI_.setString(TIME_PREFERENCES);
}

DateTimePrefsTyp::~DateTimePrefsTyp() {
}

/*****************************************************************/
//  generated method implementations for the DATE_TIME_TYP object type.
/*****************************************************************/

void *DateTimeTyp::operator new(size_t size) {
	return oracle::occi::PObject::operator new(size);
}

void *DateTimeTyp::operator new(size_t size,
		const oracle::occi::Connection * sess,
		const OCCI_STD_NAMESPACE::string& table) {
	return oracle::occi::PObject::operator new(size, sess, table,
			(char *) "ANDROID.DATE_TIME_TYP");
}

void *DateTimeTyp::operator new(size_t size, void *ctxOCCI_) {
	return oracle::occi::PObject::operator new(size, ctxOCCI_);
}

void *DateTimeTyp::operator new(size_t size,
		const oracle::occi::Connection *sess,
		const OCCI_STD_NAMESPACE::string &tableName,
		const OCCI_STD_NAMESPACE::string &typeName,
		const OCCI_STD_NAMESPACE::string &tableSchema,
		const OCCI_STD_NAMESPACE::string &typeSchema) {
	return oracle::occi::PObject::operator new(size, sess, tableName, typeName,
			tableSchema, typeSchema);
}

OCCI_STD_NAMESPACE::string DateTimeTyp::getSQLTypeName() const {
	return OCCI_STD_NAMESPACE::string("ANDROID.DATE_TIME_TYP");
}

void DateTimeTyp::getSQLTypeName(oracle::occi::Environment *env,
		void **schemaName, unsigned int &schemaNameLen, void **typeName,
		unsigned int &typeNameLen) const {
	PObject::getSQLTypeName(env, &DateTimeTyp::readSQL, schemaName,
			schemaNameLen, typeName, typeNameLen);
}

DateTimeTyp::DateTimeTyp() {
}

void *DateTimeTyp::readSQL(void *ctxOCCI_) {
	DateTimeTyp *objOCCI_ = new (ctxOCCI_) DateTimeTyp(ctxOCCI_);
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (streamOCCI_.isNull())
			objOCCI_->setNull();
		else
			objOCCI_->readSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		delete objOCCI_;
		excep.setErrorCtx(ctxOCCI_);
		return (void *) NULL;
	}
	return (void *) objOCCI_;
}

void DateTimeTyp::readSQL(oracle::occi::AnyData& streamOCCI_) {
	DateTyp::readSQL(streamOCCI_);
}

void DateTimeTyp::writeSQL(void *objectOCCI_, void *ctxOCCI_) {
	DateTimeTyp *objOCCI_ = (DateTimeTyp*) objectOCCI_;
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (objOCCI_->isNull())
			streamOCCI_.setNull();
		else
			objOCCI_->writeSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		excep.setErrorCtx(ctxOCCI_);
	}
	return;
}

void DateTimeTyp::writeSQL(oracle::occi::AnyData& streamOCCI_) {
	DateTyp::writeSQL(streamOCCI_);
}

DateTimeTyp::~DateTimeTyp() {
}

/*****************************************************************/
//  generated method implementations for the LATLNG_TYP object type.
/*****************************************************************/

oracle::occi::Number LatLngTyp::getLatitude() const {
	return LATITUDE;
}

void LatLngTyp::setLatitude(const oracle::occi::Number &value) {
	LATITUDE = value;
}

oracle::occi::Number LatLngTyp::getLongitude() const {
	return LONGITUDE;
}

void LatLngTyp::setLongitude(const oracle::occi::Number &value) {
	LONGITUDE = value;
}

void *LatLngTyp::operator new(size_t size) {
	return oracle::occi::PObject::operator new(size);
}

void *LatLngTyp::operator new(size_t size,
		const oracle::occi::Connection * sess,
		const OCCI_STD_NAMESPACE::string& table) {
	return oracle::occi::PObject::operator new(size, sess, table,
			(char *) "ANDROID.LATLNG_TYP");
}

void *LatLngTyp::operator new(size_t size, void *ctxOCCI_) {
	return oracle::occi::PObject::operator new(size, ctxOCCI_);
}

void *LatLngTyp::operator new(size_t size, const oracle::occi::Connection *sess,
		const OCCI_STD_NAMESPACE::string &tableName,
		const OCCI_STD_NAMESPACE::string &typeName,
		const OCCI_STD_NAMESPACE::string &tableSchema,
		const OCCI_STD_NAMESPACE::string &typeSchema) {
	return oracle::occi::PObject::operator new(size, sess, tableName, typeName,
			tableSchema, typeSchema);
}

OCCI_STD_NAMESPACE::string LatLngTyp::getSQLTypeName() const {
	return OCCI_STD_NAMESPACE::string("ANDROID.LATLNG_TYP");
}

void LatLngTyp::getSQLTypeName(oracle::occi::Environment *env,
		void **schemaName, unsigned int &schemaNameLen, void **typeName,
		unsigned int &typeNameLen) const {
	PObject::getSQLTypeName(env, &LatLngTyp::readSQL, schemaName, schemaNameLen,
			typeName, typeNameLen);
}

LatLngTyp::LatLngTyp() {
}

void *LatLngTyp::readSQL(void *ctxOCCI_) {
	LatLngTyp *objOCCI_ = new (ctxOCCI_) LatLngTyp(ctxOCCI_);
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (streamOCCI_.isNull())
			objOCCI_->setNull();
		else
			objOCCI_->readSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		delete objOCCI_;
		excep.setErrorCtx(ctxOCCI_);
		return (void *) NULL;
	}
	return (void *) objOCCI_;
}

void LatLngTyp::readSQL(oracle::occi::AnyData& streamOCCI_) {
	LATITUDE = streamOCCI_.getNumber();
	LONGITUDE = streamOCCI_.getNumber();
}

void LatLngTyp::writeSQL(void *objectOCCI_, void *ctxOCCI_) {
	LatLngTyp *objOCCI_ = (LatLngTyp*) objectOCCI_;
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (objOCCI_->isNull())
			streamOCCI_.setNull();
		else
			objOCCI_->writeSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		excep.setErrorCtx(ctxOCCI_);
	}
	return;
}

void LatLngTyp::writeSQL(oracle::occi::AnyData& streamOCCI_) {
	streamOCCI_.setNumber(LATITUDE);
	streamOCCI_.setNumber(LONGITUDE);
}

LatLngTyp::~LatLngTyp() {
}

/*****************************************************************/
//  generated method implementations for the PLACE_TYP object type.
/*****************************************************************/

OCCI_STD_NAMESPACE::string PlaceTyp::getName() const {
	return NAME;
}

void PlaceTyp::setName(const OCCI_STD_NAMESPACE::string &value) {
	NAME = value;
}

LatLng * PlaceTyp::getCoords() const {
	return COORDS;
}

void PlaceTyp::setCoords(LatLng * value) {
	COORDS = value;
}

OCCI_STD_NAMESPACE::string PlaceTyp::getDescription() const {
	return DESCRIPTION;
}

void PlaceTyp::setDescription(const OCCI_STD_NAMESPACE::string &value) {
	DESCRIPTION = value;
}

oracle::occi::Blob PlaceTyp::getSnapshot() const {
	return SNAPSHOT;
}

void PlaceTyp::setSnapshot(const oracle::occi::Blob &value) {
	SNAPSHOT = value;
}

void *PlaceTyp::operator new(size_t size) {
	return oracle::occi::PObject::operator new(size);
}

void *PlaceTyp::operator new(size_t size, const oracle::occi::Connection * sess,
		const OCCI_STD_NAMESPACE::string& table) {
	return oracle::occi::PObject::operator new(size, sess, table,
			(char *) "ANDROID.PLACE_TYP");
}

void *PlaceTyp::operator new(size_t size, void *ctxOCCI_) {
	return oracle::occi::PObject::operator new(size, ctxOCCI_);
}

void *PlaceTyp::operator new(size_t size, const oracle::occi::Connection *sess,
		const OCCI_STD_NAMESPACE::string &tableName,
		const OCCI_STD_NAMESPACE::string &typeName,
		const OCCI_STD_NAMESPACE::string &tableSchema,
		const OCCI_STD_NAMESPACE::string &typeSchema) {
	return oracle::occi::PObject::operator new(size, sess, tableName, typeName,
			tableSchema, typeSchema);
}

OCCI_STD_NAMESPACE::string PlaceTyp::getSQLTypeName() const {
	return OCCI_STD_NAMESPACE::string("ANDROID.PLACE_TYP");
}

void PlaceTyp::getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
		unsigned int &schemaNameLen, void **typeName,
		unsigned int &typeNameLen) const {
	PObject::getSQLTypeName(env, &PlaceTyp::readSQL, schemaName, schemaNameLen,
			typeName, typeNameLen);
}

PlaceTyp::PlaceTyp() {
	COORDS = (LatLng*) 0;
}

void *PlaceTyp::readSQL(void *ctxOCCI_) {
	PlaceTyp *objOCCI_ = new (ctxOCCI_) PlaceTyp(ctxOCCI_);
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (streamOCCI_.isNull())
			objOCCI_->setNull();
		else
			objOCCI_->readSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		delete objOCCI_;
		excep.setErrorCtx(ctxOCCI_);
		return (void *) NULL;
	}
	return (void *) objOCCI_;
}

void PlaceTyp::readSQL(oracle::occi::AnyData& streamOCCI_) {
	NAME = streamOCCI_.getString();
	COORDS = (LatLng*) streamOCCI_.getObject(&LatLngTyp::readSQL);
	DESCRIPTION = streamOCCI_.getString();
	SNAPSHOT = streamOCCI_.getBlob();
}

void PlaceTyp::writeSQL(void *objectOCCI_, void *ctxOCCI_) {
	PlaceTyp *objOCCI_ = (PlaceTyp*) objectOCCI_;
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (objOCCI_->isNull())
			streamOCCI_.setNull();
		else
			objOCCI_->writeSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		excep.setErrorCtx(ctxOCCI_);
	}
	return;
}

void PlaceTyp::writeSQL(oracle::occi::AnyData& streamOCCI_) {
	streamOCCI_.setString(NAME);
	streamOCCI_.setObject((LatLngTyp*) COORDS);
	streamOCCI_.setString(DESCRIPTION);
	streamOCCI_.setBlob(SNAPSHOT);
}

PlaceTyp::~PlaceTyp() {
	delete (LatLngTyp*) COORDS;
}

/*****************************************************************/
//  generated method implementations for the USER_TYP object type.
/*****************************************************************/

OCCI_STD_NAMESPACE::string UserTyp::getName() const {
	return NAME;
}

void UserTyp::setName(const OCCI_STD_NAMESPACE::string &value) {
	NAME = value;
}

OCCI_STD_NAMESPACE::string UserTyp::getSurname() const {
	return SURNAME;
}

void UserTyp::setSurname(const OCCI_STD_NAMESPACE::string &value) {
	SURNAME = value;
}

oracle::occi::Blob UserTyp::getAvatar() const {
	return AVATAR;
}

void UserTyp::setAvatar(const oracle::occi::Blob &value) {
	AVATAR = value;
}

Place * UserTyp::getHome() const {
	return HOME;
}

void UserTyp::setHome(Place * value) {
	HOME = value;
}

oracle::occi::Number UserTyp::getTelephone() const {
	return TELEPHONE;
}

void UserTyp::setTelephone(const oracle::occi::Number &value) {
	TELEPHONE = value;
}

OCCI_STD_NAMESPACE::string UserTyp::getEmail() const {
	return EMAIL;
}

void UserTyp::setEmail(const OCCI_STD_NAMESPACE::string &value) {
	EMAIL = value;
}

oracle::occi::Number UserTyp::getReputation() const {
	return REPUTATION;
}

void UserTyp::setReputation(const oracle::occi::Number &value) {
	REPUTATION = value;
}

OCCI_STD_NAMESPACE::vector<Car *>& UserTyp::getCars() {
	return CARS;
}

const OCCI_STD_NAMESPACE::vector<Car *>& UserTyp::getCars() const {
	return CARS;
}

void UserTyp::setCars(const OCCI_STD_NAMESPACE::vector<Car *> &value) {
	CARS = value;
}

void *UserTyp::operator new(size_t size) {
	return oracle::occi::PObject::operator new(size);
}

void *UserTyp::operator new(size_t size, const oracle::occi::Connection * sess,
		const OCCI_STD_NAMESPACE::string& table) {
	return oracle::occi::PObject::operator new(size, sess, table,
			(char *) "ANDROID.USER_TYP");
}

void *UserTyp::operator new(size_t size, void *ctxOCCI_) {
	return oracle::occi::PObject::operator new(size, ctxOCCI_);
}

void *UserTyp::operator new(size_t size, const oracle::occi::Connection *sess,
		const OCCI_STD_NAMESPACE::string &tableName,
		const OCCI_STD_NAMESPACE::string &typeName,
		const OCCI_STD_NAMESPACE::string &tableSchema,
		const OCCI_STD_NAMESPACE::string &typeSchema) {
	return oracle::occi::PObject::operator new(size, sess, tableName, typeName,
			tableSchema, typeSchema);
}

OCCI_STD_NAMESPACE::string UserTyp::getSQLTypeName() const {
	return OCCI_STD_NAMESPACE::string("ANDROID.USER_TYP");
}

void UserTyp::getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
		unsigned int &schemaNameLen, void **typeName,
		unsigned int &typeNameLen) const {
	PObject::getSQLTypeName(env, &UserTyp::readSQL, schemaName, schemaNameLen,
			typeName, typeNameLen);
}

UserTyp::UserTyp() {
	HOME = (Place*) 0;
}

void *UserTyp::readSQL(void *ctxOCCI_) {
	UserTyp *objOCCI_ = new (ctxOCCI_) UserTyp(ctxOCCI_);
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (streamOCCI_.isNull())
			objOCCI_->setNull();
		else
			objOCCI_->readSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		delete objOCCI_;
		excep.setErrorCtx(ctxOCCI_);
		return (void *) NULL;
	}
	return (void *) objOCCI_;
}

void UserTyp::readSQL(oracle::occi::AnyData& streamOCCI_) {
	NAME = streamOCCI_.getString();
	SURNAME = streamOCCI_.getString();
	AVATAR = streamOCCI_.getBlob();
	HOME = (Place*) streamOCCI_.getObject(&PlaceTyp::readSQL);
	TELEPHONE = streamOCCI_.getNumber();
	EMAIL = streamOCCI_.getString();
	REPUTATION = streamOCCI_.getNumber();
	oracle::occi::getVector(streamOCCI_, CARS, &CarTyp::readSQL);
}

void UserTyp::writeSQL(void *objectOCCI_, void *ctxOCCI_) {
	UserTyp *objOCCI_ = (UserTyp*) objectOCCI_;
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (objOCCI_->isNull())
			streamOCCI_.setNull();
		else
			objOCCI_->writeSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		excep.setErrorCtx(ctxOCCI_);
	}
	return;
}

void UserTyp::writeSQL(oracle::occi::AnyData& streamOCCI_) {
	streamOCCI_.setString(NAME);
	streamOCCI_.setString(SURNAME);
	streamOCCI_.setBlob(AVATAR);
	streamOCCI_.setObject((PlaceTyp*) HOME);
	streamOCCI_.setNumber(TELEPHONE);
	streamOCCI_.setString(EMAIL);
	streamOCCI_.setNumber(REPUTATION);
	OCCI_STD_NAMESPACE::vector<CarTyp*> cars;
	for (auto c = CARS.begin(); c != CARS.end(); c++)
		cars.push_back((CarTyp*)*c);
	oracle::occi::setVector(streamOCCI_,
			(OCCI_STD_NAMESPACE::vector<CarTyp*>&) cars);
}

UserTyp::~UserTyp() {
	delete (PlaceTyp*) HOME;
	for (unsigned int i = 0; i < CARS.size(); i++)
		delete (CarTyp*) CARS[i];
}

/*****************************************************************/
//  generated method implementations for the MESSAGE_TYP object type.
/*****************************************************************/

User * MessageTyp::getUser1() const {
	return USER1;
}

void MessageTyp::setUser1(User * value) {
	USER1 = value;
}

User * MessageTyp::getUser2() const {
	return USER2;
}

void MessageTyp::setUser2(User * value) {
	USER2 = value;
}

OCCI_STD_NAMESPACE::string MessageTyp::getMessage() const {
	return MESSAGE;
}

void MessageTyp::setMessage(const OCCI_STD_NAMESPACE::string &value) {
	MESSAGE = value;
}

oracle::occi::Timestamp MessageTyp::getTime_stamp() const {
	return TIME_STAMP;
}

void MessageTyp::setTime_stamp(const oracle::occi::Timestamp &value) {
	TIME_STAMP = value;
}

void *MessageTyp::operator new(size_t size) {
	return oracle::occi::PObject::operator new(size);
}

void *MessageTyp::operator new(size_t size,
		const oracle::occi::Connection * sess,
		const OCCI_STD_NAMESPACE::string& table) {
	return oracle::occi::PObject::operator new(size, sess, table,
			(char *) "ANDROID.MESSAGE_TYP");
}

void *MessageTyp::operator new(size_t size, void *ctxOCCI_) {
	return oracle::occi::PObject::operator new(size, ctxOCCI_);
}

void *MessageTyp::operator new(size_t size,
		const oracle::occi::Connection *sess,
		const OCCI_STD_NAMESPACE::string &tableName,
		const OCCI_STD_NAMESPACE::string &typeName,
		const OCCI_STD_NAMESPACE::string &tableSchema,
		const OCCI_STD_NAMESPACE::string &typeSchema) {
	return oracle::occi::PObject::operator new(size, sess, tableName, typeName,
			tableSchema, typeSchema);
}

OCCI_STD_NAMESPACE::string MessageTyp::getSQLTypeName() const {
	return OCCI_STD_NAMESPACE::string("ANDROID.MESSAGE_TYP");
}

void MessageTyp::getSQLTypeName(oracle::occi::Environment *env,
		void **schemaName, unsigned int &schemaNameLen, void **typeName,
		unsigned int &typeNameLen) const {
	PObject::getSQLTypeName(env, &MessageTyp::readSQL, schemaName,
			schemaNameLen, typeName, typeNameLen);
}

MessageTyp::MessageTyp() {
	USER1 = (User*) 0;
	USER2 = (User*) 0;
}

void *MessageTyp::readSQL(void *ctxOCCI_) {
	MessageTyp *objOCCI_ = new (ctxOCCI_) MessageTyp(ctxOCCI_);
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (streamOCCI_.isNull())
			objOCCI_->setNull();
		else
			objOCCI_->readSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		delete objOCCI_;
		excep.setErrorCtx(ctxOCCI_);
		return (void *) NULL;
	}
	return (void *) objOCCI_;
}

void MessageTyp::readSQL(oracle::occi::AnyData& streamOCCI_) {
	USER1 = (User*) streamOCCI_.getObject(&UserTyp::readSQL);
	USER2 = (User*) streamOCCI_.getObject(&UserTyp::readSQL);
	MESSAGE = streamOCCI_.getString();
	TIME_STAMP = streamOCCI_.getTimestamp();
}

void MessageTyp::writeSQL(void *objectOCCI_, void *ctxOCCI_) {
	MessageTyp *objOCCI_ = (MessageTyp*) objectOCCI_;
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (objOCCI_->isNull())
			streamOCCI_.setNull();
		else
			objOCCI_->writeSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		excep.setErrorCtx(ctxOCCI_);
	}
	return;
}

void MessageTyp::writeSQL(oracle::occi::AnyData& streamOCCI_) {
	streamOCCI_.setObject((UserTyp*) USER1);
	streamOCCI_.setObject((UserTyp*) USER2);
	streamOCCI_.setString(MESSAGE);
	streamOCCI_.setTimestamp(TIME_STAMP);
}

MessageTyp::~MessageTyp() {
	delete (UserTyp*) USER1;
	delete (UserTyp*) USER2;
}

/*****************************************************************/
//  generated method implementations for the PASSENGER_TYP object type.
/*****************************************************************/

User * PassengerTyp::getUsr() const {
	return USR;
}

void PassengerTyp::setUsr(User * value) {
	USR = value;
}

oracle::occi::Number PassengerTyp::getN_seats() const {
	return N_SEATS;
}

void PassengerTyp::setN_seats(const oracle::occi::Number &value) {
	N_SEATS = value;
}

void *PassengerTyp::operator new(size_t size) {
	return oracle::occi::PObject::operator new(size);
}

void *PassengerTyp::operator new(size_t size,
		const oracle::occi::Connection * sess,
		const OCCI_STD_NAMESPACE::string& table) {
	return oracle::occi::PObject::operator new(size, sess, table,
			(char *) "ANDROID.PASSENGER_TYP");
}

void *PassengerTyp::operator new(size_t size, void *ctxOCCI_) {
	return oracle::occi::PObject::operator new(size, ctxOCCI_);
}

void *PassengerTyp::operator new(size_t size,
		const oracle::occi::Connection *sess,
		const OCCI_STD_NAMESPACE::string &tableName,
		const OCCI_STD_NAMESPACE::string &typeName,
		const OCCI_STD_NAMESPACE::string &tableSchema,
		const OCCI_STD_NAMESPACE::string &typeSchema) {
	return oracle::occi::PObject::operator new(size, sess, tableName, typeName,
			tableSchema, typeSchema);
}

OCCI_STD_NAMESPACE::string PassengerTyp::getSQLTypeName() const {
	return OCCI_STD_NAMESPACE::string("ANDROID.PASSENGER_TYP");
}

void PassengerTyp::getSQLTypeName(oracle::occi::Environment *env,
		void **schemaName, unsigned int &schemaNameLen, void **typeName,
		unsigned int &typeNameLen) const {
	PObject::getSQLTypeName(env, &PassengerTyp::readSQL, schemaName,
			schemaNameLen, typeName, typeNameLen);
}

PassengerTyp::PassengerTyp() {
	USR = (User *) 0;
}

void *PassengerTyp::readSQL(void *ctxOCCI_) {
	PassengerTyp *objOCCI_ = new (ctxOCCI_) PassengerTyp(ctxOCCI_);
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (streamOCCI_.isNull())
			objOCCI_->setNull();
		else
			objOCCI_->readSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		delete objOCCI_;
		excep.setErrorCtx(ctxOCCI_);
		return (void *) NULL;
	}
	return (void *) objOCCI_;
}

void PassengerTyp::readSQL(oracle::occi::AnyData& streamOCCI_) {
	USR = (User *) streamOCCI_.getObject(&UserTyp::readSQL);
	N_SEATS = streamOCCI_.getNumber();
}

void PassengerTyp::writeSQL(void *objectOCCI_, void *ctxOCCI_) {
	PassengerTyp *objOCCI_ = (PassengerTyp *) objectOCCI_;
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (objOCCI_->isNull())
			streamOCCI_.setNull();
		else
			objOCCI_->writeSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		excep.setErrorCtx(ctxOCCI_);
	}
	return;
}

void PassengerTyp::writeSQL(oracle::occi::AnyData& streamOCCI_) {
	streamOCCI_.setObject((UserTyp*) USR);
	streamOCCI_.setNumber(N_SEATS);
}

PassengerTyp::~PassengerTyp() {
	delete (UserTyp*) USR;
}

/*****************************************************************/
//  generated method implementations for the WAYPOINT_TYP object type.
/*****************************************************************/

oracle::occi::Number WaypointTyp::getN_order() const {
	return N_ORDER;
}

void WaypointTyp::setN_order(const oracle::occi::Number &value) {
	N_ORDER = value;
}

Place * WaypointTyp::getPlace() const {
	return PLACE;
}

void WaypointTyp::setPlace(Place * value) {
	PLACE = value;
}

void *WaypointTyp::operator new(size_t size) {
	return oracle::occi::PObject::operator new(size);
}

void *WaypointTyp::operator new(size_t size,
		const oracle::occi::Connection * sess,
		const OCCI_STD_NAMESPACE::string& table) {
	return oracle::occi::PObject::operator new(size, sess, table,
			(char *) "ANDROID.WAYPOINT_TYP");
}

void *WaypointTyp::operator new(size_t size, void *ctxOCCI_) {
	return oracle::occi::PObject::operator new(size, ctxOCCI_);
}

void *WaypointTyp::operator new(size_t size,
		const oracle::occi::Connection *sess,
		const OCCI_STD_NAMESPACE::string &tableName,
		const OCCI_STD_NAMESPACE::string &typeName,
		const OCCI_STD_NAMESPACE::string &tableSchema,
		const OCCI_STD_NAMESPACE::string &typeSchema) {
	return oracle::occi::PObject::operator new(size, sess, tableName, typeName,
			tableSchema, typeSchema);
}

OCCI_STD_NAMESPACE::string WaypointTyp::getSQLTypeName() const {
	return OCCI_STD_NAMESPACE::string("ANDROID.WAYPOINT_TYP");
}

void WaypointTyp::getSQLTypeName(oracle::occi::Environment *env,
		void **schemaName, unsigned int &schemaNameLen, void **typeName,
		unsigned int &typeNameLen) const {
	PObject::getSQLTypeName(env, &WaypointTyp::readSQL, schemaName,
			schemaNameLen, typeName, typeNameLen);
}

WaypointTyp::WaypointTyp() {
	PLACE = (Place *) 0;
}

void *WaypointTyp::readSQL(void *ctxOCCI_) {
	WaypointTyp *objOCCI_ = new (ctxOCCI_) WaypointTyp(ctxOCCI_);
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (streamOCCI_.isNull())
			objOCCI_->setNull();
		else
			objOCCI_->readSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		delete objOCCI_;
		excep.setErrorCtx(ctxOCCI_);
		return (void *) NULL;
	}
	return (void *) objOCCI_;
}

void WaypointTyp::readSQL(oracle::occi::AnyData& streamOCCI_) {
	N_ORDER = streamOCCI_.getNumber();
	PLACE = (Place *) streamOCCI_.getObject(&PlaceTyp::readSQL);
}

void WaypointTyp::writeSQL(void *objectOCCI_, void *ctxOCCI_) {
	WaypointTyp *objOCCI_ = (WaypointTyp *) objectOCCI_;
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (objOCCI_->isNull())
			streamOCCI_.setNull();
		else
			objOCCI_->writeSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		excep.setErrorCtx(ctxOCCI_);
	}
	return;
}

void WaypointTyp::writeSQL(oracle::occi::AnyData& streamOCCI_) {
	streamOCCI_.setNumber(N_ORDER);
	streamOCCI_.setObject((PlaceTyp*) PLACE);
}

WaypointTyp::~WaypointTyp() {
	delete (PlaceTyp*) PLACE;
}

/*****************************************************************/
//  generated method implementations for the TRIP_TYP object type.
/*****************************************************************/

oracle::occi::Number TripTyp::getTrip_id() const {
	return TRIP_ID;
}

void TripTyp::setTrip_id(const oracle::occi::Number &value) {
	TRIP_ID = value;
}

Place * TripTyp::getFrom_place() const {
	return FROM_PLACE;
}

void TripTyp::setFrom_place(Place * value) {
	FROM_PLACE = value;
}

Place * TripTyp::getTo_place() const {
	return TO_PLACE;
}

void TripTyp::setTo_place(Place * value) {
	TO_PLACE = value;
}

Date * TripTyp::getDatetime() const {
	return DATETIME;
}

void TripTyp::setDatetime(Date * value) {
	DATETIME = value;
}

oracle::occi::Number TripTyp::getN_seats() const {
	return N_SEATS;
}

void TripTyp::setN_seats(const oracle::occi::Number &value) {
	N_SEATS = value;
}

Date * TripTyp::getReturn_datetime() const {
	return RETURN_DATETIME;
}

void TripTyp::setReturn_datetime(Date * value) {
	RETURN_DATETIME = value;
}

OCCI_STD_NAMESPACE::vector< OCCI_STD_NAMESPACE::string>& TripTyp::getWeek_days() {
	return WEEK_DAYS;
}

const OCCI_STD_NAMESPACE::vector< OCCI_STD_NAMESPACE::string>& TripTyp::getWeek_days() const {
	return WEEK_DAYS;
}

void TripTyp::setWeek_days(
		const OCCI_STD_NAMESPACE::vector< OCCI_STD_NAMESPACE::string> &value) {
	WEEK_DAYS = value;
}

OCCI_STD_NAMESPACE::string TripTyp::getPeriodicity() const {
	return PERIODICITY;
}

void TripTyp::setPeriodicity(const OCCI_STD_NAMESPACE::string &value) {
	PERIODICITY = value;
}

oracle::occi::Number TripTyp::getDistance() const {
	return DISTANCE;
}

void TripTyp::setDistance(const oracle::occi::Number &value) {
	DISTANCE = value;
}

OCCI_STD_NAMESPACE::string TripTyp::getCharacteristics() const {
	return CHARACTERISTICS;
}

void TripTyp::setCharacteristics(const OCCI_STD_NAMESPACE::string &value) {
	CHARACTERISTICS = value;
}

oracle::occi::Number TripTyp::getTrip_type() const {
	return TRIP_TYPE;
}

void TripTyp::setTrip_type(const oracle::occi::Number &value) {
	TRIP_TYPE = value;
}

void *TripTyp::operator new(size_t size) {
	return oracle::occi::PObject::operator new(size);
}

void *TripTyp::operator new(size_t size, const oracle::occi::Connection * sess,
		const OCCI_STD_NAMESPACE::string& table) {
	return oracle::occi::PObject::operator new(size, sess, table,
			(char *) "ANDROID.TRIP_TYP");
}

void *TripTyp::operator new(size_t size, void *ctxOCCI_) {
	return oracle::occi::PObject::operator new(size, ctxOCCI_);
}

void *TripTyp::operator new(size_t size, const oracle::occi::Connection *sess,
		const OCCI_STD_NAMESPACE::string &tableName,
		const OCCI_STD_NAMESPACE::string &typeName,
		const OCCI_STD_NAMESPACE::string &tableSchema,
		const OCCI_STD_NAMESPACE::string &typeSchema) {
	return oracle::occi::PObject::operator new(size, sess, tableName, typeName,
			tableSchema, typeSchema);
}

OCCI_STD_NAMESPACE::string TripTyp::getSQLTypeName() const {
	return OCCI_STD_NAMESPACE::string("ANDROID.TRIP_TYP");
}

void TripTyp::getSQLTypeName(oracle::occi::Environment *env, void **schemaName,
		unsigned int &schemaNameLen, void **typeName,
		unsigned int &typeNameLen) const {
	PObject::getSQLTypeName(env, &TripTyp::readSQL, schemaName, schemaNameLen,
			typeName, typeNameLen);
}

TripTyp::TripTyp() {
	FROM_PLACE = (Place *) 0;
	TO_PLACE = (Place *) 0;
	DATETIME = (Date *) 0;
	RETURN_DATETIME = (Date *) 0;
}

void *TripTyp::readSQL(void *ctxOCCI_) {
	TripTyp *objOCCI_ = new (ctxOCCI_) TripTyp(ctxOCCI_);
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (streamOCCI_.isNull())
			objOCCI_->setNull();
		else
			objOCCI_->readSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		delete objOCCI_;
		excep.setErrorCtx(ctxOCCI_);
		return (void *) NULL;
	}
	return (void *) objOCCI_;
}

void TripTyp::readSQL(oracle::occi::AnyData& streamOCCI_) {
	TRIP_ID = streamOCCI_.getNumber();
	FROM_PLACE = (Place *) streamOCCI_.getObject(&PlaceTyp::readSQL);
	TO_PLACE = (Place *) streamOCCI_.getObject(&PlaceTyp::readSQL);
	DATETIME = (Date *) streamOCCI_.getObject(&DateTyp::readSQL);
	N_SEATS = streamOCCI_.getNumber();
	RETURN_DATETIME = (Date *) streamOCCI_.getObject(&DateTyp::readSQL);
	oracle::occi::getVector(streamOCCI_, WEEK_DAYS);
	PERIODICITY = streamOCCI_.getString();
	DISTANCE = streamOCCI_.getNumber();
	CHARACTERISTICS = streamOCCI_.getString();
	TRIP_TYPE = streamOCCI_.getNumber();
}

void TripTyp::writeSQL(void *objectOCCI_, void *ctxOCCI_) {
	TripTyp *objOCCI_ = (TripTyp *) objectOCCI_;
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (objOCCI_->isNull())
			streamOCCI_.setNull();
		else
			objOCCI_->writeSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		excep.setErrorCtx(ctxOCCI_);
	}
	return;
}

void TripTyp::writeSQL(oracle::occi::AnyData& streamOCCI_) {
	streamOCCI_.setNumber(TRIP_ID);
	streamOCCI_.setObject((PlaceTyp*) FROM_PLACE);
	streamOCCI_.setObject((PlaceTyp*) TO_PLACE);
	streamOCCI_.setObject((DateTyp*) DATETIME);
	streamOCCI_.setNumber(N_SEATS);
	streamOCCI_.setObject((DateTyp*) RETURN_DATETIME);
	oracle::occi::setVector(streamOCCI_, WEEK_DAYS);
	streamOCCI_.setString(PERIODICITY);
	streamOCCI_.setNumber(DISTANCE);
	streamOCCI_.setString(CHARACTERISTICS);
	streamOCCI_.setNumber(TRIP_TYPE);
}

TripTyp::~TripTyp() {
	delete (PlaceTyp*) FROM_PLACE;
	delete (PlaceTyp*) TO_PLACE;
	delete (DateTyp*) DATETIME;
	delete (DateTyp*) RETURN_DATETIME;
}

/*****************************************************************/
//  generated method implementations for the TRIP_OFFER_TYP object type.
/*****************************************************************/

User * TripOfferTyp::getDriver() const {
	return DRIVER;
}

void TripOfferTyp::setDriver(User * value) {
	DRIVER = value;
}

OCCI_STD_NAMESPACE::vector<Waypoint *>& TripOfferTyp::getWaypoints() {
	return WAYPOINTS;
}

const OCCI_STD_NAMESPACE::vector<Waypoint *>& TripOfferTyp::getWaypoints() const {
	return WAYPOINTS;
}

void TripOfferTyp::setWaypoints(
		const OCCI_STD_NAMESPACE::vector<Waypoint *> &value) {
	WAYPOINTS = value;
}

Car * TripOfferTyp::getCar() const {
	return CAR;
}

void TripOfferTyp::setCar(Car * value) {
	CAR = value;
}

OCCI_STD_NAMESPACE::vector<Passenger *>& TripOfferTyp::getPassengers() {
	return PASSENGERS;
}

const OCCI_STD_NAMESPACE::vector<Passenger *>& TripOfferTyp::getPassengers() const {
	return PASSENGERS;
}

void TripOfferTyp::setPassengers(
		const OCCI_STD_NAMESPACE::vector<Passenger *> &value) {
	PASSENGERS = value;
}

oracle::occi::Number TripOfferTyp::getPrice() const {
	return PRICE;
}

void TripOfferTyp::setPrice(const oracle::occi::Number &value) {
	PRICE = value;
}

OCCI_STD_NAMESPACE::vector< OCCI_STD_NAMESPACE::string>& TripOfferTyp::getAllowed() {
	return ALLOWED;
}

const OCCI_STD_NAMESPACE::vector< OCCI_STD_NAMESPACE::string>& TripOfferTyp::getAllowed() const {
	return ALLOWED;
}

void TripOfferTyp::setAllowed(
		const OCCI_STD_NAMESPACE::vector< OCCI_STD_NAMESPACE::string> &value) {
	ALLOWED = value;
}

void *TripOfferTyp::operator new(size_t size) {
	return oracle::occi::PObject::operator new(size);
}

void *TripOfferTyp::operator new(size_t size,
		const oracle::occi::Connection * sess,
		const OCCI_STD_NAMESPACE::string& table) {
	return oracle::occi::PObject::operator new(size, sess, table,
			(char *) "ANDROID.TRIP_OFFER_TYP");
}

void *TripOfferTyp::operator new(size_t size, void *ctxOCCI_) {
	return oracle::occi::PObject::operator new(size, ctxOCCI_);
}

void *TripOfferTyp::operator new(size_t size,
		const oracle::occi::Connection *sess,
		const OCCI_STD_NAMESPACE::string &tableName,
		const OCCI_STD_NAMESPACE::string &typeName,
		const OCCI_STD_NAMESPACE::string &tableSchema,
		const OCCI_STD_NAMESPACE::string &typeSchema) {
	return oracle::occi::PObject::operator new(size, sess, tableName, typeName,
			tableSchema, typeSchema);
}

OCCI_STD_NAMESPACE::string TripOfferTyp::getSQLTypeName() const {
	return OCCI_STD_NAMESPACE::string("ANDROID.TRIP_OFFER_TYP");
}

void TripOfferTyp::getSQLTypeName(oracle::occi::Environment *env,
		void **schemaName, unsigned int &schemaNameLen, void **typeName,
		unsigned int &typeNameLen) const {
	PObject::getSQLTypeName(env, &TripOfferTyp::readSQL, schemaName,
			schemaNameLen, typeName, typeNameLen);
}

TripOfferTyp::TripOfferTyp() {
	DRIVER = (User *) 0;
	CAR = (Car *) 0;
}

void *TripOfferTyp::readSQL(void *ctxOCCI_) {
	TripOfferTyp *objOCCI_ = new (ctxOCCI_) TripOfferTyp(ctxOCCI_);
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (streamOCCI_.isNull())
			objOCCI_->setNull();
		else
			objOCCI_->readSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		delete objOCCI_;
		excep.setErrorCtx(ctxOCCI_);
		return (void *) NULL;
	}
	return (void *) objOCCI_;
}

void TripOfferTyp::readSQL(oracle::occi::AnyData& streamOCCI_) {
	TripTyp::readSQL(streamOCCI_);
	DRIVER = (User *) streamOCCI_.getObject(&UserTyp::readSQL);
	oracle::occi::getVector(streamOCCI_, WAYPOINTS, &WaypointTyp::readSQL);
	CAR = (Car *) streamOCCI_.getObject(&CarTyp::readSQL);
	oracle::occi::getVector(streamOCCI_, PASSENGERS, &PassengerTyp::readSQL);
	PRICE = streamOCCI_.getNumber();
	oracle::occi::getVector(streamOCCI_, ALLOWED);
}

void TripOfferTyp::writeSQL(void *objectOCCI_, void *ctxOCCI_) {
	TripOfferTyp *objOCCI_ = (TripOfferTyp *) objectOCCI_;
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (objOCCI_->isNull())
			streamOCCI_.setNull();
		else
			objOCCI_->writeSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		excep.setErrorCtx(ctxOCCI_);
	}
	return;
}

void TripOfferTyp::writeSQL(oracle::occi::AnyData& streamOCCI_) {
	TripTyp::writeSQL(streamOCCI_);
	streamOCCI_.setObject((UserTyp*) DRIVER);
	OCCI_STD_NAMESPACE::vector<WaypointTyp*> waypoints;
	for (auto wp = WAYPOINTS.begin(); wp != WAYPOINTS.end(); wp++)
		waypoints.push_back((WaypointTyp*)*wp);
	oracle::occi::setVector(streamOCCI_,
			(OCCI_STD_NAMESPACE::vector<WaypointTyp*>&) waypoints);
	streamOCCI_.setObject((CarTyp*) CAR);
	OCCI_STD_NAMESPACE::vector<PassengerTyp*> passengers;
	for (auto p = PASSENGERS.begin(); p != PASSENGERS.end(); p++)
		passengers.push_back((PassengerTyp*)*p);
	oracle::occi::setVector(streamOCCI_,
			(OCCI_STD_NAMESPACE::vector<PassengerTyp*>&) passengers);
	streamOCCI_.setNumber(PRICE);
	oracle::occi::setVector(streamOCCI_, ALLOWED);
}

TripOfferTyp::~TripOfferTyp() {
	delete (UserTyp*) DRIVER;
	for (unsigned int i = 0; i < WAYPOINTS.size(); i++)
		delete (WaypointTyp*) WAYPOINTS[i];
	delete (CarTyp*) CAR;
	for (unsigned int i = 0; i < PASSENGERS.size(); i++)
		delete (PassengerTyp*) PASSENGERS[i];
}

/*****************************************************************/
//  generated method implementations for the TRIP_REQUEST_TYP object type.
/*****************************************************************/

User * TripRequestTyp::getRequester() const {
	return REQUESTER;
}

void TripRequestTyp::setRequester(User * value) {
	REQUESTER = value;
}

void *TripRequestTyp::operator new(size_t size) {
	return oracle::occi::PObject::operator new(size);
}

void *TripRequestTyp::operator new(size_t size,
		const oracle::occi::Connection * sess,
		const OCCI_STD_NAMESPACE::string& table) {
	return oracle::occi::PObject::operator new(size, sess, table,
			(char *) "ANDROID.TRIP_REQUEST_TYP");
}

void *TripRequestTyp::operator new(size_t size, void *ctxOCCI_) {
	return oracle::occi::PObject::operator new(size, ctxOCCI_);
}

void *TripRequestTyp::operator new(size_t size,
		const oracle::occi::Connection *sess,
		const OCCI_STD_NAMESPACE::string &tableName,
		const OCCI_STD_NAMESPACE::string &typeName,
		const OCCI_STD_NAMESPACE::string &tableSchema,
		const OCCI_STD_NAMESPACE::string &typeSchema) {
	return oracle::occi::PObject::operator new(size, sess, tableName, typeName,
			tableSchema, typeSchema);
}

OCCI_STD_NAMESPACE::string TripRequestTyp::getSQLTypeName() const {
	return OCCI_STD_NAMESPACE::string("ANDROID.TRIP_REQUEST_TYP");
}

void TripRequestTyp::getSQLTypeName(oracle::occi::Environment *env,
		void **schemaName, unsigned int &schemaNameLen, void **typeName,
		unsigned int &typeNameLen) const {
	PObject::getSQLTypeName(env, &TripRequestTyp::readSQL, schemaName,
			schemaNameLen, typeName, typeNameLen);
}

TripRequestTyp::TripRequestTyp() {
	REQUESTER = (User *) 0;
}

void *TripRequestTyp::readSQL(void *ctxOCCI_) {
	TripRequestTyp *objOCCI_ = new (ctxOCCI_) TripRequestTyp(ctxOCCI_);
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (streamOCCI_.isNull())
			objOCCI_->setNull();
		else
			objOCCI_->readSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		delete objOCCI_;
		excep.setErrorCtx(ctxOCCI_);
		return (void *) NULL;
	}
	return (void *) objOCCI_;
}

void TripRequestTyp::readSQL(oracle::occi::AnyData& streamOCCI_) {
	TripTyp::readSQL(streamOCCI_);
	REQUESTER = (User *) streamOCCI_.getObject(&UserTyp::readSQL);
}

void TripRequestTyp::writeSQL(void *objectOCCI_, void *ctxOCCI_) {
	TripRequestTyp *objOCCI_ = (TripRequestTyp *) objectOCCI_;
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (objOCCI_->isNull())
			streamOCCI_.setNull();
		else
			objOCCI_->writeSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		excep.setErrorCtx(ctxOCCI_);
	}
	return;
}

void TripRequestTyp::writeSQL(oracle::occi::AnyData& streamOCCI_) {
	TripTyp::writeSQL(streamOCCI_);
	streamOCCI_.setObject((UserTyp*) REQUESTER);
}

TripRequestTyp::~TripRequestTyp() {
	delete (UserTyp*) REQUESTER;
}

/*****************************************************************/
//  generated method implementations for the USER_ACTIVITY_TYP object type.
/*****************************************************************/

User * UserActivityTyp::getActivity_user() const {
	return ACTIVITY_USER;
}

void UserActivityTyp::setActivity_user(User * value) {
	ACTIVITY_USER = value;
}

TripOffer * UserActivityTyp::getActivity_trip() const {
	return ACTIVITY_TRIP;
}

void UserActivityTyp::setActivity_trip(TripOffer * value) {
	ACTIVITY_TRIP = value;
}

OCCI_STD_NAMESPACE::string UserActivityTyp::getActivity_type() const {
	return ACTIVITY_TYPE;
}

void UserActivityTyp::setActivity_type(
		const OCCI_STD_NAMESPACE::string &value) {
	ACTIVITY_TYPE = value;
}

oracle::occi::Timestamp UserActivityTyp::getTime_stamp() const {
	return TIME_STAMP;
}

void UserActivityTyp::setTime_stamp(const oracle::occi::Timestamp &value) {
	TIME_STAMP = value;
}

void *UserActivityTyp::operator new(size_t size) {
	return oracle::occi::PObject::operator new(size);
}

void *UserActivityTyp::operator new(size_t size,
		const oracle::occi::Connection * sess,
		const OCCI_STD_NAMESPACE::string& table) {
	return oracle::occi::PObject::operator new(size, sess, table,
			(char *) "ANDROID.USER_ACTIVITY_TYP");
}

void *UserActivityTyp::operator new(size_t size, void *ctxOCCI_) {
	return oracle::occi::PObject::operator new(size, ctxOCCI_);
}

void *UserActivityTyp::operator new(size_t size,
		const oracle::occi::Connection *sess,
		const OCCI_STD_NAMESPACE::string &tableName,
		const OCCI_STD_NAMESPACE::string &typeName,
		const OCCI_STD_NAMESPACE::string &tableSchema,
		const OCCI_STD_NAMESPACE::string &typeSchema) {
	return oracle::occi::PObject::operator new(size, sess, tableName, typeName,
			tableSchema, typeSchema);
}

OCCI_STD_NAMESPACE::string UserActivityTyp::getSQLTypeName() const {
	return OCCI_STD_NAMESPACE::string("ANDROID.USER_ACTIVITY_TYP");
}

void UserActivityTyp::getSQLTypeName(oracle::occi::Environment *env,
		void **schemaName, unsigned int &schemaNameLen, void **typeName,
		unsigned int &typeNameLen) const {
	PObject::getSQLTypeName(env, &UserActivityTyp::readSQL, schemaName,
			schemaNameLen, typeName, typeNameLen);
}

UserActivityTyp::UserActivityTyp() {
	ACTIVITY_USER = (User *) 0;
	ACTIVITY_TRIP = (TripOffer *) 0;
}

void *UserActivityTyp::readSQL(void *ctxOCCI_) {
	UserActivityTyp *objOCCI_ = new (ctxOCCI_) UserActivityTyp(ctxOCCI_);
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (streamOCCI_.isNull())
			objOCCI_->setNull();
		else
			objOCCI_->readSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		delete objOCCI_;
		excep.setErrorCtx(ctxOCCI_);
		return (void *) NULL;
	}
	return (void *) objOCCI_;
}

void UserActivityTyp::readSQL(oracle::occi::AnyData& streamOCCI_) {
	ACTIVITY_USER = (User *) streamOCCI_.getObject(&UserTyp::readSQL);
	ACTIVITY_TRIP = (TripOffer *) streamOCCI_.getObject(&TripOfferTyp::readSQL);
	ACTIVITY_TYPE = streamOCCI_.getString();
	TIME_STAMP = streamOCCI_.getTimestamp();
}

void UserActivityTyp::writeSQL(void *objectOCCI_, void *ctxOCCI_) {
	UserActivityTyp *objOCCI_ = (UserActivityTyp *) objectOCCI_;
	oracle::occi::AnyData streamOCCI_(ctxOCCI_);

	try {
		if (objOCCI_->isNull())
			streamOCCI_.setNull();
		else
			objOCCI_->writeSQL(streamOCCI_);
	} catch (oracle::occi::SQLException& excep) {
		excep.setErrorCtx(ctxOCCI_);
	}
	return;
}

void UserActivityTyp::writeSQL(oracle::occi::AnyData& streamOCCI_) {
	streamOCCI_.setObject((UserTyp*) ACTIVITY_USER);
	streamOCCI_.setObject((TripOfferTyp*) ACTIVITY_TRIP);
	streamOCCI_.setString(ACTIVITY_TYPE);
	streamOCCI_.setTimestamp(TIME_STAMP);
}

UserActivityTyp::~UserActivityTyp() {
	delete (UserTyp*) ACTIVITY_USER;
	delete (TripOfferTyp*) ACTIVITY_TRIP;
}
