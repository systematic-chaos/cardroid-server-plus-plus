#ifndef IPASSENGER_H_
#define IPASSENGER_H_

#include "IUser.h"

namespace Cardroid {
namespace Data {

/**
 * \interface IPassenger
 * Public operations interface for the implementation of a Passenger object
 */
class IPassenger: public IUser {

public:

	/**
	 * Create an instance of the class implementing this interface,
	 * from the received parameter, which also implements it.
	 * This method acts as an Abstract Factory, for the sake of the
	 * implementation of the Abstract Factory pattern in the search of
	 * interoperability among the different implementations of this interface
	 * which could exist in the different subsystems of the platform
	 * @param passengerObject An object instance implementing this interface
	 * @return An instance of the class implementing this interface, containing
	 * 			exactly the same data of the received bitmapObject parameter,
	 * 			from the viewpoint of the operations defined in this interface
	 */
	virtual shared_ptr<IPassenger> newInstance(
			const shared_ptr<IPassenger> passengerObject) = 0;

	/**
	 * @param userPassenger This IPassenger's new user
	 */
	virtual void setUserPassenger(const IUser& passenger) = 0;

	/**
	 * @return This IPassenger's user
	 */
	virtual IUser& getUserPassenger() = 0;

	/**
	 * @param nSeats The new number of seats reserved by this IPassenger
	 */
	virtual void setNSeats(int nSeats) = 0;

	/**
	 * @return The number of seats reserved by this IPassenger
	 */
	virtual int getNSeats() = 0;

