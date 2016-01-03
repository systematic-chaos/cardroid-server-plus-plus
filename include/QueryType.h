#ifndef QUERYTYPE_H_
#define QUERYTYPE_H_

namespace Cardroid {
namespace Network {
namespace Server {

/**
 * \enum QueryType
 * The query types available to execute on CarDroid's database
 */
enum class QueryType {
	GET_USER_PLACES,
	SEARCH_TRIPS,
	GET_USER_TRIPS,
	GET_PASSENGER_TRIPS,
	GET_MESSAGE_TALKS,
	GET_MESSAGE_TALKS_SPEAKERS,
	GET_USER_ACTIVITY
};
}
}
}

#endif /* QUERYTYPE_H_ */
