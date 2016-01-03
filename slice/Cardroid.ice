#pragma once

#include <Ice/BuiltinSequences.ice>

[["java:package:uclm.esi"]]
module cardroid {
	
	module data {
	
		module zerocice {
			
			["java:getset"]
			struct LatLngTyp {
				double latitude;
				double longitude;
			};
			
			sequence<byte> Blob;
			enum CompressFormat { JPEG, PNG, WEBP };
			enum Config { ALPHA8, ARGB4444, ARGB8888, RGB565 };
			
			["java:getset"]
			struct BitmapTyp {
				Blob bitmapBitmap;
				CompressFormat bitmapCompressFormat;
				Config bitmapConfig;
				int density;
			};
			
			class PlaceTyp {
				string name;
				LatLngTyp coords;
				optional(3) string placeDescription;
				optional(4) Blob placeSnapshotBytes;
				
				["cpp:const"] idempotent string getName();
				idempotent void setName(string name);
				["cpp:const"] idempotent LatLngTyp getCoords();
				idempotent void setCoords(LatLngTyp coords);
				["cpp:const"] idempotent string getDescription();
				idempotent void setDescription(string description);
				["cpp:const"] idempotent bool hasDescription();
				["cpp:const"] idempotent Blob getSnapshotBytes();
				idempotent void setSnapshotBytes(Blob snapshotBytes);
				["cpp:const"] idempotent bool hasSnapshot();
				["cpp:const"] idempotent string toString();
			};
			
			enum Fuel { UNLEADED95, UNLEADED98, DIESELA, DIESELAPLUS, BIODIESEL };
			
			class CarTyp {
				string brand;
				string model;
				Fuel carFuel;
				double consumptionPerKm;
				int nSeats;
				string color;
				string plate;
				
				["cpp:const"] idempotent string getBrand();
				idempotent void setBrand(string brand);
				["cpp:const"] idempotent string getModel();
				idempotent void setModel(string model);
				["cpp:const"] idempotent Fuel getCarFuel();
				idempotent void setCarFuel(Fuel carFuel);
				["cpp:const"] idempotent double getConsumptionPerKm();
				idempotent void setConsumptionPerKm(double consumptionPerKm);
				["cpp:const"] idempotent int getNSeats();
				idempotent void setNSeats(int nSeats);
				["cpp:const"] idempotent string getColor();
				idempotent void setColor(string color);
				["cpp:const"] idempotent string getPlate();
				idempotent void setPlate(string plate);
				["cpp:const"] idempotent string toString();
			};
			
			["java:type:java.util.ArrayList<CarTypPrx>:java.util.List<CarTypPrx>"]
			sequence<CarTyp*> CarCollection;
			
			class UserTyp {
				string name;
				string surname;
				optional(3) Blob userAvatarBytes;
				PlaceTyp home;
				int telephone;
				string email;
				optional(7) int userReputation;
				CarCollection cars;
				
				["cpp:const"] idempotent string getName();
				idempotent void setName(string name);
				["cpp:const"] idempotent string getSurname();
				idempotent void setSurname(string surname);
				["cpp:const"] idempotent Blob getAvatarBytes();
				idempotent void setAvatarBytes(Blob avatarBytes);
				["cpp:const"] idempotent bool hasAvatar();
				["cpp:const"] idempotent PlaceTyp getUserHome();
				idempotent void setUserHome(PlaceTyp home);
				["cpp:const"] idempotent int getTelephone();
				idempotent void setTelephone(int telephone);
				["cpp:const"] idempotent string getEmail();
				idempotent void setEmail(string email);
				["cpp:const"] idempotent int getReputation();
				idempotent void setReputation(int reputation);
				["cpp:const"] idempotent bool hasReputation();
				["cpp:const"] idempotent CarCollection getUserCars();
				idempotent void setUserCars(CarCollection cars);
				["cpp:const"] idempotent string toString();
				
				void increaseReputation1();
				void increaseReputation(int increase);
				void decreaseReputation1();
				void decreaseReputation(int decrease);
				bool addCar(CarTyp c);
				bool removeCar(CarTyp c);
				idempotent int getNCars();
				idempotent void clearCars();
			};
			
			class DateTyp {
				long datetime;
				
				["cpp:const"] idempotent long getTimeInMillis();
				idempotent void setTimeInMillis(long datetime);
				["cpp:const"] idempotent string toString();
			};
			