	virtual ~IPassenger() {
	}
};

namespace Ice {

using namespace IceProxy;
using namespace Cardroid::Data;

/**
 * \class Passenger
 * Domain class implementing a Passenger for its transmission between systems
 * communicating across an Ice network infrastructure
 */
class Passenger: public ::cardroid::data::zerocice::PassengerTyp,
		public IPassenger, public ::Ice::ObjectFactory {

public:

	Passenger(::Ice::ObjectAdapterPtr adapter) {
		_adapter = adapter;
	}

	/**
	 * Default constructor
	 */
	Passenger(::cardroid::data::zerocice::UserTypPrx pu, int ns) {
		passengerUser = pu;
		nSeats = ns;
	}

	Passenger(const ::cardroid::data::zerocice::PassengerTyp& passenger) {
		passengerUser = passenger.passengerUser;
		nSeats = passenger.nSeats;
	}

	Passenger(const Passenger& passenger) {
		passengerUser = passenger.passengerUser;
		nSeats = passenger.nSeats;
	}

	Passenger(Passenger&& passenger) {
		swap(passengerUser, passenger.passengerUser);
		swap(nSeats, passenger.nSeats);
	}

	~Passenger() {
		delete &passengerUser;
	}

	Passenger& operator=(Passenger passenger) {
		passengerUser = passenger.passengerUser;
		nSeats = passenger.nSeats;
		return *this;
	}

	Passenger& operator=(Passenger& passenger) {
		swap(passengerUser, passenger.passengerUser);
		swap(nSeats, passenger.nSeats);
		return *this;
	}

	/* IPassenger interface */

	shared_ptr<IUser> newInstance(const shared_ptr<IUser> userObject) {
		return User(_adapter).newInstance(shared_ptr<IUser>(userObject));
	}

	void setName(const string& name) {
		passengerUser->setName(name);
	}

	const string& getName() {
		const string& n = passengerUser->getName();
		return n;
	}

	void setSurname(const string& surname) {
		passengerUser->setSurname(surname);
	}

	const string& getSurname() {
		const string& sn = passengerUser->getSurname();
		return sn;
	}

	void setAvatar(const ::oracle::occi::Blob& avatar) {
		passengerUser->setAvatarBytes(
				::Cardroid::occi::blobToByteArray(avatar));
	}

	bool hasAvatar() {
		return passengerUser->hasAvatar();
	}

	const ::oracle::occi::Blob& getAvatar() {
		return ::Cardroid::occi::byteArrayToBlob(
				passengerUser->getAvatarBytes());
	}

	void setHome(const IPlace& home) {
		passengerUser->setUserHome(
				static_cast<Place*>(Place().newInstance(shared_ptr<IPlace>(
						&const_cast<IPlace&>(home))).get()));
	}

	const IPlace& getHome() {
		::cardroid::data::zerocice::PlaceTypPtr home =
				passengerUser->getUserHome();
		const IPlace& h = dynamic_cast<Place*>(home.get()) ?
				static_cast<Place&>(*(home.get())) : *(new Place(*(home.get())));
		return h;
	}

	void setTelephoneNumber(int telephone) {
		passengerUser->setTelephone(telephone);
	}

	int getTelephoneNumber() {
		return passengerUser->getTelephone();
	}

	void setEmail(const string& email) {
		passengerUser->setEmail(email);
	}

	const string& getEmail() {
		const string& em = passengerUser->getEmail();
		return em;
	}

	void setReputation(int reputation) {
		passengerUser->setReputation(reputation);
	}

	int getReputation() {
		return passengerUser->getReputation();
	}

	void increaseReputation() {
		passengerUser->increaseReputation1();
	}

	void increaseReputation(int increase) {
		passengerUser->increaseReputation(increase);
	}

	void decreaseReputation() {
		passengerUser->decreaseReputation1();
	}

	void decreaseReputation(int decrease) {
		passengerUser->decreaseReputation(decrease);
	}

	bool hasReputation() {
		return passengerUser->hasReputation();
	}

	void setUserPassenger(const IUser& pu) {
		passengerUser = User::createProxy(pu, _adapter);
	}

	User& getUserPassenger() {
		return *(User::extractObject(passengerUser).get());
	}

	void setNSeats(int ns) {
		nSeats = ns;
	}

	int getNSeats() {
		return nSeats;
	}

	void setCars(vector<shared_ptr<ICar>> cars) {
		typedef vector<shared_ptr<ICar>> carVector;
		vector< ::cardroid::data::zerocice::CarTypPrx> c;
		for (carVector::iterator i = cars.begin();
				i != cars.end(); i++)
			c.push_back(Car::createProxy(passengerUser->getEmail(),
					const_cast<const ICar&>(*((*i).get())), _adapter));
		passengerUser->setUserCars(c);
	}

	vector<shared_ptr<ICar>> getCars() {
		vector< ::cardroid::data::zerocice::CarTypPrx> cars =
				passengerUser->getUserCars();
		vector<shared_ptr<ICar>> c;
		for (::cardroid::data::zerocice::CarCollection::iterator cprx =
				cars.begin(); cprx != cars.end(); cprx++)
			c.push_back(static_pointer_cast<ICar>(Car::extractObject(*cprx)));
		return c;
	}

	bool addCar(const ICar& car) {
		return passengerUser->addCar(
				static_cast<Car*>(Car().newInstance(shared_ptr<ICar>(&const_cast<ICar&>(car))).get()));
	}

	bool removeCar(const ICar& car) {
		return passengerUser->removeCar(
				static_cast<Car*>(Car().newInstance(shared_ptr<ICar>(
						&const_cast<ICar&>(car))).get()));
	}

	void clearCars() {
		passengerUser->clearCars();
	}

	int getNCars() {
		return passengerUser->getNCars();
	}

	shared_ptr<IPassenger> newInstance(
			const shared_ptr<IPassenger> passengerObject) {
		if (!passengerObject)
			return NULL;
		if (dynamic_cast<Passenger*>(passengerObject.get()))
			return passengerObject;

		return make_shared<Passenger>(
			User::createProxy(*(passengerObject.get()), _adapter),
			passengerObject->getNSeats());
	}

	/* Overriding superclass */

	::cardroid::data::zerocice::UserTypPrx getPassengerUser(
			const ::Ice::Current& = ::Ice::Current()) const {
		return passengerUser;
	}

	void setPassengerUser(const ::cardroid::data::zerocice::UserTypPrx& pu,
			const ::Ice::Current& = ::Ice::Current()) {
		passengerUser = pu;
	}

	int getNSeats(const ::Ice::Current& = ::Ice::Current()) const {
		return nSeats;
	}

	void setNSeats(int ns, const ::Ice::Current& = ::Ice::Current()) {
		nSeats = ns;
	}

	string toString(const ::Ice::Current& = ::Ice::Current()) const {
		return passengerUser->toString() + ": " + to_string(nSeats) + " asientos";
	}

	/* ObjectFactory interface */

	::Ice::ObjectPtr create(const string& type) {
		if (!type.compare(ice_staticId()))
			return new Passenger(_adapter);
		return NULL;
	}

	void destroy() {
	}

private:

	::Ice::ObjectAdapterPtr _adapter;
	static const long long int serialVersionUID = 4960964214323097918L;
	typedef ::cardroid::data::zerocice::PassengerTyp super;
};
}

namespace Oracle {

using namespace Cardroid::Data;

/**
 * \class TripOffer_Passenger
 * Domain class implementing a Passenger for its storage and retrieval from an
 * Oracle database
 */
class Passenger: private ::PassengerTyp, public IPassenger {

public:

	Passenger() {
	}

	Passenger(void* ctxOCCI_) :
			super(ctxOCCI_) {
	}

	/**
	 * Default constructor
	 */
	Passenger(const User& usr, int nSeats) {
		setUserPassenger(usr);
		setNSeats(nSeats);
	}

	Passenger(const ::PassengerTyp& passenger) {
		setUsr(passenger.getUsr());
		setN_seats(passenger.getN_seats());
	}

