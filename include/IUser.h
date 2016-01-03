#ifndef IUSER_H_
#define IUSER_H_

#include <Ice/Current.h>
#include <Ice/Handle.h>
#include <Ice/Identity.h>
#include <Ice/ObjectAdapter.h>
#include <Ice/ProxyHandle.h>
#include <IceUtil/Handle.h>
#include <IceUtil/Optional.h>
#include <occiCommon.h>
#include <occiControl.h>
#include <occiData.h>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

#include "Cardroid.h"
#include "DbTypes.h"
#include "ICar.h"
#include "IPlace.h"

using namespace std;

namespace Cardroid {
namespace Data {

/**
 * \interface IUser
 * Public operations interface for the implementation of an User object
 */
class IUser {

public:

	/**
	 * Create an instance of the class implementing this interface,
	 * from the received parameter, which also implements it.
	 * This method acts as an Abstract Factory, for the sake of the
	 * implementation of the Abstract Factory pattern in the search of
	 * interoperability among the different implementations of this interface
	 * which could exist in the different subsystems of the platform
	 * @param userObject An object instance implementing this interface
	 * @return An instance of the class implementing this interface, containing
	 * 			exactly the same data of the received bitmapObject parameter,
	 * 			from the viewpoint of the operations defined in this interface
	 */
	virtual shared_ptr<IUser> newInstance(
			const shared_ptr<IUser> userObject) = 0;

	/**
	 * @param name The new name for this IUser
	 */
	virtual void setName(const string& name) = 0;

	/**
	 * @return The name for this IUser
	 */
	virtual const string& getName() = 0;

	/**
	 * @param surname The new surname for this IUser object
	 */
	virtual void setSurname(const string& surname) = 0;

	/**
	 * @return The surname for this IUser object
	 */
	virtual const string& getSurname() = 0;

	/**
	 * @param avatar The new avatar picture for this IUser, in the form of a
	 * 					Blob, so it might need to be read by means of a binary
	 * 					stream
	 */
	virtual void setAvatar(const oracle::occi::Blob& avatar) = 0;

	/**
	 * @return Whether this IUser has an avatar picture
	 */
	virtual bool hasAvatar() = 0;

	/**
	 * @return The avatar picture for this IUser, in the form of a Blob, so it
	 * 			might need to be read by means of a binary stream
	 */
	virtual const oracle::occi::Blob& getAvatar() = 0;

	/**
	 * @param home The new home place for this user
	 */
	virtual void setHome(const IPlace& home) = 0;

	/**
	 * @return The home place for this user
	 */
	virtual const IPlace& getHome() = 0;

	/**
	 * @param telephone The new telephone number for this IUser
	 */
	virtual void setTelephoneNumber(int telephone) = 0;

	/**
	 * @return The telephone number for this IUser
	 */
	virtual int getTelephoneNumber() = 0;

	/**
	 * @param email The new email for this IUser
	 */
	virtual void setEmail(const string& email) = 0;

	/**
	 * @return The email for this IUser
	 */
	virtual const string& getEmail() = 0;

	/*
	 * @return The new reputation scale value for this IUser
	 */
	virtual void setReputation(int reputation) = 0;

	/**
	 * @return The reputation scale value for this IUser
	 */
	virtual int getReputation() = 0;

	/**
	 * Increase the reputation scale value for this IUser in one unit
	 */
	virtual void increaseReputation() = 0;
	/**
	 * Increase the reputation scale value for this IUser in the specified
	 * number of units
	 * @param increase The number of units on which this IUser 's reputation
	 * 					scale value will be increased
	 */
	virtual void increaseReputation(int increase) = 0;

	/**
	 * Decrease the reputation scale value for this IUser in one unit
	 */
	virtual void decreaseReputation() = 0;

	/**
	 * Decrease the reputation scale value for this IUser in the specified
	 * number of units
	 * @param decrease The number of units on which this User's reputation
	 * 					scale value wil be decreased
	 */
	virtual void decreaseReputation(int decrease) = 0;

	/**
	 * @return Whether this IUser has a value in the reputation scale
	 */
	virtual bool hasReputation() = 0;

