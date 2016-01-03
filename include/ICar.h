#ifndef ICAR_H_
#define ICAR_H_

#include <Ice/Current.h>
#include <Ice/Identity.h>
#include <Ice/ObjectAdapter.h>
#include <Ice/ObjectFactory.h>
#include <Ice/ProxyHandle.h>
#include <occiControl.h>
#include <occiData.h>
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>

#include "Cardroid.h"
#include "DbTypes.h"

using namespace std;

namespace Cardroid {
namespace Data {

/**
 * \interface ICar
 * Public operations interface for the implementation of a Car object
 */
class ICar {

public:

	/**
	 * \enum Fuel
	 * enum type depicting the different types of fuel a car can
	 * consume
	 */
	enum Fuel {
		UNLEADED95, UNLEADED98, DIESELA, DIESELAPLUS, BIODIESEL
	};

	/**
	 * Create an instance of the class implementing this interface,
	 * from the received parameter, which also implements it.
	 * This method acts as an Abstract Factory, for the sake of the
	 * implementation of the Abstract Factory pattern in the search of
	 * interoperability among the different implementations of this interface
	 * which could exist in the different subsystems of the platform
	 * @param carObject An object instance implementing this interface
	 * @return An instance of the class implementing this interface, containing
	 * 			exactly the same data of the received bitmapObject parameter,
	 * 			from the viewpoint of the operations defined in this interface
	 */
	virtual shared_ptr<ICar> newInstance(const shared_ptr<ICar> carObject) = 0;

	/**
	 * @param brand The new brand for this ICar
	 */
	virtual void setBrand(const string& brand) = 0;

	/**
	 * @return The brand for this ICar
	 */
	virtual const string& getBrand() = 0;

	/**
	 * @param model The new model for this ICar
	 */
	virtual void setModel(const string& model) = 0;

	/**
	 * @return The model for this ICar
	 */
	virtual const string& getModel() = 0;

	/**
	 * @param fuel The new fuel for this ICar
	 */
	virtual void setFuel(Fuel fuel) = 0;

	/**
	 * @return The fuel for this ICar
	 */
	virtual Fuel getFuel() = 0;

	/**
	 * @param consumptionPerKm The new consumption per kilometer rate
	 * 							for this ICar
	 */
	virtual void setConsumptionPerKm(double consumptionPerKm) = 0;

	/**
	 * @return The consumption per kilometer rate for this ICar
	 */
	virtual double getConsumptionPerKm() = 0;

	/**
	 * @param nSeats The new number of seats for this ICar
	 */
	virtual void setNSeats(int nSeats) = 0;

	/**
	 * @return The number of seats for this ICar
	 */
	virtual int getNSeats() = 0;

	/**
	 * @param color The new color for this ICar
	 */
	virtual void setColor(const string& color) = 0;

	/**
	 * @return The color for this ICar
	 */
	virtual const string& getColor() = 0;

	/**
	 * @param plate The new plate for this ICar
	 */
	virtual void setPlate(const string& plate) = 0;

	/**
	 * @return The plate for this ICar
	 */
	virtual const string& getPlate() = 0;