			class DateTimeTyp extends DateTyp {
			};
			
			enum TimePreferences { ANY, MORNING, AFTERNOON, NIGHT };
			
			class DateTimePrefsTyp extends DateTyp {
				int toleranceDays;
				TimePreferences timePrefs;
				
				["cpp:const"] idempotent int getToleranceDays();
				idempotent void setToleranceDays(int toleranceDays);
				["cpp:const"] idempotent TimePreferences getTimePrefs();
				idempotent void setTimePrefs(TimePreferences timePrefs);
			};
			
			sequence<string> WeekdaysV;
			
			enum Periodicity { EVERYWEEK, EVENWEEKS, ODDWEEKS };
			
			class TripTyp {
				int tripId;
				PlaceTyp fromPlace;
				PlaceTyp toPlace;
				DateTyp tDate;
				int nSeats;
				optional(6) DateTyp tReturnDate;
				optional(7) WeekdaysV tWeekDays;
				//optional(7) string weekDays;
				optional(8) Periodicity tPeriodicity;
				optional(9) int tripDistance;
				optional(10) string tripCharacteristics;
				int tripType;
				
				["cpp:const"] idempotent int getTripId();
				idempotent void setTripId(int tripId);
				["cpp:const"] idempotent PlaceTyp getPlace1();
				idempotent void setPlace1(PlaceTyp fromPlace);
				["cpp:const"] idempotent PlaceTyp getPlace2();
				idempotent void setPlace2(PlaceTyp toPlace);
				["cpp:const"] idempotent DateTyp getTripDate();
				idempotent void setTripDate(DateTyp tripDate);
				["cpp:const"] idempotent int getNSeats();
				idempotent void setNSeats(int nSeats);
				["cpp:const"] idempotent DateTyp getTripReturnDate();
				idempotent void setTripReturnDate(DateTyp tripReturnDate);
				["cpp:const"] idempotent bool hasTripReturnDate();
				["cpp:const"] idempotent WeekdaysV getTripWeekDays();
				["cpp:const"] idempotent Periodicity getTripPeriodicity();
				idempotent void setTripWeekDaysPeriodicity(WeekdaysV tripWeekDays, Periodicity tripPeriodicity);
				["cpp:const"] idempotent bool hasWeekDaysPeriodicity();
				["cpp:const"] idempotent int getDistance();
				idempotent void setDistance(int distance);
				["cpp:const"] idempotent bool hasDistance();
				["cpp:const"] idempotent string getCharacteristics();
				idempotent void setCharacteristics(string characteristics);
				["cpp:const"] idempotent bool hasCharacteristics();
				idempotent void setTripType(int type);
				["cpp:const"] idempotent int getTripType();
				["cpp:const"] idempotent string toString();
			};
			
			class WaypointTyp {
				int nOrder;
				PlaceTyp waypointPlace;
				
				["cpp:const"] idempotent int getNOrder();
				idempotent void setNOrder(int nOrder);
				["cpp:const"] idempotent PlaceTyp getWaypointPlace();
				idempotent void setWaypointPlace(PlaceTyp waypointPlace);
				["cpp:const"] idempotent string toString();
			};
			
			["java:type:java.util.ArrayList<WaypointTyp>:java.util.List<WaypointTyp>"]
			sequence<WaypointTyp> WaypointCollection;
			
			class PassengerTyp {
				UserTyp* passengerUser;
				int nSeats;
				
				["cpp:const"] idempotent UserTyp* getPassengerUser();
				idempotent void setPassengerUser(UserTyp* passengerUser);
				["cpp:const"] idempotent int getNSeats();
				idempotent void setNSeats(int nSeats);
				["cpp:const"] idempotent string toString();
			};
			
			["java:type:java.util.ArrayList<PassengerTyp>:java.util.List<PassengerTyp>"]
			sequence<PassengerTyp> PassengerCollection;
			
			sequence<string> AllowedV;
			
			class TripOfferTyp extends TripTyp {
				UserTyp* driver;
				WaypointCollection waypoints;
				CarTyp* tripOfferCar;
				PassengerCollection passengers;
				double price;
				AllowedV allowed;
				//string allowed;
				DateTimeTyp tDateTime;
				optional(11) DateTimeTyp tReturnDateTime;
				
