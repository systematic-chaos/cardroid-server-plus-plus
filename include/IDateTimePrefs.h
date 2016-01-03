#ifndef IDATETIMEPREFS_H_
#define IDATETIMEPREFS_H_

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

using namespace std;

/**
 * \interface IDateTimePrefs
 * Public operations interface for the implementation of a DateTimePrefs object
 */
class IDateTimePrefs: public IDate {

public:

	/**
	 * \enum TimePreferences
	 * enum type depicting the values available to
	 * express time preferences
	 */
	enum TimePreferences {
		ANY, MORNING, AFTERNOON, NIGHT
	};

	/**
	 * Create an instance of the class implementing this interface,
	 * from the received parameter, which also implements it.
	 * This method acts as an Abstract Factory, for the sake of the
	 * implementation of the Abstract Factory pattern in the search of
	 * interoperability among the different implementations of this interface
	 * which could exist in the different subsystems of the platform
	 * @param dateTimePrefsObject An object instance implementing this interface
	 * @return An instance of the class implementing this interface, containing
	 * 			exactly the same data of the received bitmapObject parameter,
	 * 			from the viewpoint of the operations defined in this interface
	 */
	virtual shared_ptr<IDateTimePrefs> newInstance(
			const shared_ptr<IDateTimePrefs> dateTimePrefsObject) = 0;

	/**
	 * @param toleranceDays The new tolerance days in the date for this
	 * 							IDateTimePrefs
	 */
	virtual void setToleranceDays(int toleranceDays) = 0;

	/**
	 * @return The tolerance days in the date for this IDateTimePrefs
	 */
	virtual int getToleranceDays() = 0;

	/**
	 * @param timePreferences The new time preferences in the date for this
	 * 							IDateTimePrefs
	 */
	virtual void setTimePreferences(TimePreferences timePreferences) = 0;

	/**
	 * @return The time preferences in the date for this IDateTimePrefs
	 */
	virtual TimePreferences getTimePreferences() = 0;
};