	virtual ~ICar() {
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
class Car: public ::cardroid::data::zerocice::CarTyp,
		public ICar, public ::Ice::ObjectFactory {

public:

	Car() {
	}

	/**
	 * Default constructor
	 */
	Car(const string& b, const string& m, ::cardroid::data::zerocice::Fuel cf,
			double cons, int ns, const string& col, const string& p) {
		brand = b;
		model = m;
		carFuel = cf;
		consumptionPerKm = cons;
		nSeats = ns;
		color = col;
		plate = p;
	}

	Car(const ::cardroid::data::zerocice::CarTyp& car) {
		brand = car.brand;
		model = car.model;
		carFuel = car.carFuel;
		consumptionPerKm = car.consumptionPerKm;
		nSeats = car.nSeats;
		color = car.color;
		plate = car.plate;
	}

	Car(const Car& car) {
		brand = car.brand;
		model = car.model;
		carFuel = car.carFuel;
		consumptionPerKm = car.consumptionPerKm;
		nSeats = car.nSeats;
		color = car.color;
		plate = car.plate;
	}

	Car(Car&& car) {
		brand = car.brand;
		model = car.model;
		carFuel = car.carFuel;
		consumptionPerKm = car.consumptionPerKm;
		nSeats = car.nSeats;
		color = car.color;
		plate = car.plate;
	}

	~Car() {
	}

	Car& operator=(Car car) {
		brand = car.brand;
		model = car.model;
		carFuel = car.carFuel;
		consumptionPerKm = car.consumptionPerKm;
		nSeats = car.nSeats;
		color = car.color;
		plate = car.plate;
		return *this;
	}

	Car& operator=(Car& car) {
		swap(brand, car.brand);
		swap(model, car.model);
		swap(carFuel, car.carFuel);
		swap(consumptionPerKm, car.consumptionPerKm);
		swap(nSeats, car.nSeats);
		swap(color, car.color);
		swap(plate, car.plate);
		return *this;
	}

	/**
	 * @return An Ice Identity for this datatype category and the data provided
	 */
	static ::Ice::Identity createIdentity(const string& plate,
			const string& ownerEmail) {
		::Ice::Identity id;
		id.category = "car";
		id.name = plate + "@" + ownerEmail;
		return id;
	}

	/**
	 * @return A proxy to an Ice Object incarnating the provided data, whose
	 * 			servant is added to adapter 's active servant map
	 */
	static ::cardroid::data::zerocice::CarTypPrx createProxy(
			const string& ownerEmail,
			const ::cardroid::data::zerocice::CarTyp& carObject,
			::Ice::ObjectAdapterPtr adapter) {
		::cardroid::data::zerocice::CarTypPrx carPrx =
				::cardroid::data::zerocice::CarTypPrx::uncheckedCast(
						adapter->createProxy(
								createIdentity(carObject.getPlate(),
										ownerEmail)));
		fillProxyData(carObject, carPrx, adapter);
		return carPrx;
	}

	/**
	 * @return A proxy to an Ice Object incarnating the provided data, whose
	 * 			servant is added to adapter 's active servant map
	 */
	static ::cardroid::data::zerocice::CarTypPrx createProxy(
			const string& ownerEmail, const ICar& co,
			::Ice::ObjectAdapterPtr adapter) {
		ICar& carObject = const_cast<ICar&>(co);
		::cardroid::data::zerocice::CarTypPrx carPrx =
				::cardroid::data::zerocice::CarTypPrx::uncheckedCast(
						adapter->createProxy(
								createIdentity(carObject.getPlate(),
										ownerEmail)));
		fillProxyData(carObject, carPrx, adapter);
		return carPrx;
	}

	/**
	 * Fill the servant referenced by the given proxy with the data contained
	 * in the given object
	 */
	static void fillProxyData(const ::cardroid::data::zerocice::CarTyp& object,
			::cardroid::data::zerocice::CarTypPrx proxy,
			::Ice::ObjectAdapterPtr adapter) {
		proxy->setBrand(object.getBrand());
		proxy->setModel(object.getModel());
		proxy->setCarFuel(object.getCarFuel());
		proxy->setConsumptionPerKm(object.getConsumptionPerKm());
		proxy->setNSeats(object.getNSeats());
		proxy->setColor(object.getColor());
		proxy->setPlate(object.getPlate());
	}

	/**
	 * Fill the servant referenced by the given proxy with the data contained
	 * in the given object
	 */
	static void fillProxyData(const ICar& o,
			::cardroid::data::zerocice::CarTypPrx proxy,
			::Ice::ObjectAdapterPtr adapter) {
		ICar& object = const_cast<ICar&>(o);
		proxy->setBrand(object.getBrand());
		proxy->setModel(object.getModel());
		proxy->setCarFuel(
				static_cast< ::cardroid::data::zerocice::Fuel>(object.getFuel()));
		proxy->setConsumptionPerKm(object.getConsumptionPerKm());
		proxy->setNSeats(object.getNSeats());
		proxy->setColor(object.getColor());
		proxy->setPlate(object.getPlate());
	}

	/**
	 * @param proxy A proxy to a remote object implementing a Car
	 * @return A local Car object containing the data of the remote object
	 * 			referenced by proxy
	 */
	static shared_ptr<Car> extractObject(
			::cardroid::data::zerocice::CarTypPrx proxy) {
		return shared_ptr<Car>(
				new Car(proxy->getBrand(), proxy->getModel(),
						proxy->getCarFuel(), proxy->getConsumptionPerKm(),
						proxy->getNSeats(), proxy->getColor(),
						proxy->getPlate()));
	}

	/* ICar interface */

	shared_ptr<ICar> newInstance(shared_ptr<ICar> carObject) {
		if (!carObject)
			return NULL;
		if (dynamic_cast<Car*>(carObject.get()))
			return carObject;
		string brand = carObject->getBrand();
		string model = carObject->getModel();
		::cardroid::data::zerocice::Fuel fuel =
				static_cast< ::cardroid::data::zerocice::Fuel>(carObject->getFuel());
		double consumptionPerKm = carObject->getConsumptionPerKm();
		int nSeats = carObject->getNSeats();
		string color = carObject->getColor();
		string plate = carObject->getPlate();
		return static_pointer_cast<ICar>(make_shared<Car>(
				brand, model, fuel, consumptionPerKm, nSeats, color,
						plate));
	}

	void setBrand(const string& b) {
		brand = b;
	}

	const string& getBrand() {
		return brand;
	}

	void setModel(const string& m) {
		model = m;
	}

	const string& getModel() {
		return model;
	}

	void setFuel(ICar::Fuel fuel) {
		setCarFuel(static_cast< ::cardroid::data::zerocice::Fuel>(fuel));
	}

	ICar::Fuel getFuel() {
		return static_cast<ICar::Fuel>(getCarFuel());
	}

	void setConsumptionPerKm(double consumption) {
		consumptionPerKm = consumption;
	}

	double getConsumptionPerKm() {
		return consumptionPerKm;
	}

	void setNSeats(int seats) {
		nSeats = seats;
	}

	int getNSeats() {
		return nSeats;
	}

	void setColor(const string& c) {
		color = c;
	}

	const string& getColor() {
		return color;
	}

	void setPlate(const string& p) {
		plate = p;
	}

	const string& getPlate() {
		return plate;
	}

	/* Overriding superclass */

	static const string& ice_staticId() {
		return super::ice_staticId();
	}


	string getBrand(const ::Ice::Current& = ::Ice::Current()) const {
		return brand;
	}

	void setBrand(const string& b, const ::Ice::Current& = ::Ice::Current()) {
		brand = b;
	}

	string getModel(const ::Ice::Current& = ::Ice::Current()) const {
		return model;
	}

	void setModel(const string& m, const ::Ice::Current& = ::Ice::Current()) {
		model = m;
	}

	::cardroid::data::zerocice::Fuel getCarFuel(const ::Ice::Current& =
			::Ice::Current()) const {
		return carFuel;
	}

	void setCarFuel(::cardroid::data::zerocice::Fuel fuel,
			const ::Ice::Current& = ::Ice::Current()) {
		carFuel = fuel;
	}

	double getConsumptionPerKm(const ::Ice::Current& = ::Ice::Current()) const {
		return consumptionPerKm;
	}

	void setConsumptionPerKm(double consumption, const ::Ice::Current& =
			::Ice::Current()) {
		consumptionPerKm = consumption;
	}

	int getNSeats(const ::Ice::Current& = ::Ice::Current()) const {
		return nSeats;
	}

	void setNSeats(int seats, const ::Ice::Current& = ::Ice::Current()) {
		nSeats = seats;
	}

	string getColor(const ::Ice::Current& = ::Ice::Current()) const {
		return color;
	}

	void setColor(const string& c, const ::Ice::Current& = ::Ice::Current()) {
		color = c;
	}

	string getPlate(const ::Ice::Current& = ::Ice::Current()) const {
		return plate;
	}

	void setPlate(const string& p, const ::Ice::Current& = ::Ice::Current()) {
		plate = p;
	}

	string toString(const ::Ice::Current& = ::Ice::Current()) const {
		return brand + " " + model + " - " + plate;
	}

	/* ObjectFactory interface */

	::Ice::ObjectPtr create(const string& type) {
		if (!type.compare(ice_staticId()))
			return new Car;

		return NULL;
	}

	void destroy() {
	}

private:

	static const long long int serialVersionUID = -3328992844832771026L;
	typedef ::cardroid::data::zerocice::CarTyp super;
};
}

namespace Oracle {

using namespace Cardroid::Data;

/**
 * \class Car
 * Domain class implementing a Car for its storage and retrieval from an
 * Oracle database
 */
class Car: public ::CarTyp, public ICar {

public:

	Car() {
	}

	/**
	 * Default constructor
	 */
	Car(const string& brand, const string& model, const string& fuelName,
			double consumptionPerKm, int nSeats, const string& color,
			const string& plate) {
		setBrand(brand);
		setModel(model);
		ICar::Fuel fuel;
		if (!fuelName.compare("UNLEADED95"))
			fuel = UNLEADED95;
		if (!fuelName.compare("UNLEADED98"))
			fuel = UNLEADED98;
		if (!fuelName.compare("DIESELA"))
			fuel = DIESELA;
		if (!fuelName.compare("DIESELAPLUS"))
			fuel = DIESELAPLUS;
		if (!fuelName.compare("BIODIESEL"))
			fuel = BIODIESEL;
		setFuel(fuel);
		setConsumptionPerKm(consumptionPerKm);
		setNSeats(nSeats);
		setColor(color);
		setPlate(plate);
	}

	/* ICar interface */

	shared_ptr<ICar> newInstance(const shared_ptr<ICar> carObject) {
		if (!carObject)
			return NULL;
		if (dynamic_cast<Car*>(carObject.get()))
			return carObject;
		string brand = carObject->getBrand();
		string model = carObject->getModel();
		string fuelName;
		switch (carObject->getFuel()) {
		case UNLEADED95:
			fuelName = "UNLEADED95";
			break;
		case UNLEADED98:
			fuelName = "UNLEADED98";
			break;
		case DIESELA:
			fuelName = "DIESELA";
			break;
		case DIESELAPLUS:
			fuelName = "DIESELAPLUS";
			break;
		case BIODIESEL:
			fuelName = "BIODIESEL";
			break;
		}
		double consumptionPerKm = carObject->getConsumptionPerKm();
		int nSeats = carObject->getNSeats();
		string color = carObject->getColor();
		string plate = carObject->getPlate();
		return static_pointer_cast<ICar>(make_shared<Car>(
				brand, model, fuelName, consumptionPerKm, nSeats, color,
						plate));
	}