				["cpp:const"] idempotent DateTimeTyp getTripDateTime();
				idempotent void setTripDateTime(DateTimeTyp dt);
				["cpp:const"] idempotent DateTimeTyp getTripReturnDateTime();
				idempotent void setTripReturnDateTime(DateTimeTyp rdt);
				["cpp:const"] idempotent UserTyp* getTripDriver();
				idempotent void setTripDriver(UserTyp* driver);
				["cpp:const"] idempotent WaypointCollection getTripWaypoints();
				idempotent void setTripWaypoints(WaypointCollection waypoints);
				["cpp:const"] idempotent CarTyp* getTripCar();
				idempotent void setTripCar(CarTyp* tripCar);
				["cpp:const"] idempotent PassengerCollection getTripPassengers();
				idempotent void setTripPassengers(PassengerCollection passengers);
				["cpp:const"] idempotent double getPrice();
				idempotent void setPrice(double price);
				["cpp:const"] idempotent AllowedV getAllowedFeatures();
				idempotent void setAllowedFeatures(AllowedV allowed);
				
				bool addTripWaypoint(PlaceTyp waypoint);
				bool removeTripWaypoint(int pos);
				idempotent int getNTripWaypoints();
				idempotent void clearTripWaypoints();
				bool addTripPassenger(UserTyp* passenger, int seats);
				bool removeTripPassenger(UserTyp* passenger);
				idempotent int getNTripPassengers();
				idempotent void clearTripPassengers();
			};
			
			class TripRequestTyp extends TripTyp {
				UserTyp* requester;
				DateTimePrefsTyp tDateTimePrefs;
				optional(11) DateTimePrefsTyp tReturnDateTimePrefs;
				
				["cpp:const"] idempotent DateTimePrefsTyp getTripDateTimePrefs();
				idempotent void setTripDateTimePrefs(DateTimePrefsTyp dtp);
				["cpp:const"] idempotent DateTimePrefsTyp getTripReturnDateTimePrefs();
				idempotent void setTripReturnDateTimePrefs(DateTimePrefsTyp rdtp);
				["cpp:const"] idempotent UserTyp* getTripRequester();
				idempotent void setTripRequester(UserTyp* requester);
			};
			
			enum ActivityType { TRIPJOIN, TRIPREQUESTANSWERED, TRIPACCEPT, TRIPREFUSE };
			
			class UserActivityTyp {
				UserTyp* activityUser;
				TripOfferTyp* activityTrip;
				ActivityType userActivityType;
				long timeStamp;
				
				["cpp:const"] idempotent UserTyp* getActivityUser();
				idempotent void setActivityUser(UserTyp* activityUser);
				["cpp:const"] idempotent TripOfferTyp* getActivityTrip();
				idempotent void setActivityTrip(TripOfferTyp* activityTrip);
				["cpp:const"] idempotent ActivityType getUserActivityType();
				idempotent void setUserActivityType(ActivityType userActivityType);
				["cpp:const"] idempotent long getTimeStampInMillis();
				idempotent void setTimeStampInMillis(long timeStampMillis);
				["cpp:const"] idempotent string toString();
			};
			
			class MessageTyp {
				UserTyp* user1;
				UserTyp* user2;
				string msg;
				long timeStamp;
				
				["cpp:const"] idempotent UserTyp* getUser1();
				idempotent void setUser1(UserTyp* user1);
				["cpp:const"] idempotent UserTyp* getUser2();
				idempotent void setUser2(UserTyp* user2);
				["cpp:const"] idempotent string getMessageText();
				idempotent void setMessageText(string msg);
				["cpp:const"] idempotent long getTimeStampInMillis();
				idempotent void setTimeStampInMillis(long timeStampMillis);
				["cpp:const"] idempotent string toString();
			};
		};
	};
	
	["java:type:java.util.LinkedList<Ice.ObjectPrx>:java.util.List<Ice.ObjectPrx>"]
	sequence<Object*> ResultSeq;
	
	/**
 	 *
 	 * Interface to get query results.
 	 *
 	 **/
	interface QueryResult
	{
	    /**
     	 *
     	 * Get more query results.
     	 *
     	 * @param n The maximum number of results to return.
     	 *
     	 * @param destroyed There are no more results, and the query has
     	 * been destroyed.
     	 *
     	 * @returns A sequence of up to n results.
     	 *
     	 **/
		ResultSeq next(int n, out bool destroyed);
		