	/**
	 * @param cars The new car collection for this IUser
	 */
	virtual void setCars(vector<shared_ptr<ICar>> cars) = 0;

	/**
	 * @return The car collection for this IUSer
	 */
	virtual vector<shared_ptr<ICar>> getCars() = 0;

	/**
	 * @param car 	The car to be added to this IUser' s car collection
	 * @return		true
	 */
	virtual bool addCar(const ICar& car) = 0;

	/**
	 * @param car The car to be removed from this IUser's car collection
	 * @return Whether this IUser's car collection was modified
	 */
	virtual bool removeCar(const ICar &car) = 0;

	/**
	 * Remove every car from this IUser's car collection
	 */
	virtual void clearCars() = 0;

	/**
	 * @return The number of cars in this Iuser's car collection
	 */
	virtual int getNCars() = 0;

	virtual ~IUser() {
	}
};

namespace Ice {

using namespace IceProxy;
using namespace Cardroid::Data;

/**
 * \class User
 * Domain class implementing an User for its transmission between systems
 * communicating across an Ice network infrastructure
 */
class User: public ::cardroid::data::zerocice::UserTyp,
		public IUser, public ::Ice::ObjectFactory {

public:

	User(::Ice::ObjectAdapterPtr adapter) {
		_adapter = adapter;
	}

	/**
	 * Short constructor
	 */
	User(const string& n, const string& s,
			const ::cardroid::data::zerocice::PlaceTypPtr h, int t,
			const string& e,
			const vector< ::cardroid::data::zerocice::CarTypPrx> c) {
		name = n;
		surname = s;
		home = h;
		telephone = t;
		email = e;
		cars = c;
	}

	/**
	 * Long constructor
	 */
	User(const string& n, const string& s, const vector<unsigned char> a,
			const ::cardroid::data::zerocice::PlaceTypPtr h, int t,
			const string& e, int r,
			const vector< ::cardroid::data::zerocice::CarTypPrx> c) {
		name = n;
		surname = s;
		userAvatarBytes = a;
		home = h;
		telephone = t;
		email = e;
		userReputation = r;
		cars = c;
	}

	User(const ::cardroid::data::zerocice::UserTyp& user)  {
		name = user.name;
		surname = user.surname;
		home = user.home;
		telephone = user.telephone;
		email = user.email;
		cars = user.cars;
		if (user.userAvatarBytes)
			userAvatarBytes = user.userAvatarBytes.get();
		if (user.userReputation)
			userReputation = user.userReputation.get();
	}

	User(const User& u) {
		User& user = const_cast<User&>(u);
		name = user.name;
		surname = user.surname;
		home = user.home;
		telephone = user.telephone;
		email = user.email;
		cars = user.cars;
		if (user.userAvatarBytes)
			userAvatarBytes = user.userAvatarBytes.get();
		if (user.userReputation)
			userReputation = user.userReputation.get();
	}

	User(User&& user) {
		swap(name, user.name);
		swap(surname, user.surname);
		swap(home, user.home);
		swap(telephone, user.telephone);
		swap(email, user.email);
		swap(cars, user.cars);
		if (user.userAvatarBytes)
			swap(userAvatarBytes, user.userAvatarBytes);
		if (user.userReputation)
			swap(userReputation, user.userReputation);
	}

	~User() {
		delete &home;
		delete &cars;
		if (userAvatarBytes)
			delete &userAvatarBytes;
	}

	User& operator=(User user) {
		name = user.name;
		surname = user.surname;
		home = user.home;
		telephone = user.telephone;
		email = user.email;
		cars = user.cars;
		if (user.userAvatarBytes)
			userAvatarBytes = user.userAvatarBytes.get();
		if (user.userReputation)
					userReputation = user.userReputation.get();
		return *this;
	}

	User& operator=(User& user) {
		swap(name, user.name);
		swap(surname, user.surname);
		swap(home, user.home);
		swap(telephone, user.telephone);
		swap(email, user.email);
		swap(cars, user.cars);
		if (user.userAvatarBytes)
			swap(userAvatarBytes, user.userAvatarBytes);
		if (user.userReputation)
			swap(userReputation, user.userReputation);
		return *this;
	}

	/**
	 * @return An Ice Identity for this datatype category and the data provided
	 */
	static ::Ice::Identity createIdentity(const string& email) {
		::Ice::Identity id;
		id.category = "user";
		id.name = email;
		return id;
	}

	/**
	 * @return A proxy to an Ice Object incarnating the provided data, whose
	 * 			servant is added to adapter 's active servant map
	 */
	static ::cardroid::data::zerocice::UserTypPrx createProxy(
			const ::cardroid::data::zerocice::UserTyp& userObject,
			::Ice::ObjectAdapterPtr adapter) {
		::cardroid::data::zerocice::UserTypPrx userPrx =
				::cardroid::data::zerocice::UserTypPrx::uncheckedCast(
						adapter->createProxy(
								createIdentity(userObject.getEmail())));
			fillProxyData(userObject, userPrx, adapter);

		return userPrx;
	}

	/**
	 * @return A proxy to an Ice Object incarnating the provided data, whose
	 * 			servant is added to adapter 's active servant map
	 */
	static ::cardroid::data::zerocice::UserTypPrx createProxy(const IUser& uo,
			::Ice::ObjectAdapterPtr adapter) {
		IUser& userObject = const_cast<IUser&>(uo);

		::cardroid::data::zerocice::UserTypPrx userPrx =
				::cardroid::data::zerocice::UserTypPrx::uncheckedCast(
						adapter->createProxy(
								createIdentity(userObject.getEmail())));
		fillProxyData(userObject, userPrx, adapter);

		return userPrx;
	}

	/**
	 * Fill the servant referenced by the given proxy with the data contained
	 * in the given object
	 */
	static void fillProxyData(const ::cardroid::data::zerocice::UserTyp& object,
			::cardroid::data::zerocice::UserTypPrx proxy,
			::Ice::ObjectAdapterPtr adapter) {
		proxy->setName(object.getName());
		proxy->setSurname(object.getSurname());
		if (object.hasAvatar())
			proxy->setAvatarBytes(object.getAvatarBytes());
		proxy->setUserHome(object.getUserHome());
		proxy->setTelephone(object.getTelephone());
		proxy->setEmail(object.getEmail());
		if (object.hasReputation())
			proxy->setReputation(object.getReputation());
		proxy->setUserCars(object.getUserCars());
	}

	/**
	 * Fill the servant referenced by the given proxy with the data contained
	 * in the given object
	 */
	static void fillProxyData(const IUser& o,
			::cardroid::data::zerocice::UserTypPrx& proxy,
			::Ice::ObjectAdapterPtr adapter) {
		IUser& object = const_cast<IUser&>(o);
		proxy->setName(object.getName());
		proxy->setSurname(object.getSurname());
		if (object.hasAvatar())
			proxy->setAvatarBytes(
					::Cardroid::occi::blobToByteArray(object.getAvatar()));
		proxy->setUserHome(static_cast< ::cardroid::data::zerocice::PlaceTyp*>
				(static_pointer_cast<Place>(Place().newInstance(
				shared_ptr<IPlace>(&(const_cast<IPlace&>(object.getHome()))))).get()));
		proxy->setTelephone(object.getTelephoneNumber());
		proxy->setEmail(object.getEmail());
		typedef vector<shared_ptr<ICar>> carVector;
		if (object.hasReputation())
			proxy->setReputation(object.getReputation());
		carVector objectCars = object.getCars();
		vector< ::cardroid::data::zerocice::CarTypPrx> proxyCars;
		for (carVector::const_iterator i = objectCars.cbegin();
				i != objectCars.cend(); i++)
			proxyCars.push_back(
					Car::createProxy(object.getEmail(), *((*i).get()), adapter));
		proxy->setUserCars(proxyCars);
	}

	/**
	 * @param proxy A proxy to a remote object implementing an User
	 * @return A local User object containing the data of the remote object
	 * 			referenced by proxy
	 */
	static shared_ptr<User> extractObject(
			::cardroid::data::zerocice::UserTypPrx proxy) {
		shared_ptr<User> user = NULL;

		if (!(proxy->hasAvatar() && proxy->hasReputation())) {
			user = shared_ptr<User>(
					new User(proxy->getName(), proxy->getSurname(),
							proxy->getUserHome(), proxy->getTelephone(),
							proxy->getEmail(), proxy->getUserCars()));
			if (!proxy->hasAvatar() && !proxy->hasReputation())
				return user;
		}

		vector<unsigned char> userAvatar = proxy->getAvatarBytes();
		int userReputation = proxy->getReputation();

		if (!(proxy->hasAvatar() && proxy->hasReputation())) {
			if (proxy->hasAvatar())
				user->setAvatarBytes(userAvatar);
			if (proxy->hasReputation())
				user->setReputation(userReputation, ::Ice::Current());
		} else
			user = shared_ptr<User>(
					new User(proxy->getName(), proxy->getSurname(), userAvatar,
							proxy->getUserHome(), proxy->getTelephone(),
							proxy->getEmail(), userReputation,
							proxy->getUserCars()));
		return user;
	}

	/* IUser interface */

	shared_ptr<IUser> newInstance(const shared_ptr<IUser> userObject) {
		if (!userObject)
			return NULL;
		if (dynamic_cast<User*>(userObject.get()))
			return userObject;

		shared_ptr<IUser> user;

		typedef vector<shared_ptr<ICar>> carVector;

		shared_ptr<Place> userHome = static_pointer_cast<Place>(Place().newInstance(
				shared_ptr<IPlace>(&const_cast<IPlace&>(userObject->getHome()))));
		vector< ::cardroid::data::zerocice::CarTypPrx> userCars;
		for (carVector::const_iterator c = userObject->getCars().cbegin();
				c != userObject->getCars().cend(); c++) {
			userCars.push_back(
					Car::createProxy(userObject->getEmail(), *((*c).get()), _adapter));
		}
		if (!(userObject->hasAvatar() && userObject->hasReputation())) {
			user = static_pointer_cast<User>(make_shared<User>(
					userObject->getName(), userObject->getSurname(),
					userHome.get(), userObject->getTelephoneNumber(),
					userObject->getEmail(), userCars));
			if (!userObject->hasAvatar() && !userObject->hasReputation())
				return user;
		}

		vector<unsigned char> userAvatar;
		if (userObject->hasAvatar())
			userAvatar = ::Cardroid::occi::blobToByteArray(
					userObject->getAvatar());
		int userReputation = userObject->getReputation();

		if (!(userObject->hasAvatar() && userObject->hasReputation())) {
			if (userObject->hasAvatar())
				user->setAvatar(userObject->getAvatar());
			if (userObject->hasReputation())
				user->setReputation(userReputation);
		} else
			user = static_pointer_cast<IUser>(make_shared<User>(
					userObject->getName(), userObject->getSurname(),
					userAvatar, userHome.get(),
					userObject->getTelephoneNumber(),
					userObject->getEmail(), userReputation, userCars));

		return user;
	}

	void setName(const string& n) {
		name = n;
	}

	const string& getName() {
		return name;
	}

	void setSurname(const string& s) {
		surname = s;
	}

	const string& getSurname() {
		return surname;
	}

	void setAvatar(const ::oracle::occi::Blob& a) {
		setAvatarBytes(::Cardroid::occi::blobToByteArray(a));
	}

	const oracle::occi::Blob& getAvatar() {
		return ::Cardroid::occi::byteArrayToBlob(getAvatarBytes());
	}

	bool hasAvatar() {
		return userAvatarBytes;
	}

	void setHome(const IPlace& h) {
		home = static_pointer_cast<Place>(Place().newInstance(shared_ptr<IPlace>(&const_cast<IPlace&>(h)))).get();
	}

	const Place& getHome() {
		if (dynamic_cast<Place*>(home.get())) {
			const Place& h = dynamic_cast<Place&>(*(home.get()));
			return h;
		} else {
			Place* h = new Place(const_cast<const ::cardroid::data::zerocice::PlaceTyp&>(*(home.get())));
			return const_cast<const Place&>(*h);
		}
	}

	void setTelephoneNumber(int t) {
		telephone = t;
	}

	int getTelephoneNumber() {
		return telephone;
	}

	void setEmail(const string& e) {
		email = e;
	}

	const string& getEmail() {
		return email;
	}

	void setCars(vector<shared_ptr<ICar>> newCars) {
		typedef vector<shared_ptr<ICar>> carVector;
		cars = vector< ::cardroid::data::zerocice::CarTypPrx>();
		for (carVector::const_iterator i = newCars.begin();
				i != newCars.end(); i++) {
			cars.push_back(Car::createProxy(getEmail(), const_cast<const ICar&>(*((*i).get())), _adapter));
		}
	}

	vector<shared_ptr<ICar>> getCars() {
		vector<shared_ptr<ICar>> newCars = vector<shared_ptr<ICar>>();
		for (vector< ::cardroid::data::zerocice::CarTypPrx>::const_iterator c =
				cars.begin(); c != cars.end(); c++) {
			newCars.push_back(static_pointer_cast<ICar>(Car::extractObject(*c)));
		}
		return newCars;
	}

	int getNCars() {
		return cars.size();
	}

	void clearCars() {
		cars.clear();
	}

	bool addCar(const ICar& c) {
		ICar& car = const_cast<ICar&>(c);
		::cardroid::data::zerocice::CarTypPrx cp = Car::createProxy(getEmail(),
				car, _adapter);
		unsigned int n;
		for (n = 0; n < cars.size(); n++)
			if (!cars.at(n)->getPlate().compare(car.getPlate()))
				break;
		if (n < cars.size())
			cars[n] = cp;
		else
			cars.push_back(cp);
		return true;
	}

	bool removeCar(const ICar& c) {
		ICar& car = const_cast<ICar&>(c);
		unsigned int n;
		for (n = 0; n < cars.size(); n++)
			if (!cars.at(n)->getPlate().compare(car.getPlate()))
				break;
		if (n < cars.size())
			cars.erase(cars.begin() + n);
		return n < cars.size();
	}

	void setReputation(int reputation) {
		userReputation = reputation;
	}

	int getReputation() {
		return hasReputation() ? userReputation.get() : 0;
	}

	bool hasReputation() {
		return userReputation > 0;
	}

	void increaseReputation() {
		increaseReputation(1, ::Ice::Current());
	}

	void increaseReputation(int increase) {
		setReputation(
				hasReputation() ?
						getReputation() + increase : increase, ::Ice::Current());
	}

	void decreaseReputation() {
		decreaseReputation(1, ::Ice::Current());
	}

	void decreaseReputation(int decrease) {
		setReputation(
				hasReputation() ? getReputation() - decrease : 0, ::Ice::Current());
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

	string getSurname(const ::Ice::Current& = ::Ice::Current()) const {
		return surname;
	}

	void setSurname(const string& s, const ::Ice::Current& = ::Ice::Current()) {
		surname = s;
	}

	vector<unsigned char> getAvatarBytes(const ::Ice::Current& =
			::Ice::Current()) const {
		return userAvatarBytes.get();
	}

	void setAvatarBytes(const vector<unsigned char>& blob,
			const ::Ice::Current& = ::Ice::Current()) {
		userAvatarBytes = blob;
	}

	bool hasAvatar(const ::Ice::Current& = ::Ice::Current()) const {
		return userAvatarBytes;
	}

	::cardroid::data::zerocice::PlaceTypPtr getUserHome(const ::Ice::Current& =
			::Ice::Current()) const {
		return home;
	}

	void setUserHome(const ::cardroid::data::zerocice::PlaceTypPtr& h,
			const ::Ice::Current& = ::Ice::Current()) {
		home = h;
	}

	int getTelephone(const ::Ice::Current& = ::Ice::Current()) const {
		return telephone;
	}

	void setTelephone(int t, const ::Ice::Current& = ::Ice::Current()) {
		telephone = t;
	}

	string getEmail(const ::Ice::Current& current = ::Ice::Current()) const {
		return email;
	}

	void setEmail(const string& e, const ::Ice::Current& = ::Ice::Current()) {
		email = e;
	}

	int getReputation(const ::Ice::Current& = ::Ice::Current()) const {
		return userReputation.get();
	}

	void setReputation(int r, const ::Ice::Current& = ::Ice::Current()) {
		userReputation = r;
	}

	bool hasReputation(const ::Ice::Current& = ::Ice::Current()) const {
		return userReputation;
	}

	vector< ::cardroid::data::zerocice::CarTypPrx> getUserCars(
			const ::Ice::Current& = ::Ice::Current()) const {
		return cars;
	}

	void setUserCars(const vector< ::cardroid::data::zerocice::CarTypPrx>& c,
			const ::Ice::Current& = ::Ice::Current()) {
		cars = c;
	}

	string toString(const ::Ice::Current& = ::Ice::Current()) const {
		return name + " " + surname;
	}

	void increaseReputation1(const ::Ice::Current& current = ::Ice::Current()) {
		increaseReputation(1, current);
	}

	void increaseReputation(int increase, const ::Ice::Current& current =
			::Ice::Current()) {
		setReputation(
				hasReputation() ? getReputation() + increase : increase, current);
	}

	void decreaseReputation1(const ::Ice::Current& current = ::Ice::Current()) {
		decreaseReputation(1, current);
	}

	void decreaseReputation(int decrease, const ::Ice::Current& current =
			::Ice::Current()) {
		setReputation(
				(hasReputation() ? max(getReputation() - decrease, 0) : 0), current);
	}

	bool addCar(const ::cardroid::data::zerocice::CarTypPtr& c,
			const ::Ice::Current& current = ::Ice::Current()) {
		unsigned int n;
		for (n = 0; n < cars.size(); n++)
			if (!cars.at(n)->getPlate().compare(c->getPlate()))
				break;
		if (n <= cars.size())
			cars[n] = Car::createProxy(email, *c, current.adapter);
		else
			cars.push_back(Car::createProxy(email, *c, current.adapter));
		return true;
	}

	bool removeCar(const ::cardroid::data::zerocice::CarTypPtr& c,
			const ::Ice::Current& = ::Ice::Current()) {
		unsigned int n;
		for (n = 0; n < cars.size(); n++)
			if (!cars.at(n)->getPlate().compare(c->getPlate()))
				break;
		if (n < cars.size())
			cars.erase(cars.begin() + n);
		return n < cars.size();
	}

	int getNCars(const ::Ice::Current& = ::Ice::Current()) {
		return cars.size();
	}

	void clearCars(const ::Ice::Current& = ::Ice::Current()) {
		cars.clear();
	}

	/* ObjectFactory interface */

	::Ice::ObjectPtr create(const string& type) {
		if (!type.compare(ice_staticId()))
			return new User(_adapter);
		return NULL;
	}

	void destroy() {
	}

private:

	::Ice::ObjectAdapterPtr _adapter;
	static const long long int serialVersionUID = -1954801766504512077L;
	typedef ::cardroid::data::zerocice::UserTyp super;
};
}

namespace Oracle {

using namespace Cardroid::Data;

/**
 * \class Place
 * Domain class implementing a Place for its storage and retrieval from an
 * Oracle database
 */
class User: public ::UserTyp, public IUser {

public:

