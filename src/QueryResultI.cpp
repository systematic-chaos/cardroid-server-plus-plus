#include "QueryResultI.h"

#include <Ice/Config.h>
#include <Ice/Current.h>
#include <Ice/LocalException.h>
#include <Ice/ObjectAdapter.h>
#include <forward_list>

using namespace std;
using namespace Cardroid::Network::Server;

boost::mutex QueryResultI::_synchronized;

QueryResultI::QueryResultI(SQLRequestContext& context,
		::oracle::occi::ResultSet* rs, ::Ice::ObjectAdapterPtr adapter,
		QueryType queryType) {
	// Cache the first entries of the query result
	_data = stack< ::Ice::ObjectPrx>();
	for (int i = 0;
			i < MAX_ENTRY_QUERY_RESULT
					&& rs->next()
							== ::oracle::occi::ResultSet::Status::DATA_AVAILABLE;
			++i) {
		switch (queryType) {
			case QueryType::GET_MESSAGE_TALKS:
			{
				::Cardroid::Data::Oracle::Message* messageTalks =
						static_cast< ::Cardroid::Data::Oracle::Message*>(rs->getObject(1));
				_data.push(
						::Cardroid::Data::Ice::Message::createProxy(*messageTalks, adapter));
				break;
			}
			case QueryType::GET_MESSAGE_TALKS_SPEAKERS:
			{
				::Cardroid::Data::Oracle::User* messageTalksSpeakers =
						static_cast< ::Cardroid::Data::Oracle::User*>(rs->getObject(1));
				_data.push(::Cardroid::Data::Ice::User::createProxy(*messageTalksSpeakers, adapter));
				break;
			}
			case QueryType::GET_PASSENGER_TRIPS:
			{
				::Cardroid::Data::Oracle::TripOffer* passengerTrips =
						static_cast< ::Cardroid::Data::Oracle::TripOffer*>(rs->getObject(1));
				_data.push(
						::Cardroid::Data::Ice::TripOffer::createProxy(*passengerTrips,
								adapter));
				break;
			}
			case QueryType::GET_USER_ACTIVITY:
			{
				::Cardroid::Data::Oracle::UserActivity* userActivity =
						static_cast< ::Cardroid::Data::Oracle::UserActivity*>(rs->getObject(1));
				_data.push(
						::Cardroid::Data::Ice::UserActivity::createProxy(*userActivity, adapter));
				break;
			}
			case QueryType::GET_USER_PLACES:
			{
				::Cardroid::Data::Oracle::Place* userPlaces =
						static_cast< ::Cardroid::Data::Oracle::Place*>(rs->getObject(1));
				_data.push(
						::Cardroid::Data::Ice::Place::createProxy(*userPlaces, adapter));
				break;
			}
			case QueryType::GET_USER_TRIPS:
			{
				::Cardroid::Data::Oracle::Trip* userTrips =
						static_cast< ::Cardroid::Data::Oracle::Trip*>(rs->getObject(1));
				_data.push(::Cardroid::Data::Ice::Trip::createProxy(*userTrips, adapter));
				break;
			}
			case QueryType::SEARCH_TRIPS:
			{
				::Cardroid::Data::Oracle::Trip* trips =
						static_cast< ::Cardroid::Data::Oracle::Trip*>(rs->getObject(1));
				_data.push(::Cardroid::Data::Ice::Trip::createProxy(*trips, adapter));
				break;
			}
		}
	}
}

cardroid::ResultSeq QueryResultI::next(::Ice::Int n, bool& destroyed,
		const ::Ice::Current &current) {
	_synchronized.lock();
	if (_destroyed) {
		throw ::Ice::ObjectNotExistException(__FILE__, __LINE__);
	}
	destroyed = false;
	vector< ::Ice::ObjectPrx> nextResultSeq;
	if (n <= 0) {
		nextResultSeq = vector< ::Ice::ObjectPrx>();
		return nextResultSeq;
	}

	forward_list< ::Ice::ObjectPrx> l = forward_list< ::Ice::ObjectPrx>();
	for (int i = 0; i < n && _data.size() > 0; ++i) {
		l.push_front(_data.top());
		_data.pop();
	}
	l.reverse();
	nextResultSeq = vector< ::Ice::ObjectPrx>(l.begin(), ++l.end());

	if (_data.size() <= 0) {
		try {
			destroyed = true;
			destroy(current);
		} catch (...) {
		}
	}

	_synchronized.unlock();
	return nextResultSeq;
}

void QueryResultI::destroy(const ::Ice::Current& current) {
	_synchronized.lock();
	if (_destroyed) {
		throw ::Ice::ObjectNotExistException(__FILE__, __LINE__);
	}
	_destroyed = true;

	current.adapter->remove(current.id);
	_synchronized.unlock();
}

void QueryResultI::shutdown() {
	_synchronized.lock();
	if (!_destroyed) {
		_destroyed = true;
	}
	_synchronized.unlock();
}
