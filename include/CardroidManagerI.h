#ifndef CARDROIDMANAGERI_H_
#define CARDROIDMANAGERI_H_

#include <boost/thread/mutex.hpp>
#include <Ice/Config.h>
#include <Ice/Current.h>
#include <forward_list>
#include <string>

#include "Cardroid.h"
#include "QueryResultI.h"
#include "QueryType.h"
#include "SQLRequestContext.h"

using namespace std;

namespace Cardroid {
namespace Network {
namespace Server {

class QueryProxyPair;

/**
 * \class CardroidManagerI
 * This is a per-session CarDroid manager object servant
 */
class CardroidManagerI: public cardroid::zerocice::CardroidManager,
	public QueryResultListener {

public:

	CardroidManagerI();
	void getUserPlaces(const ::cardroid::data::zerocice::UserTypPrx&,
			::Ice::Int, ::cardroid::ResultSeq&, ::Ice::Int&,
			::cardroid::QueryResultPrx&, const ::Ice::Current& =
					::Ice::Current()) override;
	void searchTrips(const ::cardroid::data::zerocice::TripRequestTypPtr&,
			::Ice::Int, ::cardroid::ResultSeq&, ::Ice::Int&,
			::cardroid::QueryResultPrx&, const ::Ice::Current& =
					::Ice::Current()) override;
	::cardroid::data::zerocice::TripTypPrx getTripFromId(::Ice::Int,
			const ::Ice::Current& = ::Ice::Current()) override;
	::cardroid::data::zerocice::TripOfferTypPrx getTripOfferFromId(::Ice::Int,
			const ::Ice::Current& = ::Ice::Current()) override;
	::cardroid::data::zerocice::TripRequestTypPrx getTripRequestFromId(
			::Ice::Int, const ::Ice::Current& = ::Ice::Current()) override;
	void joinTrip(const ::cardroid::data::zerocice::TripOfferTypPrx&,
			const ::cardroid::data::zerocice::UserTypPrx&, ::Ice::Int,
			const ::Ice::Current& = ::Ice::Current()) override;
	::cardroid::data::zerocice::TripOfferTypPrx organizeTrip(
			const ::cardroid::data::zerocice::TripRequestTypPrx&,
			const ::cardroid::data::zerocice::TripOfferTypPtr&,
			const ::Ice::Current& = ::Ice::Current()) override;
	void getUserTrips(const ::cardroid::data::zerocice::UserTypPrx&, ::Ice::Int,
			::cardroid::ResultSeq&, ::Ice::Int&, ::cardroid::QueryResultPrx&,
			const ::Ice::Current& = ::Ice::Current()) override;
	void getPassengerTrips(const ::cardroid::data::zerocice::UserTypPrx&,
			::Ice::Int, ::cardroid::ResultSeq&, ::Ice::Int&,
			::cardroid::QueryResultPrx&, const ::Ice::Current& =
					::Ice::Current()) override;
	bool userTripRegistered(const ::cardroid::data::zerocice::UserTypPrx&,
			const ::cardroid::data::zerocice::TripTypPrx&,
			const ::Ice::Current& = ::Ice::Current()) override;
	::cardroid::data::zerocice::TripOfferTypPrx newTripOffer(
			const ::cardroid::data::zerocice::TripOfferTypPtr&,
			const ::Ice::Current& = ::Ice::Current()) override;
	::cardroid::data::zerocice::TripRequestTypPrx newTripRequest(
			const ::cardroid::data::zerocice::TripRequestTypPtr&,
			const ::Ice::Current& = ::Ice::Current()) override;
	void getMessageTalksSpeakers(const ::cardroid::data::zerocice::UserTypPrx&,
			::Ice::Int, ::cardroid::ResultSeq&, ::Ice::Int&,
			::cardroid::QueryResultPrx&, const ::Ice::Current& =
					::Ice::Current()) override;
	void getMessageTalks(const ::cardroid::data::zerocice::UserTypPrx&,
			const ::cardroid::data::zerocice::UserTypPrx&, ::Ice::Int,
			::cardroid::ResultSeq&, ::Ice::Int&, ::cardroid::QueryResultPrx&,
			const ::Ice::Current& = ::Ice::Current()) override;
	::cardroid::data::zerocice::MessageTypPrx newMessage(
			const ::cardroid::data::zerocice::UserTypPrx&,
			const ::cardroid::data::zerocice::UserTypPrx&, const ::std::string&,
			const ::Ice::Current& = ::Ice::Current()) override;
	void getUserActivity(const ::cardroid::data::zerocice::UserTypPrx&,
			::Ice::Int, ::cardroid::ResultSeq&, ::Ice::Int&,
			::cardroid::QueryResultPrx&, const ::Ice::Current& =
					::Ice::Current()) override;
	::cardroid::data::zerocice::UserTypPrx getUserFromEmail(
			const ::std::string&, const ::Ice::Current& = ::Ice::Current())
					override;
	::cardroid::data::zerocice::CarTypPrx getCarFromPlate(const ::std::string&,
			const ::cardroid::data::zerocice::UserTypPrx&,
			const ::Ice::Current& = ::Ice::Current()) override;
	::cardroid::data::zerocice::CarTypPrx getCarFromPlateEmail(
			const ::std::string&, const ::std::string&, const ::Ice::Current& =
					::Ice::Current()) override;
	::cardroid::data::zerocice::UserTypPrx updateUserData(
			const ::cardroid::data::zerocice::UserTypPtr&,
			const ::Ice::Current& = ::Ice::Current()) override;
	::cardroid::data::zerocice::CarTypPrx updateCarData(
			const ::cardroid::data::zerocice::CarTypPtr&,
			const ::cardroid::data::zerocice::UserTypPtr&,
			const ::Ice::Current& = ::Ice::Current()) override;
	::cardroid::data::zerocice::CarTypPrx updateCarDataEmail(
			const ::cardroid::data::zerocice::CarTypPtr&, const ::std::string&,
			const ::Ice::Current& = ::Ice::Current()) override;
	::cardroid::data::zerocice::CarTypPrx addCar(
			const ::cardroid::data::zerocice::CarTypPtr&,
			const ::cardroid::data::zerocice::UserTypPrx&,
			const ::Ice::Current& = ::Ice::Current()) override;
	::cardroid::data::zerocice::CarTypPrx addCarEmail(
			const ::cardroid::data::zerocice::CarTypPtr&, const ::std::string&,
			const ::Ice::Current& = ::Ice::Current()) override;
	void removeCar(const ::cardroid::data::zerocice::CarTypPrx&,
			const ::cardroid::data::zerocice::UserTypPrx&,
			const ::Ice::Current& = ::Ice::Current()) override;
	void removeCarPlateEmail(const ::std::string&, const ::std::string&,
			const ::Ice::Current& = ::Ice::Current()) override;
	::Ice::Double calculatePriceEstimation(::cardroid::data::zerocice::Fuel,
				::Ice::Int, const ::Ice::Current& = ::Ice::Current()) const;
	void destroy();
	void shutdown();
	void add(const ::cardroid::QueryResultPrx& proxy, QueryResultI* impl,
			QueryType queryType);

private:

	forward_list<QueryProxyPair> _queries;
	bool _destroyed;
	static const long long int serialVersionUID = 1290124593860850175L;
	static ::boost::mutex _synchronized;

	void reapQueries();
};

/**
 * \class QueryProxyPair
 * Class containing a proxy to a query result and the servant it references
 */
class QueryProxyPair {
public:

	cardroid::QueryResultPrx proxy;
	QueryResultI* impl;

	QueryProxyPair(const cardroid::QueryResultPrx& p, QueryResultI* i);
};
}
}
}

#endif /* CARDROIDMANAGERI_H_ */