		/**
     	 *
     	 * Destroy the query result.
     	 *
     	 **/
    	void destroy();
	};
	
	module zerocice {
			
		interface CardroidManager {
			idempotent void getUserPlaces(cardroid::data::zerocice::UserTyp* usr, int n, out ResultSeq first, out int nrows, out QueryResult* result);
			idempotent void searchTrips(cardroid::data::zerocice::TripRequestTyp tRequest, int n, out ResultSeq first, out int nrows, out QueryResult* result);
			idempotent cardroid::data::zerocice::TripTyp* getTripFromId(int tripId);
			idempotent cardroid::data::zerocice::TripOfferTyp* getTripOfferFromId(int tripId);
			idempotent cardroid::data::zerocice::TripRequestTyp* getTripRequestFromId(int tripId);
			void joinTrip(cardroid::data::zerocice::TripOfferTyp* trip, cardroid::data::zerocice::UserTyp* passenger, int nSeats);
			cardroid::data::zerocice::TripOfferTyp* organizeTrip(cardroid::data::zerocice::TripRequestTyp* tripRequest, cardroid::data::zerocice::TripOfferTyp tripOffer); 
			idempotent void getUserTrips(cardroid::data::zerocice::UserTyp* usr, int n, out ResultSeq first, out int nrows, out QueryResult* result);
			idempotent void getPassengerTrips(cardroid::data::zerocice::UserTyp* passenger, int n, out ResultSeq first, out int nrows, out QueryResult* result);
			idempotent bool userTripRegistered(cardroid::data::zerocice::UserTyp* usr, cardroid::data::zerocice::TripTyp* trip);
			["cpp:const"] idempotent double calculatePriceEstimation(cardroid::data::zerocice::Fuel f, int distance);
			cardroid::data::zerocice::TripOfferTyp* newTripOffer(cardroid::data::zerocice::TripOfferTyp tOffer);
			cardroid::data::zerocice::TripRequestTyp* newTripRequest(cardroid::data::zerocice::TripRequestTyp tRequest);
			idempotent void getMessageTalksSpeakers(cardroid::data::zerocice::UserTyp* usr, int n, out ResultSeq first, out int nrows, out QueryResult* result);
			idempotent void getMessageTalks(cardroid::data::zerocice::UserTyp* usr1, cardroid::data::zerocice::UserTyp* usr2, int n, out ResultSeq first, out int nrows, out QueryResult* result);
			cardroid::data::zerocice::MessageTyp* newMessage(cardroid::data::zerocice::UserTyp* usr1, cardroid::data::zerocice::UserTyp* usr2, string message);
			idempotent void getUserActivity(cardroid::data::zerocice::UserTyp* usr, int n, out ResultSeq first, out int nrows, out QueryResult* result);
			idempotent cardroid::data::zerocice::UserTyp* getUserFromEmail(string email);
			idempotent cardroid::data::zerocice::CarTyp* getCarFromPlate(string plate, cardroid::data::zerocice::UserTyp* owner);
			idempotent cardroid::data::zerocice::CarTyp* getCarFromPlateEmail(string plate, string ownerEmail);
			idempotent cardroid::data::zerocice::UserTyp* updateUserData(cardroid::data::zerocice::UserTyp usr);
			idempotent cardroid::data::zerocice::CarTyp* updateCarData(cardroid::data::zerocice::CarTyp car, cardroid::data::zerocice::UserTyp usr);
			idempotent cardroid::data::zerocice::CarTyp* updateCarDataEmail(cardroid::data::zerocice::CarTyp car, string usrEmail);
			cardroid::data::zerocice::CarTyp* addCar(cardroid::data::zerocice::CarTyp car, cardroid::data::zerocice::UserTyp* usr);
			cardroid::data::zerocice::CarTyp* addCarEmail(cardroid::data::zerocice::CarTyp car, string usrEmail);
			void removeCar(cardroid::data::zerocice::CarTyp* car, cardroid::data::zerocice::UserTyp* usr);
			void removeCarPlateEmail(string plate, string ownerEmail);
		};
		
		/**
	 	*
	 	* This local exception is used internally if a SQLException
	 	* is raised.
	 	*
	 	**/
		local exception OCCIException
		{
		};
	};
};