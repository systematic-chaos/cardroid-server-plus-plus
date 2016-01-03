#ifndef DBTYPESMAP_ORACLE
# include "DbTypesMap.h"
#endif

void DbTypesMap(oracle::occi::Environment* envOCCI_) {
	oracle::occi::Map *mapOCCI_ = envOCCI_->getMap();
	mapOCCI_->put("ANDROID.BITMAP_TYP", &BitmapTyp::readSQL,
			&BitmapTyp::writeSQL);
	mapOCCI_->put("ANDROID.CAR_TYP", &CarTyp::readSQL, &CarTyp::writeSQL);
	mapOCCI_->put("ANDROID.DATE_TIME_PREFS_TYP", &DateTimePrefsTyp::readSQL,
			&DateTimePrefsTyp::writeSQL);
	mapOCCI_->put("ANDROID.DATE_TIME_TYP", &DateTimeTyp::readSQL,
			&DateTimeTyp::writeSQL);
	mapOCCI_->put("ANDROID.DATE_TYP", &DateTyp::readSQL, &DateTyp::writeSQL);
	mapOCCI_->put("ANDROID.LATLNG_TYP", &LatLngTyp::readSQL,
			&LatLngTyp::writeSQL);
	mapOCCI_->put("ANDROID.MESSAGE_TYP", &MessageTyp::readSQL,
			&MessageTyp::writeSQL);
	mapOCCI_->put("ANDROID.PASSENGER_TYP", &PassengerTyp::readSQL,
			&PassengerTyp::writeSQL);
	mapOCCI_->put("ANDROID.PLACE_TYP", &PlaceTyp::readSQL, &PlaceTyp::writeSQL);
	mapOCCI_->put("ANDROID.TRIP_OFFER_TYP", &TripOfferTyp::readSQL,
			&TripOfferTyp::writeSQL);
	mapOCCI_->put("ANDROID.TRIP_REQUEST_TYP", &TripRequestTyp::readSQL,
			&TripRequestTyp::writeSQL);
	mapOCCI_->put("ANDROID.TRIP_TYP", &TripTyp::readSQL, &TripTyp::writeSQL);
	mapOCCI_->put("ANDROID.USER_ACTIVITY_TYP", &UserActivityTyp::readSQL,
			&UserActivityTyp::writeSQL);
	mapOCCI_->put("ANDROID.USER_TYP", &UserTyp::readSQL, &UserTyp::writeSQL);
	mapOCCI_->put("ANDROID.WAYPOINT_TYP", &WaypointTyp::readSQL,
			&WaypointTyp::writeSQL);
}
