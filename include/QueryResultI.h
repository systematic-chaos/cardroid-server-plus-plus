#ifndef QUERYRESULTI_H_
#define QUERYRESULTI_H_

#include <boost/thread/mutex.hpp>
#include <Ice/Ice.h>
#include <Ice/ProxyHandle.h>
#include <occiCommon.h>
#include <occiControl.h>
#include <occiData.h>
#include <stack>

#include "Cardroid.h"
#include "SQLRequestContext.h"
#include "QueryType.h"

namespace Cardroid {
namespace Network {
namespace Server {

class SQLRequestContext;

class QueryResultI: public cardroid::QueryResult {

private:

	std::stack< ::Ice::ObjectPrx> _data;
	bool _destroyed = false;
	static const int MAX_ENTRY_QUERY_RESULT = 1000;
	static const long long int serialVersionUID = 8433958483272712676L;
	static ::boost::mutex _synchronized;

public:

	QueryResultI(SQLRequestContext& context, ::oracle::occi::ResultSet* rs,
			::Ice::ObjectAdapterPtr adapter, QueryType queryType);
	cardroid::ResultSeq next(::Ice::Int n, bool& destroyed,
			const ::Ice::Current& current);
	void destroy(const ::Ice::Current& current);
	void shutdown();
};
}
}
}

#endif /* QUERYRESULTI_H_ */