namespace Ice {

using namespace IceProxy;
using namespace Cardroid::Data;

/**
 * \class DateTimePrefs
 * Domain class implementing a DateTimePrefs for its transmission between systems
 * communicating across an Ice network infrastructure
 */
class DateTimePrefs: public ::cardroid::data::zerocice::DateTimePrefsTyp,
		public IDateTimePrefs, public ::Ice::ObjectFactory {

public:

	DateTimePrefs() {
	}

	/**
	 * Default constructor
	 */
	DateTimePrefs(long long dt, int td,
			::cardroid::data::zerocice::TimePreferences tp)  {
		datetime = dt;
		toleranceDays = td;
		timePrefs = tp;
	}

	DateTimePrefs(
			const ::cardroid::data::zerocice::DateTimePrefsTyp& dtp) {
		datetime = dtp.datetime;
		toleranceDays = dtp.toleranceDays;
		timePrefs = dtp.timePrefs;
	}

	DateTimePrefs(const DateTimePrefs& dtp) {
		datetime = dtp.datetime;
		toleranceDays = dtp.toleranceDays;
		timePrefs = dtp.timePrefs;
	}

	DateTimePrefs(DateTimePrefs&& dtp) {
		swap(datetime, dtp.datetime);
		swap(toleranceDays, dtp.toleranceDays);
		swap(timePrefs, dtp.timePrefs);
	}

	~DateTimePrefs() {
	}

	DateTimePrefs& operator=(DateTimePrefs dtp) {
		datetime = dtp.datetime;
		toleranceDays = dtp.toleranceDays;
		timePrefs = dtp.timePrefs;
		return *this;
	}

	DateTimePrefs& operator=(DateTimePrefs& dtp) {
		swap(datetime, dtp.datetime);
		swap(toleranceDays, dtp.toleranceDays);
		swap(timePrefs, dtp.timePrefs);
		return *this;
	}

	/* IDateTimePrefs interface */

	shared_ptr<IDate> newInstance(const shared_ptr<IDate> dateObject) {
		return ((parent*) this)->newInstance(dateObject);
	}

	shared_ptr<IDateTimePrefs> newInstance(
			const shared_ptr<IDateTimePrefs> dateTimePrefsObject) {
		if (!dateTimePrefsObject)
			return NULL;
		if (dynamic_cast<DateTimePrefs*>(dateTimePrefsObject.get()))
			return dateTimePrefsObject;
		long long datetime = dateTimePrefsObject->getTimeInMillis();
		int toleranceDays = dateTimePrefsObject->getToleranceDays();
		::cardroid::data::zerocice::TimePreferences timePreferences =
				static_cast< ::cardroid::data::zerocice::TimePreferences>(dateTimePrefsObject->getTimeInMillis());
		return static_pointer_cast<IDateTimePrefs>(make_shared<DateTimePrefs>(
				datetime, toleranceDays, timePreferences));
	}

	long long getTimeInMillis() {
		return datetime;
	}

	void setTimeInMillis(long long dt) {
		datetime = dt;
	}

	void setToleranceDays(int td) {
		toleranceDays = td;
	}

	int getToleranceDays() {
		return toleranceDays;
	}

	void setTimePreferences(IDateTimePrefs::TimePreferences tp) {
		timePrefs = static_cast< ::cardroid::data::zerocice::TimePreferences>(tp);
	}

	IDateTimePrefs::TimePreferences getTimePreferences() {
		return static_cast<IDateTimePrefs::TimePreferences>(timePrefs);
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

	int getToleranceDays(const ::Ice::Current& = ::Ice::Current()) const {
		return toleranceDays;
	}

	void setToleranceDays(int td, const ::Ice::Current& = ::Ice::Current()) {
		toleranceDays = td;
	}

	::cardroid::data::zerocice::TimePreferences getTimePrefs(
			const ::Ice::Current& = ::Ice::Current()) const {
		return timePrefs;
	}

	void setTimePrefs(::cardroid::data::zerocice::TimePreferences tp,
			const ::Ice::Current&) {
		timePrefs = tp;
	}

	void getDate(int* year, unsigned int* month, unsigned int* day) {
		::boost::posix_time::ptime p = ::boost::posix_time::from_time_t(
				datetime / 1000);
		struct tm tm = ::boost::posix_time::to_tm(p);
		*year = tm.tm_year;
		*month = tm.tm_mon;
		*day = tm.tm_mday;
	}

	void setDate(int year, unsigned int month, unsigned int day) {
		datetime = ::boost::posix_time::time_duration(24 * (day + 365.25 * (year + month / 12)), 0, 0).total_milliseconds();
	}

	string toString(const ::Ice::Current&) const {
		::boost::posix_time::ptime p = ::boost::posix_time::from_time_t(
				datetime / 1000);
		struct tm tm = ::boost::posix_time::to_tm(p);
		char buf[32];
		strftime(buf, 31, "%d/%m/%Y", &tm);
		switch (timePrefs) {
		case MORNING:
			strcat(buf, " Mañana");
			break;
		case AFTERNOON:
			strcat(buf, " Tarde");
			break;
		case NIGHT:
			strcat(buf, " Noche");
			break;
		case ANY:
		default:
			strcat(buf, " Cualquier hora");
		}
		return string(buf);
	}

	/* Comparable<DateTimePrefs> interface */

	bool operator==(const DateTimePrefs& other) {
		return datetime
				== const_cast<DateTimePrefs&>(other).getTimeInMillis();
	}

	bool operator<(const DateTimePrefs& other) {
			return datetime
					< const_cast<DateTimePrefs&>(other).getTimeInMillis();
	}

	bool operator>(const DateTimePrefs& other) {
			return datetime
					> const_cast<DateTimePrefs&>(other).getTimeInMillis();
	}

	/* ObjectFactory interface */

	::Ice::ObjectPtr create(const string& type) {
		if (!type.compare(ice_staticId()))
			return new DateTimePrefs;
		return NULL;
	}

	void destroy() {
	}

private:

	typedef ::cardroid::data::zerocice::DateTimePrefsTyp super;
	typedef Date parent;
	static const long long int serialVersionUID = 8438816444504630535L;
};
}

namespace Oracle {

using namespace Cardroid::Data;

/**
 * \class DateTimePrefs
 * Domain class implementing a DateTimePrefs for its storage and retrieval
 * from an Oracle database
 */
class DateTimePrefs: private ::DateTimePrefsTyp, public IDateTimePrefs {

public:

	DateTimePrefs() {
	}

	DateTimePrefs(void* ctxOCCI_) :
			super(ctxOCCI_) {
	}

	/**
	 * Default constructor
	 */
	DateTimePrefs(const ::oracle::occi::Timestamp &datetime,
			::oracle::occi::Number toleranceDays, string timePreferences) {
		setDatetime(datetime);
		setTolerance_days(toleranceDays);
		setTime_preferences(timePreferences);
	}

	DateTimePrefs(const ::DateTimePrefsTyp& dateTimePrefs) {
		setDatetime(dateTimePrefs.getDatetime());
		setTolerance_days(dateTimePrefs.getTolerance_days());
		setTime_preferences(dateTimePrefs.getTime_preferences());
	}