	User() {
	}

	User(void* ctxOCCI_) :
			super(ctxOCCI_) {
	}

	/* Long constructor */
	User(const string& name, const string& surname,
			const ::oracle::occi::Blob& avatar, Place& home, int telephone,
			const string& email, int reputation, vector<Car*>& cars) {
		setName(name);
		setSurname(surname);
		//setAvatar(avatar);
		setHome(home);
		setTelephone(telephone);
		setEmail(email);
		setReputation(reputation);
		setCars(cars);
	}

	/* Short constructor */
	User(const string& name, const string& surname, Place& home, int telephone,
			const string& email) {
		setName(name);
		setSurname(surname);
		setHome(home);
		setTelephone(telephone);
		setEmail(email);
	}

	User(const ::UserTyp& user) {
		setName(user.getName());
		setSurname(user.getSurname());
		super::setHome(user.getHome());
		setTelephone(user.getTelephone());
		setEmail(user.getEmail());
		if (!user.getAvatar().isNull())
			setAvatar(user.getAvatar());
		if (((int)user.getReputation()) >= 0)
			super::setReputation(user.getReputation());
		setCars(user.getCars());
	}

	User(const User& u) {
		User& user = const_cast<User&>(u);
		setName(user.getName());
		setSurname(user.getSurname());
		setHome(user.getHome());
		setTelephone(user.getTelephone());
		setEmail(user.getEmail());
		if (user.hasAvatar())
			setAvatar(user.getAvatar());
		if (user.hasReputation())
			super::setReputation(user.getReputation());
		setCars(user.getCars());
	}

