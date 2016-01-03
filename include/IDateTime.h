#ifndef IDATETIME_H_
#define IDATETIME_H_

#include <boost/date_time/posix_time/conversion.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/time.hpp>
#include <boost/date_time/time_duration.hpp>
#include <Ice/Current.h>
#include <Ice/ObjectFactory.h>
#include <occiControl.h>
#include <occiData.h>
#include <ctime>
#include <iostream>
#include <memory>
#include <string>

#include "Cardroid.h"
#include "DbTypes.h"
#include "IDate.h"

namespace Cardroid {
namespace Data {

/**
 * \interface IDateTIme
 * Public operations interface for the implementation of a DateTime object
 */
class IDateTime: public IDate {

public:

	/**
	 * Create an instance of the class implementing this interface,
	 * from the received parameter, which also implements it.
	 * This method acts as an Abstract Factory, for the sake of the
	 * implementation of the Abstract Factory pattern in the search of
	 * interoperability among the different implementations of this interface
	 * which could exist in the different subsystems of the platform
	 * @param dateTimeObject An object instance implementing this interface
	 * @return An instance of the class implementing this interface, containing
	 * 			exactly the same data of the received bitmapObject parameter,
	 * 			from the viewpoint of the operations defined in this interface
	 */
	virtual shared_ptr<IDateTime> newInstance(
			const shared_ptr<IDateTime> dateTimeObject) = 0;
};

namespace Ice {

using namespace IceProxy;
using namespace Cardroid::Data;

/**
 * \class DateTime
 * Domain class implementing a DateTime for its transmission between systems
 * communicating across an Ice network infrastructure
 */
class DateTime: public ::cardroid::data::zerocice::DateTimeTyp,
		public IDateTime, public ::Ice::ObjectFactory {

public:

	DateTime() {
	}

	/**
	 * Default constructor
	 */
	DateTime(long long dt) {
		datetime = dt;
	}

	DateTime(const ::cardroid::data::zerocice::DateTimeTyp& dt) {
		datetime = dt.datetime;
	}

	DateTime(const DateTime& dt) {
		datetime = dt.datetime;
	}

	DateTime(DateTime&& dt) {
		swap(datetime, dt.datetime);
	}

	~DateTime() {
	}

	DateTime& operator=(DateTime dt) {
		datetime = dt.datetime;
		return *this;
	}

	DateTime& operator=(DateTime& dt) {
		swap(datetime, dt.datetime);
		return *this;
	}

	/* IDateTime interface */

	shared_ptr<IDate> newInstance(const shared_ptr<IDate> dateObject) {
		return ((parent*) this)->newInstance(dateObject);
	}

	shared_ptr<IDateTime> newInstance(
			const shared_ptr<IDateTime> dateTimeObject) {
		if (!dateTimeObject)
			return NULL;
		if (dynamic_cast<DateTime*>(dateTimeObject.get()))
			return dateTimeObject;
		long long datetime = dateTimeObject->getTimeInMillis();
		return static_pointer_cast<IDateTime>(make_shared<DateTime>(datetime));
	}

	long long getTimeInMillis() {
		return datetime;
	}

	void setTimeInMillis(long long dt) {
		datetime = dt;
	}

	/* Overriding superclass */

	static const string& ice_staticId() {
		return super::ice_staticId();
	}

	long long getTimeInMillis(const ::Ice::Current& = ::Ice::Current()) const {
		return datetime;
	}

	void setTimeInMillis(long long dt, const ::Ice::Current& =
			::Ice::Current()) {
		datetime = dt;
	}

	void getDate(int* year, unsigned int* month, unsigned int* day) {
		::boost::posix_time::ptime p = ::boost::posix_time::from_time_t(
				datetime / 1000)
				+ ::boost::posix_time::milliseconds(datetime % 1000);
		struct tm tm = ::boost::posix_time::to_tm(p);
		*year = tm.tm_year;
		*month = tm.tm_mon;
		*day = tm.tm_mday;
	}

	void setDate(int year, unsigned int month, unsigned int day) {
		datetime = ::boost::posix_time::time_duration(24 * (day + 365.25 * (year + month / 12)), 0, 0).total_milliseconds();
	}

	void getTime(unsigned int* hour, unsigned int *minute, unsigned int* second,
			unsigned int* fs) {
		::boost::posix_time::ptime p = ::boost::posix_time::from_time_t(
				datetime / 1000)
				+ ::boost::posix_time::milliseconds(datetime % 1000);
		struct tm tm = to_tm(p);
		*hour = tm.tm_hour;
		*minute = tm.tm_min;
		*second = tm.tm_sec;
		*fs = datetime % 1000;
	}

	void setTime(unsigned int hour, unsigned int minute, unsigned int second,
			unsigned int fs) {
		datetime = ::boost::posix_time::time_duration(hour, minute, second, fs).total_milliseconds();
	}

	string toString(const ::Ice::Current&) const {
		::boost::posix_time::ptime p = ::boost::posix_time::from_time_t(
				datetime / 1000)
				+ ::boost::posix_time::milliseconds(datetime % 1000);
		struct tm tm = ::boost::posix_time::to_tm(p);
		char buf[32];
		strftime(buf, 31, "%e/%m/%Y %H:%M:%S", &tm);
		return string(buf);
	}

	/* Comparable<Date> interface */

	bool operator==(const DateTime& other) {
					return datetime
							== const_cast<DateTime&>(other).getTimeInMillis();
	}

	bool operator<(const DateTime& other) {
					return datetime
							< const_cast<DateTime&>(other).getTimeInMillis();
	}

	bool operator>(const DateTime& other) {
				return datetime
						> const_cast<DateTime&>(other).getTimeInMillis();
	}

	/* ObjectFactory interface */

	::Ice::ObjectPtr create(const string& type) {
		if (!type.compare(ice_staticId()))
			return new DateTime;
		return NULL;
	}

	void destroy() {
	}

private:

	typedef ::cardroid::data::zerocice::DateTimeTyp super;
	typedef Date parent;
	static const long long int serialVersionUID = 6326227506434498485L;
};
}

namespace Oracle {

using namespace Cardroid::Data;

/**
 * \class DateTime
 * Domain class implementing a DateTime for its storage and retrieval from an
 * Oracle database
 */
class DateTime: private ::DateTimeTyp, public IDateTime {

public:

	DateTime() {
	}

	DateTime(void* ctxOCCI_) :
			super(ctxOCCI_) {
	}

	/**
	 * Default constructor
	 */
	DateTime(const ::oracle::occi::Timestamp &datetime) {
		setDatetime(datetime);
	}

	DateTime(const ::DateTimeTyp& dateTime) {
		setDatetime(dateTime.getDatetime());
	}

	DateTime(const DateTime& dateTime) {
		setDatetime(dateTime.getDatetime());
	}

	DateTime(DateTime&& dateTime) {
		setDatetime(dateTime.getDatetime());
	}

	~DateTime() {
	}

	DateTime& operator=(DateTime dateTime) {
		setDatetime(dateTime.getDatetime());
		return *this;
	}

	DateTime& operator=(DateTime& dateTime) {
		setDatetime(dateTime.getDatetime());
		return *this;
	}

	/* IDateTime interface */

	shared_ptr<IDateTime> newInstance(
			const shared_ptr<IDateTime> dateTimeObject) {
		if (!dateTimeObject)
			return NULL;
		if (dynamic_cast<DateTime*>(dateTimeObject.get()))
			return dateTimeObject;

		return static_pointer_cast<IDateTime>(make_shared<DateTime>(
						::Cardroid::occi::millisToTimestamp(
								dateTimeObject->getTimeInMillis())));
	}

	shared_ptr<IDate> newInstance(const shared_ptr<IDate> dateObject) {
		return ((parent*) this)->newInstance(dateObject);
	}

	long long getTimeInMillis() {
		return ((parent) this).getTimeInMillis();
	}

	void setTimeInMillis(long long time) {
		((parent) this).setTimeInMillis(time);
	}

	static string getSqlTypeName() {
		return string("ANDROID.DATE_TIME_TYP");
	}

	/* Comparable<DateTime> interface */

	bool operator==(const DateTime& other) {
		return getDatetime()
				== const_cast<DateTime&>(other).getDatetime();
	}

	bool operator<(const DateTime& other) {
		return getDatetime()
				> const_cast<DateTime&>(other).getDatetime();
	}

	bool operator>(const DateTime& other) {
		return getDatetime()
				> const_cast<DateTime&>(other).getDatetime();
	}

private:

	typedef ::DateTimeTyp super;
	typedef Date parent;
};

}
}
}

#endif /* IDATETIME_H_ */