	DateTimePrefs(const DateTimePrefs& dateTimePrefs) {
		setDatetime(dateTimePrefs.getDatetime());
		setTolerance_days(dateTimePrefs.getTolerance_days());
		setTime_preferences(dateTimePrefs.getTime_preferences());
	}

	DateTimePrefs(DateTimePrefs&& dateTimePrefs) {
		setDatetime(dateTimePrefs.getDatetime());
		setTolerance_days(dateTimePrefs.getTolerance_days());
		setTime_preferences(dateTimePrefs.getTime_preferences());
	}

	~DateTimePrefs() {
	}

	DateTimePrefs& operator=(DateTimePrefs dateTimePrefs) {
		setDatetime(dateTimePrefs.getDatetime());
		setTolerance_days(dateTimePrefs.getTolerance_days());
		setTime_preferences(dateTimePrefs.getTime_preferences());
		return *this;
	}

	DateTimePrefs& operator=(DateTimePrefs& dateTimePrefs) {
		setDatetime(dateTimePrefs.getDatetime());
		setTolerance_days(dateTimePrefs.getTolerance_days());
		setTime_preferences(dateTimePrefs.getTime_preferences());
		return *this;
	}

	/* IDateTimePrefs interface */

	shared_ptr<IDateTimePrefs> newInstance(
			const shared_ptr<IDateTimePrefs> dateTimePrefsObject) {
		if (!dateTimePrefsObject)
			return NULL;
		if (dynamic_cast<DateTimePrefs*>(dateTimePrefsObject.get()))
			return dateTimePrefsObject;

		::oracle::occi::Timestamp ts = ::Cardroid::occi::millisToTimestamp(
				dateTimePrefsObject->getTimeInMillis());
		string prefs = NULL;
		switch (dateTimePrefsObject->getTimePreferences()) {
		case MORNING:
			prefs = "MORNING";
			break;
		case AFTERNOON:
			prefs = "AFTERNOON";
			break;
		case NIGHT:
			prefs = "NIGHT";
			break;
		case ANY:
		default:
			prefs = "ANY";
		}
		return static_pointer_cast<IDateTimePrefs>(make_shared<DateTimePrefs>(ts,
				dateTimePrefsObject->getToleranceDays(), prefs));
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

	void setToleranceDays(int toleranceDays) {
		setTolerance_days(toleranceDays);
	}

	int getToleranceDays() {
		return (int)getTolerance_days();
	}

	void setTimePreferences(IDateTimePrefs::TimePreferences timePreferences) {
		string prefs;
		switch (timePreferences) {
		case MORNING:
			prefs = "MORNING";
			break;
		case AFTERNOON:
			prefs = "AFTERNOON";
			break;
		case NIGHT:
			prefs = "NIGHT";
			break;
		case ANY:
		default:
			prefs = "ANY";
		}
		setTime_preferences(prefs);
	}

	IDateTimePrefs::TimePreferences getTimePreferences() {
		string prefs = getTime_preferences();
		if (!prefs.compare("MORNING"))
			return IDateTimePrefs::MORNING;
		if (!prefs.compare("AFTERNOON"))
			return IDateTimePrefs::AFTERNOON;
		if (!prefs.compare("NIGHT"))
			return IDateTimePrefs::NIGHT;
		return IDateTimePrefs::ANY;
	}

	/* Overriding superclass */

	::oracle::occi::Number getTolerance_days() const {
		int toleranceDays = 0;
		try {
			toleranceDays = super::getTolerance_days();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return toleranceDays;
	}

	void setTolerance_days(const ::oracle::occi::Number& value) {
		try {
			super::setTolerance_days(value);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	string getTime_preferences() const {
		string timePreferences = "";
		try {
			timePreferences = super::getTime_preferences();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return timePreferences;
	}

	void setTime_preferences(const string& value) {
		try {
			super::setTime_preferences(value);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	static string getSqlTypeName() {
		return string("ANDROID.DATE_TIME_PREFS_TYP");
	}

	/* Comparable<DateTimePrefs> interface */

	bool operator==(const DateTimePrefs& other) {
		return getDatetime()
				== const_cast<DateTimePrefs&>(other).getDatetime();
	}

	bool operator<(const DateTimePrefs& other) {
		return getDatetime()
				< const_cast<DateTimePrefs&>(other).getDatetime();
	}

	bool operator>(const DateTimePrefs& other) {
		return getDatetime()
				> const_cast<DateTimePrefs&>(other).getDatetime();
	}

private:

	typedef ::DateTimePrefsTyp super;
	typedef Date parent;
};
}
}
}

#endif /* IDATETIMEPREFS_H_ */