	void setBrand(const string& brand) {
		try {
			super::setBrand(brand);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	const string& getBrand() {
		const string& b = "";
		try {
			const string& brand = super::getBrand();
			return brand;
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return b;
	}

	void setModel(const string& model) {
		try {
			super::setModel(model);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	const string& getModel() {
		const string& m = "";
		try {
			const string& model = super::getModel();
			return model;
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return m;
	}

	void setFuel(ICar::Fuel fuel) {
		string fuelName;
		switch (fuel) {
		case UNLEADED95:
			fuelName = "UNLEADED95";
			break;
		case UNLEADED98:
			fuelName = "UNLEADED98";
			break;
		case DIESELA:
			fuelName = "DIESELA";
			break;
		case DIESELAPLUS:
			fuelName = "DIESELAPLUS";
			break;
		case BIODIESEL:
			fuelName = "BIODIESEL";
			break;
		}
		setFuel(fuelName);
	}

	ICar::Fuel getFuel() {
		ICar::Fuel fuel = ICar::Fuel::UNLEADED95;
		try {
			if (!super::getFuel().compare("UNLEADED95"))
				return fuel = ICar::Fuel::UNLEADED95;
			if (!super::getFuel().compare("UNLEADED98"))
				return fuel = ICar::Fuel::UNLEADED98;
			if (!super::getFuel().compare("DIESELA"))
				return fuel = ICar::Fuel::DIESELA;
			if (!super::getFuel().compare("DIESELAPLUS"))
				return fuel = ICar::Fuel::DIESELAPLUS;
			else
				return fuel = ICar::Fuel::BIODIESEL;
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return fuel;
	}

	void setConsumptionPerKm(double consumptionPerKm) {
		setConsumption_per_km(consumptionPerKm);
	}

	double getConsumptionPerKm() {
		return (double) getConsumption_per_km();
	}

	void setNSeats(int nSeats) {
		setN_seats(nSeats);
	}

	int getNSeats() {
		return getN_seats();
	}

	void setColor(const string& color) {
		try {
			super::setColor(color);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	const string& getColor() {
		const string& c = "";
		try {
			const string& color = super::getColor();
			return color;
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return c;
	}

	void setPlate(const string& plate) {
		try {
			super::setPlate(plate);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	const string& getPlate() {
		const string& p = "";
		try {
			const string& plate = super::getPlate();
			return plate;
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return p;
	}

	/* Overriding superclass */

	string getBrand() const {
		string brand = "";
		try {
			brand = super::getBrand();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return brand;
	}

	string getModel() const {
		string model = "";
		try {
			model = super::getModel();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return model;
	}

	string getFuel() const {
		string fuel = "";
		try {
			fuel = super::getFuel();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return fuel;
	}

	void setFuel(const string& value) {
		try {
			super::setFuel(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	::oracle::occi::Number getConsumption_per_km() const {
		::oracle::occi::Number consumptionPerKm = 0.;
		try {
			consumptionPerKm = super::getConsumption_per_km();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return consumptionPerKm;
	}

	void setConsumption_per_km(const ::oracle::occi::Number& value) {
		try {
			super::setConsumption_per_km(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	::oracle::occi::Number getN_seats() const {
		::oracle::occi::Number nSeats = 0;
		try {
			nSeats = super::getN_seats();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return nSeats;
	}

	void setN_seats(const ::oracle::occi::Number& value) {
		try {
			super::setN_seats(value);
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
	}

	string getColor() const {
		string color = "";
		try {
			color = super::getColor();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return color;
	}

	string getPlate() const {
		string plate = "";
		try {
			plate = super::getPlate();
		} catch (::oracle::occi::SQLException& sqle) {
			cerr << "SQLException: " << sqle.what() << endl;
		}
		return plate;
	}

	static string getSqlTypeName() {
		return string("ANDROID.CAR_TYP");
	}

private:

	typedef ::CarTyp super;
};
}
}
}

#endif /* ICAR_H_ */