	User(User&& user) {
		setName(user.getName());
		setSurname(user.getSurname());
		setHome (user.getHome());
		setTelephone(user.getTelephone());
		setEmail(user.getEmail());
		if (user.hasAvatar())
			setAvatar(user.getAvatar());
		if (user.hasReputation())
			super::setReputation(user.getReputation());
		setCars(user.getCars());
	}

	~User() {
	}

	/* IUser interface */

	shared_ptr<IUser> newInstance(shared_ptr<IUser> userObject) {
		if (!userObject)
			return NULL;
		if (dynamic_cast<User*>(userObject.get()))
			return userObject;

		Place& home = *(static_pointer_cast<Place>(Place().newInstance(
				shared_ptr<IPlace>(&(const_cast<IPlace&>(userObject->getHome()))))).get());
		const vector<shared_ptr<ICar>>& cars = userObject->getCars();
		vector<Car*> c = vector<Car*>();
		for (vector<shared_ptr<ICar>>::const_iterator i = cars.cbegin(); i != cars.cend();
				i++)
			c.push_back(static_pointer_cast<Car>(Car().newInstance(*i)).get());
		::oracle::occi::Blob* avatar = new ::oracle::occi::Blob();
		avatar->setNull();
		/*return make_shared<User>(userObject->getName(), userObject->getSurname(),
		 userObject->getAvatar(), home, userObject->getTelephoneNumber(),
		 userObject->getEmail(), userObject->getReputation(),
		 vector<Car*>);*/
		return static_pointer_cast<User>(make_shared<User>(
				userObject->getName(), userObject->getSurname(),
				const_cast<const ::oracle::occi::Blob&>(*avatar),
				home, userObject->getTelephoneNumber(), userObject->getEmail(),
				userObject->getReputation(), c));
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

	void setSurname(const string& surname) {
		try {
			super::setSurname(surname);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	const string& getSurname() {
		const string& sn = "";
		try {
			const string& surname = super::getSurname();
			return surname;
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return sn;
	}

	void setAvatar(const ::oracle::occi::Blob& avatar) {
		try {
			super::setAvatar(avatar);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	const ::oracle::occi::Blob& getAvatar() {
		const ::oracle::occi::Blob& a = ::oracle::occi::Blob();
		try {
			const ::oracle::occi::Blob& avatar = super::getAvatar();
			return avatar;
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return a;
	}

	bool hasAvatar() {
		return !getAvatar().isNull();
	}

	void setHome(const IPlace& home) {
		setHome(static_pointer_cast<Place>(Place().newInstance(
				shared_ptr<IPlace>(&const_cast<IPlace&>(home)))).get());
	}

	const IPlace& getHome() {
		IPlace* h = NULL;
		try {
			const IPlace& home = const_cast<const IPlace&>(*static_cast<IPlace*>(super::getHome()));
			return home;
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return const_cast<const IPlace&>(*h);
	}

	void setTelephoneNumber(int telephone) {
		setTelephone(telephone);
	}

	int getTelephoneNumber() {
		return getTelephone();
	}

	void setEmail(const string& email) {
		try {
			super::setEmail(email);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	const string& getEmail() {
		const string& em = "";
		try {
			const string& email = super::getEmail();
			return email;
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return em;
	}

	void setReputation(int reputation) {
		setReputation(reputation);
	}

	int getReputation() {
		int reputation = 0;
		try {
			reputation = super::getReputation();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return reputation;
	}

	bool hasReputation() {
		return getReputation() > 0;
	}

	void setCars(vector<shared_ptr<ICar>> cars) {
		vector<Car*> c = vector<Car*>();
		for (auto i = cars.begin(); i != cars.end(); i++)
			c.push_back(static_pointer_cast<Car>(*i).get());
		super::setCars(c);
	}

	vector<shared_ptr<ICar>> getCars() {
		typedef vector<shared_ptr<ICar>> carVector;
		carVector cars = carVector();
		try {
			vector<Car*>& c = super::getCars();
			for (vector<Car*>::iterator i = c.begin(); i != c.end(); i++) {
				cars.push_back(shared_ptr<ICar>(static_cast<ICar*>(*i)));
			}
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return cars;
	}

	bool addCar(const ICar& car) {
		typedef vector<Car*> carVector;
		shared_ptr<Car> c = static_pointer_cast<Car>(Car().newInstance(shared_ptr<ICar>(&const_cast<ICar&>(car))));
		try {
			carVector& cars = super::getCars();
			cars.push_back(c.get());
			setCars(cars);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return true;
	}

	void clearCars() {
		typedef vector<Car*> carVector;
		carVector& cars = super::getCars();
		cars.clear();
		super::setCars(cars);
	}

	void decreaseReputation() {
		decreaseReputation(1);
	}

	void decreaseReputation(int decrease) {
		setReputation(hasReputation()
				? max(getReputation() - decrease, 0) : 0);
	}

	int getNCars() {
		return super::getCars().size();
	}

	void increaseReputation(int increase) {
		setReputation(hasReputation() ? getReputation() + increase : increase);
	}

	void increaseReputation() {
		increaseReputation(1);
	}

	bool removeCar(const ICar& co) {
		typedef vector<Car*> carVector;
		bool erase = false;
		ICar& carObject = const_cast<ICar&>(co);
		carVector& cars = super::getCars();
		for (carVector::iterator n = cars.begin(); n != cars.end(); n++) {
			if (!((*n)->getPlate().compare(carObject.getPlate()))) {
				cars.erase(n);
				erase = true;
			}
		}
		super::setCars(cars);
		return erase;
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

	string getSurname() const {
		string surname = "";
		try {
			surname = super::getSurname();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return surname;
	}

	::oracle::occi::Blob getAvatar() const {
		::oracle::occi::Blob avatar = ::oracle::occi::Blob();
		avatar.setNull();
		try {
			avatar = super::getAvatar();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return avatar;
	}

	Place* getHome() const {
		Place* home = NULL;
		try {
			home = super::getHome();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return home;
	}

	void setHome(Place* value) {
		try {
			super::setHome(value);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	::oracle::occi::Number getTelephone() const {
		::oracle::occi::Number telephone = 0;
		try {
			telephone = super::getTelephone();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return telephone;
	}

	void setTelephone(const ::oracle::occi::Number& value) {
		try {
			super::setTelephone(value);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	string getEmail() const {
		string email = "";
		try {
			email = super::getEmail();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return email;
	}

	::oracle::occi::Number getReputation() const {
		::oracle::occi::Number reputation = 0;
		try {
			reputation = super::getReputation();
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return reputation;
	}

	void setReputation(const ::oracle::occi::Number& value) {
		try {
			super::setReputation(value);
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	const vector<Car*>& getCars() const {
		const vector<Car*>& c = vector<Car*>();
		try {
			const vector<Car*>& cars = super::getCars();
			return cars;
		} catch(::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return c;
	}

	void setCars(const vector<Car*>& cars) {
		try {
			super::setCars(cars);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	static string getSqlTypeName() {
		return string("ANDROID.USER_TYP");
	}

private:

	typedef ::UserTyp super;
};
}
}
}

#endif /* IUSER_H_ */