	Passenger(const Passenger& p) {
		Passenger& passenger = const_cast<Passenger&>(p);
		setUserPassenger(passenger.getUserPassenger());
		setNSeats(passenger.getNSeats());
	}

	Passenger(Passenger&& passenger) {
		setUserPassenger(passenger.getUserPassenger());
		setNSeats(passenger.getNSeats());
	}

	~Passenger() {
	}

	Passenger& operator=(Passenger passenger) {
		setUserPassenger(passenger.getUserPassenger());
		setNSeats(passenger.getNSeats());
		return *this;
	}

	Passenger& operator=(Passenger& passenger) {
		setUserPassenger(passenger.getUserPassenger());
		setNSeats(passenger.getNSeats());
		return *this;
	}

	/* superclass accessors */

	void setUserPassenger(const IUser& userPassenger) {
		setUsr(
			static_pointer_cast<User>(User().newInstance(
				shared_ptr<IUser>(&const_cast<IUser&>(userPassenger)))).get());
	}

	IUser& getUserPassenger() {
		return *getUsr();
	}

	void setNSeats(int nSeats) {
		setN_seats(nSeats);
	}

	int getNSeats() {
		return getN_seats();
	}

	/* IPassenger interface */

	shared_ptr<IUser> newInstance(const shared_ptr<IUser> userObject) {
		return User().newInstance(userObject);
	}

	shared_ptr<IPassenger> newInstance(
			const shared_ptr<IPassenger> passengerObject) {
		if (!passengerObject)
			return NULL;
		if (dynamic_cast<Passenger*>(passengerObject.get()))
			return passengerObject;

		return make_shared<Passenger>(*(static_pointer_cast<User>(newInstance(
				shared_ptr<IUser>(&(passengerObject->getUserPassenger())))).get()),
				passengerObject->getNSeats());
	}

	void setName(const string& name) {
		getUserPassenger().setName(name);
	}

	const string& getName() {
		return getUserPassenger().getName();
	}

	void setSurname(const string& surname) {
		getUserPassenger().setSurname(surname);
	}

	const string& getSurname() {
		return getUserPassenger().getSurname();
	}

	void setAvatar(const ::oracle::occi::Blob& avatar) {
		getUserPassenger().setAvatar(avatar);
	}

	bool hasAvatar() {
		return getUserPassenger().hasAvatar();
	}

	const ::oracle::occi::Blob& getAvatar() {
		return getUserPassenger().getAvatar();
	}

	void setHome(const IPlace& home) {
		getUserPassenger().setHome(home);
	}

	const IPlace& getHome() {
		return getUserPassenger().getHome();
	}

	void setTelephoneNumber(int telephone) {
		getUserPassenger().setTelephoneNumber(telephone);
	}

	int getTelephoneNumber() {
		return getUserPassenger().getTelephoneNumber();
	}

	void setEmail(const string& email) {
		getUserPassenger().setEmail(email);
	}

	const string& getEmail() {
		return getUserPassenger().getEmail();
	}

	void setReputation(int reputation) {
		getUserPassenger().setReputation(reputation);
	}

	int getReputation() {
		return getUserPassenger().getReputation();
	}

	bool hasReputation() {
		return getUserPassenger().hasReputation();
	}

	void increaseReputation() {
		getUserPassenger().increaseReputation();
	}

	void increaseReputation(int increase) {
		getUserPassenger().increaseReputation(increase);
	}

	void decreaseReputation() {
		getUserPassenger().decreaseReputation();
	}

	void decreaseReputation(int decrease) {
		getUserPassenger().decreaseReputation(decrease);
	}

	void setCars(vector<shared_ptr<ICar>> cars) {
		getUserPassenger().setCars(cars);
	}

	vector<shared_ptr<ICar>> getCars() {
		return getUserPassenger().getCars();
	}

	bool addCar(const ICar& car) {
		return getUserPassenger().addCar(car);
	}

	bool removeCar(const ICar& car) {
		return getUserPassenger().removeCar(car);
	}

	void clearCars() {
		getUserPassenger().clearCars();
	}

	int getNCars() {
		return getUserPassenger().getNCars();
	}

	/* Overriding superclass */

	User* getUsr() const {
		User* user = NULL;
		try {
			user = super::getUsr();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return user;
	}

	void setUsr(User* value) {
		try {
			super::setUsr(value);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	::oracle::occi::Number getN_seats() const {
		::oracle::occi::Number nSeats = 0;
		try {
			nSeats = super::getN_seats();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return nSeats;
	}

	void setN_seats(const ::oracle::occi::Number& value) {
		try {
			super::setN_seats(value);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	static string getSqlTypeName() {
		return string("ANDROID.PASSENGER_TYP");
	}

private:

	typedef ::PassengerTyp super;
};
}
}
}

#endif /* IPASSENGER_H_ */
