#ifndef IDATE_H_
#define IDATE_H_

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
#include "Util.h"

using namespace std;

namespace Cardroid {
namespace Data {

/**
 * \interface IDate
 * Public operations interface for the implementation of a Date object
 */
class IDate {

public:
	/**
	 * Create an instance of the class implementing this interface,
	 * from the received parameter, which also implements it.
	 * This method acts as an Abstract Factory, for the sake of the
	 * implementation of the Abstract Factory pattern in the search of
	 * interoperability among the different implementations of this interface
	 * which could exist in the different subsystems of the platform
	 * @param dateObject An object instance implementing this interface
	 * @return An instance of the class implementing this interface, containing
	 * 			exactly the same data of the received bitmapObject parameter,
	 * 			from the viewpoint of the operations defined in this interface
	 */
	virtual shared_ptr<IDate> newInstance(
			const shared_ptr<IDate> dateObject) = 0;

	/**
	 * @param time The new time in milliseconds for this date, counting from
	 * 				"the date"
	 */
	virtual void setTimeInMillis(long long time) = 0;

	/**
	 * @return The time in milliseconds for this date, counting from "the date"
	 */
	virtual long long getTimeInMillis() = 0;

	virtual ~IDate() {
	}
};

namespace Ice {

using namespace IceProxy;
using namespace Cardroid::Data;

/**
 * \class Date
 * Domain class implementing a Date for its transmission between systems
 * communicating across an Ice network infrastructure
 */
class Date: public ::cardroid::data::zerocice::DateTyp,
		public IDate, public ::Ice::ObjectFactory {

public:

	Date() {
	}

	/**
	 * Default constructor
	 */
	Date(long long dt) {
		datetime = dt;
	}

	Date(const ::cardroid::data::zerocice::DateTyp& date) {
		datetime = date.datetime;
	}

	Date(const Date& date) {
		datetime = date.datetime;
	}

	Date(Date&& date) {
		swap(datetime, date.datetime);
	}

	~Date() {
	}

	Date& operator=(Date date) {
		datetime = date.datetime;
		return *this;
	}

	Date& operator=(Date& date) {
		swap(datetime, date.datetime);
		return *this;
	}

	/* IDate interface */

	shared_ptr<IDate> newInstance(const shared_ptr<IDate> dateObject) {
		if (!dateObject)
			return NULL;
		if (dynamic_cast<Date*>(dateObject.get()))
			return dateObject;
		long long datetime = dateObject->getTimeInMillis();
		return static_pointer_cast<IDate>(make_shared<Date>(datetime));
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

	string toString(const ::Ice::Current&) const {
		::boost::posix_time::ptime p = ::boost::posix_time::from_time_t(
				datetime / 1000);
		struct tm tm = ::boost::posix_time::to_tm(p);
		char buf[16];
		strftime(buf, 15, "%e/%m/%Y", &tm);
		return string(buf);
	}

	/* Comparable<Date> interface */

	bool operator==(const Date& other) {
		return datetime
				== const_cast<Date&>(other).getTimeInMillis();
	}

	bool operator<(const Date& other) {
			return datetime
					< const_cast<Date&>(other).getTimeInMillis();
	}

	bool operator>(const Date& other) {
			return datetime
					> const_cast<Date&>(other).getTimeInMillis();
	}

	/* ObjectFactory interface */

	::Ice::ObjectPtr create(const string& type) {
		if (!type.compare(ice_staticId()))
			return new Date;
		return NULL;
	}

	void destroy() {
	}

protected:

	typedef ::cardroid::data::zerocice::DateTyp super;
	static const long long int serialVersionUID = -3991148834484088982L;
};
}

namespace Oracle {

using namespace Cardroid::Data;

/**
 * \class Date
 * Domain class implementing a Date for its storage and retrieval from an
 * Oracle database
 */
class Date: protected ::DateTyp, public IDate {

public:

	Date() {
	}

	Date(void* ctxOCCI_) :
			super(ctxOCCI_) {
	}

	/**
	 * Default constructor
	 */
	Date(const ::oracle::occi::Timestamp &datetime) {
		setDatetime(datetime);
	}

	Date(const ::DateTyp& date) {
		setDatetime(date.getDatetime());
	}

	Date(const Date& date) {
		setDatetime(date.getDatetime());
	}

	Date(Date&& date) {
		setDatetime(date.getDatetime());
	}

	~Date() {
	}

	Date& operator=(Date date) {
		setDatetime(date.getDatetime());
		return *this;
	}

	Date& operator=(Date& date) {
		setDatetime(date.getDatetime());
		return *this;
	}

	/* IDate interface */

	shared_ptr<IDate> newInstance(const shared_ptr<IDate> dateObject) {
		if (!dateObject)
			return NULL;
		if (dynamic_cast<Date*>(dateObject.get()))
			return dateObject;

		return static_pointer_cast<IDate>(make_shared<Date>(
						::Cardroid::occi::millisToTimestamp(
								dateObject->getTimeInMillis())));
	}

	void setTimeInMillis(long long time) {
		setDatetime(::Cardroid::occi::millisToTimestamp(time));
	}

	long long getTimeInMillis() {
		return ::Cardroid::occi::timestampToMillis(getDatetime());
	}

	/* Overriding superclass */

	void setDatetime(const ::oracle::occi::Timestamp& datetime) {
		try {
			super::setDatetime(datetime);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	::oracle::occi::Timestamp getDatetime() const {
		::oracle::occi::Timestamp datetime = 0;
		try {
			datetime = super::getDatetime();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return datetime;
	}

	static string getSqlTypeName() {
		return string("ANDROID.DATE_TYP");
	}

	/* Comparable<Date> interface */

	bool operator==(const Date& other) {
		return getDatetime()
				== const_cast<Date&>(other).getDatetime();
	}

	bool operator<(const Date& other) {
		return getDatetime()
				< const_cast<Date&>(other).getDatetime();
	}

	bool operator>(const Date& other) {
		return getDatetime()
				> const_cast<Date&>(other).getDatetime();
	}

protected:

	typedef ::DateTyp super;
};
}
}
}

#endif /* IDATE_H_ */
