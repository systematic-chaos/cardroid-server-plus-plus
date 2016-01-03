#include "SQLRequestContext.h"

#include <forward_list>
#include <Ice/Handle.h>
#include <Ice/ObjectAdapter.h>
#include <Ice/ProxyHandle.h>
#include <IceUtil/Handle.h>
#include <IceUtil/Optional.h>
#include <occiCommon.h>
#include <occiControl.h>
#include <occiData.h>
#include <cstdarg>
#include <iostream>
#include <memory>
#include <utility>

using namespace std;
using namespace Cardroid::Network::Server;
using namespace ::Ice;
using namespace ::oracle::occi;

unordered_map< boost::thread::id, SQLRequestContext*> SQLRequestContext::_contextMap;
LoggerPtr SQLRequestContext::_logger;
Cardroid::occi::StatelessConnectionPoolHolder* SQLRequestContext::_pool;
IceStorm::TopicManagerPrx SQLRequestContext::_topicManager;

/**
 * Default constructor.
 * Every instance of SQLRequestContext will hold a single database
 * connection. Initialization of SQLRequestContext must take place
 * in the static method initialize, since every instance of
 * SQLRequestContext will share static resources
 */
SQLRequestContext::SQLRequestContext() :
		_conn(new ::Cardroid::occi::ConnectionHolder(_pool)),
		_queryResultListener(NULL) {
	if (_trace)
		_logger->trace("SQLRequestContext",
				"create new context: this thread: ::boost::this_thread::get_id(): connection: _conn->connection()");

	_statements = forward_list< ::Cardroid::occi::StatementHolder*>();
	_contextMap.insert(
			pair< ::boost::thread::id, SQLRequestContext*>(
					::boost::this_thread::get_id(), this));
}

/**
 * Set the object whose implementation will be called upon the completion
 * of a query
 * @param listener The new listener for the processing of queries results
 */
void SQLRequestContext::setQueryResultListener(
		const QueryResultListener& listener) {
	_queryResultListener = &const_cast<QueryResultListener&>(listener);
}

/**
 * Set the object whose implementation will be called upon the completion
 * of a query
 * @param listener The new listener for the processing of queries results
 */
void SQLRequestContext::destroyFromDispatch(bool commit) {
	// Remove the currentEntry context from the thread -> context map
	if (_contextMap.erase(::boost::this_thread::get_id()))
		destroyInternal(commit);
}

/**
 * Commit or roll back the transaction performed.
 * Release all the resources kept by this SQLRequestContext
 * @param commit Whether the transaction performed will be commited or
 * 					rolled back
 */
void SQLRequestContext::destroyInternal(bool commit) {
	// Release all resources
	try {
		if (commit) {
			_conn->commit();
			if (_trace)
				_logger->trace("SQLRequestContext",
						"commit context: this");
		} else {
			_conn->rollback();
			if (_trace)
				_logger->trace("SQLRequestContext",
						"rollback context: this");
		}

		for (auto p = _statements.begin(); p != _statements.end(); p++)
			delete (*p)->statement();
	} catch (SQLException& e) {
		error("SQLRequestContext", e);
	}

	delete _conn.get();

	_statements.clear();
	_conn = NULL;
}

/**
 * Commit or roll back the transaction performed.
 * Release all the resources kept by this SQLRequestContext
 * @param commit Whether the transaction performed will be commited or
 * 					rolled back
 */
SQLRequestContext& SQLRequestContext::getCurrentContext() {
	SQLRequestContext* currentContext = _contextMap.at(
			::boost::this_thread::get_id());
	return *currentContext;
}

/**
 * Initialize the static resources shared by every instance of
 * SQLRequestContext
 * @param logger		The logger used to post event or error messages
 * @param pool			Reference to the pool of database connections
 * @param topicManager	Proxy to the IceStorm topic manager
 */
void SQLRequestContext::initialize(LoggerPtr logger,
		::Cardroid::occi::StatelessConnectionPoolHolder* pool,
		::IceStorm::TopicManagerPrx topicManager) {
	_logger = logger;
	_pool = pool;
	_topicManager = topicManager;
	_contextMap = unordered_map< ::boost::thread::id, SQLRequestContext*>();
}

::Cardroid::occi::StatementHolder SQLRequestContext::prepareStatement(
		const string& sql) {
	::Cardroid::occi::StatementHolder* stmt =
			new ::Cardroid::occi::StatementHolder(_conn, sql);
	_statements.push_front(stmt);
	return *stmt;
}

/**
 * Called only during the dispatch process.
 * Commits or rolls back the transaction performed and destroys this
 * SQLRequestContext, previously releasing all the resources it keeps
 * @param commit Whether the transaction performed will be commited or
 * 					rolled back
 */
void SQLRequestContext::destroy(bool commit) {
	destroyInternal(commit);
}

/**
 * Log an exception
 * @param prefix	The prefix to be printed before the exception
 * @param ex		The exception message
 */
void SQLRequestContext::error(const string& prefix, const exception& ex) {
	_logger->error(prefix + ": error:\n" + ex.what());
}

/**
 * Execute a SQL statement on the connection held by this class
 * @param sql92Style	SQL statatement, according to the SQL 92 standard
 * 							specification
 * @param params		Parameters to be binded as arguments in the
 * 							statement to be executed
 * @return				Whether the statement was successfully executed
 * @throws SQLException
 */
Statement::Status SQLRequestContext::executeStatement(const string& sql92Style,
		int nParams, PObject* p[]) {
	Statement::Status result = Statement::Status::UNPREPARED;

	try {
		::Cardroid::occi::StatementHolder pstmt =
				::Cardroid::occi::StatementHolder(_conn, sql92Style);
		result = Statement::Status::PREPARED;

		for (int n = 0; n < nParams; n++)
			pstmt.statement()->setObject(n + 1, p[n]);

		result = pstmt.statement()->execute(sql92Style);
	} catch (SQLException& e) {
		// Print stack trace
		::Cardroid::occi::printException(e);
	}

	return result;
}

/**
 * Execute a SQL statement on the connection held by this class
 * @param sql92Style	SQL statatement, according to the SQL 92 standard
 * 							specification
 * @param params		Map of the parameters to be binded as arguments in
 * 							the statement to be executed. For every Map
 * 							Entry, the key is the parameter itself, as the
 * 							value is its SQL type code
 * @return				Whether the statement was successfully executed
 * @throws SQLException
 */
Statement::Status SQLRequestContext::executeStatement(const string& sql92Style,
		map<PObject*, Type> params) {
	Statement::Status result = Statement::Status::UNPREPARED;

	try {
		::Cardroid::occi::StatementHolder pstmt =
				::Cardroid::occi::StatementHolder(_conn, sql92Style);
		result = Statement::Status::PREPARED;

		int pos = 1;
		for (auto paramsIter = params.begin(); paramsIter != params.end();
				paramsIter++)
			pstmt.statement()->setObject(pos++, paramsIter->first);

		result = pstmt.statement()->execute(sql92Style);

	} catch (SQLException& e) {
		// Print stack trace
		::Cardroid::occi::printException(e);
	}

	return result;
}

/**
 * Execute a SQL query on the connection held by this class
 * @param sql92Style	SQL query statatement, according to the SQL 92
 * 							standard specification
 * @param rowsFetched	The maximum number of rows to be fetched by this
 * 							query
 * @param params		Params to be binded as arguments in the statement
 * 							to be executed
 * @return				The result of executing the commanded query
 * @throws SQLException
 */
ResultSet* SQLRequestContext::executeStatementQuery(const string& sql92Style,
		int rowsFetched, int nParams, PObject* p[]) {
	Statement* statement = NULL;
	ResultSet* result = NULL;

	try {
		::Cardroid::occi::StatementHolder pstmt =
				::Cardroid::occi::StatementHolder(_conn, sql92Style);
		statement = pstmt.statement();
		statement->setPrefetchRowCount(rowsFetched);

		for (int n = 0; n < nParams; n++)
			statement->setObject(n + 1, p[n]);

		result = statement->executeQuery(sql92Style);
	} catch (SQLException& e) {
		// Print stack trace
		::Cardroid::occi::printException(e);
		// Release OCCI resources
		if (statement) {
			if (result)
				::Cardroid::occi::close(result, statement);
			else
				delete statement;
		}
	}

	return result;
}

/**
 * Execute a SQL query on the connection held by this class
 * @param sql92Style	SQL query statatement, according to the SQL 92
 * 							standard specification
 * @param rowsFetched	The maximum number of rows to be fetched by this
 * 							query
 * @param params		Map of the parameters to be binded as arguments in
 * 							the statement to be executed. For every Map
 * 							Entry, the key is the parameter itself, as the
 * 							value is its SQL type code
 * @return				The result of executing the commanded query
 * @throws SQLException
 */
ResultSet* SQLRequestContext::executeStatementQuery(const string& sql92Style,
		int rowsFetched, map<string, Type> params) {
	Statement* statement = NULL;
	ResultSet* result = NULL;

	try {
		::Cardroid::occi::StatementHolder pstmt =
				::Cardroid::occi::StatementHolder(_conn, sql92Style);
		statement = pstmt.statement();
		statement->setPrefetchRowCount(rowsFetched);

		int pos = 1;
		for (auto paramsIter = params.begin(); paramsIter != params.end();
				paramsIter++)
			statement->setString(pos++, paramsIter->first);

		result = statement->executeQuery(sql92Style);
	} catch (SQLException& e) {
		// Print stack trace
		::Cardroid::occi::printException(e);
		// Release OCCI resources
		if (statement) {
			if (result)
				::Cardroid::occi::close(result, statement);
			else
				delete statement;
		}
	}

	return result;
}

/**
 * Execute a SQL update statement on the connection held by this class
 * @param sql92Style	SQL statatement, according to the SQL 92 standard
 * 							specification
 * @param params		Parameters to be binded as arguments in the
 * 							statement to be executed
 * @return				The number of rows affected by this statement
 * 							execution
 * @throws SQLException
 */
int SQLRequestContext::executeStatementUpdate(const string& sql92Style,
		int nParams, PObject* p[]) {
	int result = 0;

	try {
		::Cardroid::occi::StatementHolder pstmt =
				::Cardroid::occi::StatementHolder(_conn, sql92Style);

		for (int n = 0; n < nParams; n++)
			pstmt.statement()->setObject(n + 1, p[n]);

		result = pstmt.statement()->executeUpdate(sql92Style);
	} catch (SQLException& e) {
		// Print stack trace
		::Cardroid::occi::printException(e);
	}

	return result;
}

/**
 * Execute a SQL update statement on the connection held by this class
 * @param sql92Style	SQL statatement, according to the SQL 92 standard
 * 							specification
 * @param params		Map of the parameters to be binded as arguments in
 * 							the statement to be executed. For every Map
 * 							Entry, the key is the parameter itself, as the
 * 							value is its SQL type code
 * @return				The number of rows affected by this statement
 * 							execution
 * @throws SQLException
 */
int SQLRequestContext::executeStatementUpdate(const string& sql92Style,
		map<PObject*, Type> params) {
	int result = 0;

	try {
		::Cardroid::occi::StatementHolder pstmt =
				::Cardroid::occi::StatementHolder(_conn, sql92Style);

		int pos = 1;
		for (auto paramsIter = params.begin(); paramsIter != params.end();
				paramsIter++)
			pstmt.statement()->setObject(pos++, paramsIter->first);

		result = pstmt.statement()->executeUpdate(sql92Style);
	} catch (SQLException& e) {
		// Print stack trace
		::Cardroid::occi::printException(e);
	}

	return result;
}

/**
 * Execute a stored procedure call on the connection held by this class
 * @param sql92Style	SQL stored procedure call statement, according to
 * 							the SQL 92 standard specification
 * @param params		Parameters to be binded as arguments in the
 * 							statement to be executed
 * @return				Whether the stored procedure commanded was
 * 							successfully called and executed
 * @throws SQLException
 */
Statement::Status SQLRequestContext::executeProcedure(const string& sql92Style,
		int nParams, PObject* p[]) {
	Statement::Status result = Statement::Status::UNPREPARED;

	try {
		::Cardroid::occi::StatementHolder cstmt
		 = ::Cardroid::occi::StatementHolder(_conn, sql92Style);
		result = Statement::Status::PREPARED;

		for (int n = 0; n < nParams; n++)
			cstmt.statement()->setObject(n + 1, p[n]);

		result = cstmt.statement()->execute(sql92Style);
	} catch (SQLException& e) {
		// Print stack trace
		::Cardroid::occi::printException(e);
	}

	return result;
}

/**
 * Execute a stored procedure call on the connection held by this class
 * @param sql92Style	SQL stored procedure call statement, according to
 * 							the SQL 92 standard specification
 * @param params		Map of the parameters to be binded as arguments in
 * 							the statement to be executed. For every Map
 * 							Entry, the key is the parameter itself, as the
 * 							value is its SQL type code
 * @return				Whether the stored procedure commanded was
 * 							successfully called and executed
 * @throws SQLException
 */
Statement::Status SQLRequestContext::executeProcedure(const string& sql92Style,
		map<PObject*, Type> params) {
	Statement::Status result = Statement::Status::UNPREPARED;

	try {
		::Cardroid::occi::StatementHolder cstmt =
				::Cardroid::occi::StatementHolder(_conn, sql92Style);
		result = Statement::Status::PREPARED;

		int pos = 1;
		for (auto paramsIter = params.begin(); paramsIter != params.end();
				paramsIter++)
			cstmt.statement()->setObject(pos++, paramsIter->first);
		result = cstmt.statement()->execute(sql92Style);
	} catch (SQLException& e) {
		// Print stack trace
		::Cardroid::occi::printException(e);
	}

	return result;
}

/**
 * Execute a stored function call on the connection held by this class
 * @param sql92Style		SQL stored function call statement, according
 * 								to the SQL 92 standard specification
 * @param resultSqlType		SQL type code of the function's result
 * @param resultSqlName		SQL type name of the function's result
 * @param resultTypeClass	The class which the function's result must be
 * 								casted to
 * @param params			Parameters to be binded as arguments in the
 * 								statement to be executed
 * @return					The result of executing the function
 * @throws SQLException
 */
PObject* SQLRequestContext::executeFunction(const string& sql92Style,
		Type resultSqlType, const string& resultTypeName, int nParams,
		PObject* p[]) {
	PObject* result = NULL;

	try {
		::Cardroid::occi::StatementHolder cstmt =
				::Cardroid::occi::StatementHolder(_conn, sql92Style);

		cstmt.statement()->registerOutParam(1, resultSqlType, sizeof(PObject*),
				resultTypeName);

		for (int n = 0; n < nParams; n++)
			cstmt.statement()->setObject(n + 2, p[n]);

		cstmt.statement()->execute(sql92Style);
		result = cstmt.statement()->getObject(1);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
	}

	return result;
}

/**
 * Execute a stored function call on the connection held by this class
 * @param sql92Style		SQL stored function call statement, according
 * 								to the SQL 92 standard specification
 * @param resultSqlType		SQL type code of the function's result
 * @param resultSqlName		SQL type name of the function's result
 * @param resultTypeClass	The class which the function's result must be
 * 								casted to
 * @param params			Map of the parameters to be binded as
 * 								arguments in the statement to be executed.
 * 								For every Map Entry, the key is the
 * 								parameter itself, as the value is its SQL
 * 								type code
 * @return					The result of executing the function
 * @throws SQLException
 */
PObject* SQLRequestContext::executeFunction(const string& sql92Style,
		Type resultSqlType, const string& resultTypeName,
		map<PObject*, Type> params) {
	PObject* result = NULL;

	try {
		::Cardroid::occi::StatementHolder cstmt =
				::Cardroid::occi::StatementHolder(_conn, sql92Style);

		auto paramsIter = params.begin();
		cstmt.statement()->registerOutParam(1, resultSqlType, sizeof(PObject*),
				resultTypeName);
		for (int pos = 2; paramsIter != params.end(); paramsIter++)
			cstmt.statement()->setObject(pos++, paramsIter->first);

		cstmt.statement()->execute(sql92Style);
		result = cstmt.statement()->getObject(1);
	} catch (SQLException& e) {
		// Print stack trace
		::Cardroid::occi::printException(e);
	}

	return result;
}

/**
 * Execute the "getUserPlaces" function
 * @return The query result
 */
void SQLRequestContext::getUserPlaces(
		const ::cardroid::data::zerocice::UserTypPrx& usr, Int n,
		::cardroid::ResultSeq& first, Int& nrows,
		::cardroid::QueryResultPrx& result, const Current& current) {
	ResultSet* rs = getUserPlaces(static_pointer_cast< ::Cardroid::Data::Oracle::User>(
			::Cardroid::Data::Oracle::User().newInstance(
					::Cardroid::Data::Ice::User::extractObject(usr))).get(),
			current);
	Statement* stmt = NULL;

	try {
		bool next = rs->next() == ResultSet::Status::DATA_AVAILABLE;
		stmt = rs->getStatement();

		first = vector<ObjectPrx>();
		for (int i = 0; next && i < n; ++i) {
			first.push_back(
					::Cardroid::Data::Ice::Place::createProxy(
							*static_cast< ::Cardroid::Data::Oracle::Place*>(
									rs->getObject(1)), current.adapter));
			next = rs->next() == ResultSet::Status::DATA_AVAILABLE;
		}
		if (next) {
			::Cardroid::Network::Server::QueryResultI* impl =
					new ::Cardroid::Network::Server::QueryResultI(*this, rs, current.adapter,
							::Cardroid::Network::Server::QueryType::GET_USER_PLACES);
			result = ::cardroid::QueryResultPrx::uncheckedCast(
					current.adapter->addWithUUID(impl));
			if (_queryResultListener)
				_queryResultListener->add(result, impl,
						QueryType::GET_USER_PLACES);
		}
	} catch (SQLException& e) {
		::Cardroid::occi::close(rs, stmt);
		throw ::cardroid::zerocice::OCCIException(e.getMessage().c_str(), e.getErrorCode());
	}
	// Release JDBC resources
	::Cardroid::occi::close(rs, stmt);
}

/**
 * Execute the "getUserPlaces" function
 * @return The query result
 */
ResultSet* SQLRequestContext::getUserPlaces(
		::Cardroid::Data::Oracle::User* usr, const Current& current) {
	Statement* cstmt = NULL;
	ResultSet* result = NULL;

	try {
		string sql92Style =
				"BEGIN :1 := callable_statements.get_user_places(:2); END;";
		// Create the CallabeStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setObject(2, usr);
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCICURSOR, sizeof(ResultSet*));
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = cstmt->getCursor(1);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		::Cardroid::occi::close(result, cstmt);
	}

	return result;
}

/**
 * Execute the "getUserPlaces" function and store its results in a List
 * @return The query result, stored in a List structure
 */
vector< ::Cardroid::Data::Oracle::Place*> SQLRequestContext::getUserPlacesList(
		::Cardroid::Data::Oracle::User* usr, const Current& current) {
	vector<PObject*> poList = resultSetToList(getUserPlaces(usr, current));
	vector< ::Cardroid::Data::Oracle::Place*> pList =
			vector< ::Cardroid::Data::Oracle::Place*>();
	for (vector<PObject*>::iterator i = poList.begin(); i != poList.end(); i++)
		pList.push_back(static_cast< ::Cardroid::Data::Oracle::Place*>(*i));
	return pList;
}

/**
 * Execute the "searchTrips" function
 * @return The query result
 */
void SQLRequestContext::searchTrips(
		const ::cardroid::data::zerocice::TripRequestTypPtr& tRequest, Int n,
		::cardroid::ResultSeq& first, Int& nrows,
		::cardroid::QueryResultPrx& result, const Current& current) {
	ResultSet* rs = searchTrips(static_pointer_cast< ::Cardroid::Data::Oracle::TripRequest>(
			::Cardroid::Data::Oracle::TripRequest().newInstance(shared_ptr<ITripRequest>(
					static_cast< ::Cardroid::Data::Ice::TripRequest*>(tRequest.get())))).get(),
			current);
	Statement* stmt = NULL;

	try {
		bool next = rs->next() == ResultSet::Status::DATA_AVAILABLE;
		stmt = rs->getStatement();

		first = vector<ObjectPrx>();
		for (int i = 0; next && i < n; ++i) {
			first.push_back(
					::Cardroid::Data::Ice::Trip::createProxy(
							*static_cast< ::Cardroid::Data::Oracle::Trip*>(
									rs->getObject(1)), current.adapter));
			next = rs->next() == ResultSet::Status::DATA_AVAILABLE;
		}
		if (next) {
			::Cardroid::Network::Server::QueryResultI* impl =
					new ::Cardroid::Network::Server::QueryResultI(*this, rs, current.adapter,
							::Cardroid::Network::Server::QueryType::SEARCH_TRIPS);
			result = ::cardroid::QueryResultPrx::uncheckedCast(
					current.adapter->addWithUUID(impl));
			if (_queryResultListener)
				_queryResultListener->add(result, impl,
						QueryType::SEARCH_TRIPS);
		}
	} catch (SQLException& e) {
		::Cardroid::occi::close(rs, stmt);
		throw ::cardroid::zerocice::OCCIException(e.getMessage().c_str(), e.getErrorCode());
	}
	// Release JDBC resources
	::Cardroid::occi::close(rs, stmt);
}

/**
 * Execute the "searchTrips" function
 * @return The query result
 */
ResultSet* SQLRequestContext::searchTrips(
		::Cardroid::Data::Oracle::TripRequest* tRequest,
		const Current& current) {
	Statement* cstmt = NULL;
	ResultSet* result = NULL;

	try {
		string sql92Style =
				"BEGIN :1 := callable_statements.search_trips(:2); END;";
		// Create the CallabeStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setObject(2, tRequest);
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCICURSOR, sizeof(ResultSet*));
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = cstmt->getCursor(1);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		::Cardroid::occi::close(result, cstmt);
	}

	return result;
}

/**
 * Execute the "searchTrips" function and store its results in a List
 * @return The query result, stored in a List structure
 */
vector< ::Cardroid::Data::Oracle::Trip*> SQLRequestContext::searchTripsList(
		::Cardroid::Data::Oracle::TripRequest* tRequest,
		const Current& current) {
	vector<PObject*> poList = resultSetToList(searchTrips(tRequest, current));
	vector< ::Cardroid::Data::Oracle::Trip*> tList =
			vector< ::Cardroid::Data::Oracle::Trip*>();
	for (vector<PObject*>::iterator i = poList.begin(); i != poList.end(); i++)
		tList.push_back(static_cast< ::Cardroid::Data::Oracle::Trip*>(*i));
	return tList;
}

/**
 * Execute the "getTripFromId" function
 * @return The function result
 */
::cardroid::data::zerocice::TripTypPrx SQLRequestContext::getTripFromId(
		Int tripId, const Current& current) {
	Statement* cstmt = NULL;
	::cardroid::data::zerocice::TripTypPrx result;

	try {
		string sql92Style =
				"BEGIN :1 := callable_statements.get_trip_from_id(:2); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setInt(2, tripId);
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCIPOBJECT, sizeof(::TripTyp*),
				Trip::getSqlTypeName());
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = ::Cardroid::Data::Ice::Trip::createProxy(
				*static_cast< ::Cardroid::Data::Oracle::Trip*>(
						cstmt->getObject(1)), current.adapter);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);

	return result;
}

/**
 * Execute the "getTripOfferFromId" function
 * @return The function result
 */
::cardroid::data::zerocice::TripOfferTypPrx SQLRequestContext::getTripOfferFromId(
		Int tripId, const Current& current) {
	Statement* cstmt = NULL;
	::cardroid::data::zerocice::TripOfferTypPrx result;

	try {
		string sql92Style =
				"BEGIN :1 := callable_statements.get_trip_offer_from_id(:2); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setInt(2, tripId);
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCIPOBJECT, sizeof(::TripOfferTyp*),
				TripOffer::getSqlTypeName());
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = ::Cardroid::Data::Ice::TripOffer::createProxy(
				*static_cast< ::Cardroid::Data::Oracle::TripOffer*>(
						cstmt->getObject(1)), current.adapter);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);

	return result;
}

/**
 * Execute the "getTripRequestFromId" function
 * @return The function result
 */
::cardroid::data::zerocice::TripRequestTypPrx SQLRequestContext::getTripRequestFromId(
		Int tripId, const Current& current) {
	Statement* cstmt = NULL;
	::cardroid::data::zerocice::TripRequestTypPrx result;

	try {
		string sql92Style =
				"BEGIN :1 := callable_statements.get_trip_request_from_id(:2); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setInt(2, tripId);
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCIPOBJECT, sizeof(::TripRequestTyp*),
				TripRequest::getSqlTypeName());
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = ::Cardroid::Data::Ice::TripRequest::createProxy(
				*static_cast< ::Cardroid::Data::Oracle::TripRequest*>(
						cstmt->getObject(1)), current.adapter);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);

	return result;
}

void SQLRequestContext::joinTrip(
		const ::cardroid::data::zerocice::TripOfferTypPrx& trip,
		const ::cardroid::data::zerocice::UserTypPrx& passenger, Int nSeats,
		const Current& current) {
	::cardroid::data::zerocice::UserActivityTypPrx activity =
			joinTrip(static_pointer_cast< ::Cardroid::Data::Oracle::TripOffer>(
					::Cardroid::Data::Oracle::TripOffer().newInstance(static_pointer_cast<ITripOffer>(
							::Cardroid::Data::Ice::TripOffer::extractObject(trip)))).get(),
					static_pointer_cast< ::Cardroid::Data::Oracle::User>(
					::Cardroid::Data::Oracle::User().newInstance(
							::Cardroid::Data::Ice::User::extractObject(passenger))).get(),
							 nSeats, current);
	notifyUserActivity(activity);
}

/**
 * Execute the "joinTrip" procedure
 * @return The procedure result, if any
 */
::cardroid::data::zerocice::UserActivityTypPrx SQLRequestContext::joinTrip(
		::Cardroid::Data::Oracle::TripOffer* trip,
		::Cardroid::Data::Oracle::User* passenger, int nSeats,
		const Current& current) {
	Statement* cstmt = NULL;
	::Cardroid::Data::Oracle::TripOffer* result;
	::cardroid::data::zerocice::UserActivityTypPrx activity;

	try {
		string sql92Style =
				"BEGIN callable_statements.join_trip(:1, :2, :3, :4); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setObject(1, trip);
		cstmt->setObject(2, passenger);
		cstmt->setInt(3, nSeats);
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCIPOBJECT, sizeof(::TripOfferTyp*),
				TripOffer::getSqlTypeName());
		cstmt->registerOutParam(4, Type::OCCIPOBJECT,
				sizeof(::UserActivityTyp*),
				UserActivity::getSqlTypeName());
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = static_cast< ::Cardroid::Data::Oracle::TripOffer*>(cstmt->getObject(1));
		*trip = static_cast< ::Cardroid::Data::Oracle::TripOffer&>(*result);
		activity =
				::Cardroid::Data::Ice::UserActivity::createProxy(
						*static_cast< ::Cardroid::Data::Oracle::UserActivity*>(
								cstmt->getObject(4)), current.adapter);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);

	return activity;
}

/**
 * Execute the "organize" procedure
 * @return The procedure result, if any
 */
::cardroid::data::zerocice::TripOfferTypPrx SQLRequestContext::organizeTrip(
		const ::cardroid::data::zerocice::TripRequestTypPrx& tripRequest,
		const ::cardroid::data::zerocice::TripOfferTypPtr& tripOffer,
		const Current& current) {
	::Cardroid::Data::Oracle::TripOffer* result = static_pointer_cast< ::Cardroid::Data::Oracle::TripOffer>(
			::Cardroid::Data::Oracle::TripOffer().newInstance(shared_ptr< ITripOffer>(
					static_cast< ::Cardroid::Data::Ice::TripOffer*>(tripOffer.get())))).get();
	::cardroid::data::zerocice::UserActivityTypPrx activity =
			organizeTrip(static_pointer_cast< ::Cardroid::Data::Oracle::TripRequest>(
					::Cardroid::Data::Oracle::TripRequest().newInstance(static_pointer_cast<ITripRequest>(
							::Cardroid::Data::Ice::TripRequest::extractObject(tripRequest)))).get(),
					result, current);
	notifyUserActivity(activity);
	return ::Cardroid::Data::Ice::TripOffer::createProxy(*result, current.adapter);
}

/**
 * Execute the "organize" procedure
 * @return The procedure result, if any
 */
::cardroid::data::zerocice::UserActivityTypPrx SQLRequestContext::organizeTrip(
		::Cardroid::Data::Oracle::TripRequest* tripRequest,
		::Cardroid::Data::Oracle::TripOffer* tripOffer,
		const Current& current) {
	Statement* cstmt = NULL;
	::Cardroid::Data::Oracle::TripOffer* result;
	::cardroid::data::zerocice::UserActivityTypPrx activity;

	try {
		string sql92Style =
				"BEGIN callable_statements.organize_trip(:1, :2, :3); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setObject(1, tripRequest);
		cstmt->setObject(2, tripOffer);
		// Register output parameters
		cstmt->registerOutParam(2, Type::OCCIPOBJECT, sizeof(::TripOfferTyp*),
				TripOffer::getSqlTypeName());
		cstmt->registerOutParam(3, Type::OCCIPOBJECT,
				sizeof(::UserActivityTyp*),
				UserActivity::getSqlTypeName());
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = static_cast< ::Cardroid::Data::Oracle::TripOffer*>(cstmt->getObject(2));
		*tripOffer = (static_cast< ::Cardroid::Data::Oracle::TripOffer&>(*result));
		activity =
				::Cardroid::Data::Ice::UserActivity::createProxy(
						*static_cast< ::Cardroid::Data::Oracle::UserActivity*>(
								cstmt->getObject(3)), current.adapter);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);

	return activity;
}

/**
 * Execute the "getUserTrips" function
 * @return The query result
 */
void SQLRequestContext::getUserTrips(
		const ::cardroid::data::zerocice::UserTypPrx& usr, Int n,
		::cardroid::ResultSeq& first, Int& nrows,
		::cardroid::QueryResultPrx& result, const Current& current) {
	ResultSet* rs = getUserTrips(static_pointer_cast< ::Cardroid::Data::Oracle::User>(
			::Cardroid::Data::Oracle::User().newInstance(
					::Cardroid::Data::Ice::User::extractObject(usr))).get(), current);
	Statement* stmt = NULL;

	try {
		bool next = rs->next() == ResultSet::Status::DATA_AVAILABLE;
		stmt = rs->getStatement();

		first = vector<ObjectPrx>();
		for (int i = 0; next && i < n; ++i) {
			first.push_back(
					::Cardroid::Data::Ice::Trip::createProxy(
							*static_cast< ::Cardroid::Data::Oracle::Trip*>(rs->getObject(
									1)), current.adapter));
			next = rs->next() == ResultSet::Status::DATA_AVAILABLE;
		}
		if (next) {
			::Cardroid::Network::Server::QueryResultI* impl =
					new ::Cardroid::Network::Server::QueryResultI(*this, rs, current.adapter,
							::Cardroid::Network::Server::QueryType::GET_USER_TRIPS);
			result = ::cardroid::QueryResultPrx::uncheckedCast(
					current.adapter->addWithUUID(impl));
			if (_queryResultListener)
				_queryResultListener->add(result, impl,
						QueryType::GET_USER_TRIPS);
		}
	} catch (SQLException& e) {
		::Cardroid::occi::close(rs, stmt);
		throw ::cardroid::zerocice::OCCIException(e.getMessage().c_str(), e.getErrorCode());
	}
	// Release JDBC resources
	::Cardroid::occi::close(rs, stmt);
}

/**
 * Execute the "getUserTrips" function
 * @return The query result
 */
ResultSet* SQLRequestContext::getUserTrips(
		::Cardroid::Data::Oracle::User* usr, const Current& current) {
	Statement* cstmt = NULL;
	ResultSet* result = NULL;

	try {
		string sql92Style =
				"BEGIN :1 := callable_statements.get_user_trips(:2); END;";
		// Create the CallabeStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setObject(2, usr);
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCICURSOR, sizeof(ResultSet*));
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = cstmt->getCursor(1);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		::Cardroid::occi::close(result, cstmt);
	}

	return result;
}

/**
 * Execute the "getUserTrips" function and store its results in a List
 * @return The query result, stored in a List structure
 */
vector< ::Cardroid::Data::Oracle::Trip*> SQLRequestContext::getUserTripsList(
		::Cardroid::Data::Oracle::User* usr, const Current& current) {
	vector<PObject*> poList = resultSetToList(getUserTrips(usr, current));
	vector< ::Cardroid::Data::Oracle::Trip*> tList =
			vector< ::Cardroid::Data::Oracle::Trip*>();
	for (vector<PObject*>::iterator i = poList.begin(); i != poList.end(); i++)
		tList.push_back(static_cast< ::Cardroid::Data::Oracle::Trip*>(*i));
	return tList;
}

/**
 * Execute the "getPassengerTrips" function
 * @return The query result
 */
void SQLRequestContext::getPassengerTrips(
		const ::cardroid::data::zerocice::UserTypPrx& usr, Int n,
		::cardroid::ResultSeq& first, Int& nrows,
		::cardroid::QueryResultPrx& result, const Current& current) {
	ResultSet* rs = getPassengerTrips(static_pointer_cast< ::Cardroid::Data::Oracle::User>(
			::Cardroid::Data::Oracle::User().newInstance(
					::Cardroid::Data::Ice::User::extractObject(usr))).get(), current);
	Statement* stmt = NULL;

	try {
		bool next = rs->next() == ResultSet::Status::DATA_AVAILABLE;
		stmt = rs->getStatement();

		first = vector<ObjectPrx>();
		for (int i = 0; next && i < n; ++i) {
			first.push_back(
					::Cardroid::Data::Ice::TripOffer::createProxy(
							*static_cast< ::Cardroid::Data::Oracle::TripOffer*>(
									rs->getObject(1)), current.adapter));
			next = rs->next() == ResultSet::Status::DATA_AVAILABLE;
		}
		if (next) {
			::Cardroid::Network::Server::QueryResultI* impl =
					new ::Cardroid::Network::Server::QueryResultI(*this, rs, current.adapter,
							::Cardroid::Network::Server::QueryType::GET_PASSENGER_TRIPS);
			result = ::cardroid::QueryResultPrx::uncheckedCast(
					current.adapter->addWithUUID(impl));
			if (_queryResultListener)
				_queryResultListener->add(result, impl,
						QueryType::GET_PASSENGER_TRIPS);
		}
	} catch (SQLException& e) {
		::Cardroid::occi::close(rs, stmt);
		throw ::cardroid::zerocice::OCCIException(e.getMessage().c_str(), e.getErrorCode());
	}
	// Release JDBC resources
	::Cardroid::occi::close(rs, stmt);
}

/**
 * Execute the "getPassengerTrips" function
 * @return The query result
 */
ResultSet* SQLRequestContext::getPassengerTrips(
		::Cardroid::Data::Oracle::User* usr, const Current& current) {
	Statement* cstmt = NULL;
	ResultSet* result = NULL;

	try {
		string sql92Style =
				"BEGIN :1 := callable_statements.get_passenger_trips(:2); END;";
		// Create the CallabeStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setObject(2, usr);
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCICURSOR, sizeof(ResultSet*));
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = cstmt->getCursor(1);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		::Cardroid::occi::close(result, cstmt);
	}

	return result;
}

/**
 * Execute the "getPassengerTrips" function and store its results in a List
 * @return The query result, stored in a List structure
 */
vector< ::Cardroid::Data::Oracle::TripOffer*> SQLRequestContext::getPassengerTripsList(
		::Cardroid::Data::Oracle::User* usr, const Current& current) {
	vector<PObject*> poList = resultSetToList(getPassengerTrips(usr, current));
	vector< ::Cardroid::Data::Oracle::TripOffer*> toList =
			vector< ::Cardroid::Data::Oracle::TripOffer*>();
	for (vector<PObject*>::iterator i = poList.begin(); i != poList.end(); i++)
		toList.push_back(static_cast< ::Cardroid::Data::Oracle::TripOffer*>(*i));
	return toList;
}

/**
 * Execute the "calculatePriceEstimation" function
 * @return The function result
 */
Double SQLRequestContext::calculatePriceEstimation(
		::cardroid::data::zerocice::Fuel fuel, Int distance,
		const Current& current) const {
	Statement* cstmt = NULL;
	double result = 0.;

	try {
		string sql92Style =
				"BEGIN :1 := callable_statements.calculate_price_estimation(:2, :3); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		string fuelName;
		switch (fuel) {
		case ::cardroid::data::zerocice::Fuel::UNLEADED95:
			fuelName = "UNLEADED95";
			break;
		case ::cardroid::data::zerocice::Fuel::UNLEADED98:
			fuelName = "UNLEADED98";
			break;
		case ::cardroid::data::zerocice::Fuel::DIESELA:
			fuelName = "DIESELA";
			break;
		case ::cardroid::data::zerocice::Fuel::DIESELAPLUS:
			fuelName = "DIESELAPLUS";
			break;
		case ::cardroid::data::zerocice::Fuel::BIODIESEL:
			fuelName = "BIODIESEL";
			break;
		}
		cstmt->setString(2, fuelName);
		cstmt->setInt(3, distance);
		// Register output parameters
		cstmt->registerOutParam(2, Type::OCCIDOUBLE, sizeof(double));
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = cstmt->getDouble(1);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);

	return result;
}

/**
 * Execute the "newTripOffer" procedure
 * @return The procedure result, if any
 */
::cardroid::data::zerocice::TripOfferTypPrx SQLRequestContext::newTripOffer(
		const ::cardroid::data::zerocice::TripOfferTypPtr& tOffer,
		const Current& current) {
	Statement* cstmt = NULL;
	::cardroid::data::zerocice::TripOfferTypPrx result;

	try {
		string sql92Style = "BEGIN callable_statements.new_trip_offer(:1); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		::Cardroid::Data::Oracle::TripOffer* toffer = static_pointer_cast< ::Cardroid::Data::Oracle::TripOffer>(
				::Cardroid::Data::Oracle::TripOffer().newInstance(
						shared_ptr<ITripOffer>(static_cast< ::Cardroid::Data::Ice::TripOffer*>(tOffer.get())))).get();
		cstmt->setObject(1, toffer);
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCIPOBJECT, sizeof(::TripOfferTyp*),
				TripOffer::getSqlTypeName());
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = ::Cardroid::Data::Ice::TripOffer::createProxy(
				*static_cast< ::Cardroid::Data::Oracle::TripOffer*>(cstmt->getObject(1)),
				current.adapter);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);

	return result;
}

/**
 * Execute the "newTripRequest" procedure
 * @return The procedure result, if any
 */
::cardroid::data::zerocice::TripRequestTypPrx SQLRequestContext::newTripRequest(
		const ::cardroid::data::zerocice::TripRequestTypPtr& tRequest,
		const Current& current) {
	Statement* cstmt = NULL;
	::cardroid::data::zerocice::TripRequestTypPrx result;

	try {
		string sql92Style =
				"BEGIN callable_statements.new_trip_request(:1); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		::Cardroid::Data::Oracle::TripRequest* trequest =
				static_pointer_cast< ::Cardroid::Data::Oracle::TripRequest>(
				::Cardroid::Data::Oracle::TripRequest().newInstance(shared_ptr<ITripRequest>(
						static_cast< ::Cardroid::Data::Ice::TripRequest*>(tRequest.get())))).get();
		cstmt->setObject(1, trequest);
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCIPOBJECT, sizeof(::TripRequestTyp*),
				TripOffer::getSqlTypeName());
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = ::Cardroid::Data::Ice::TripRequest::createProxy(
				*static_cast< ::Cardroid::Data::Oracle::TripRequest*>(cstmt->getObject(1)),
				current.adapter);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);

	return result;
}

/**
 * Execute the "getMessageTalksSpeakers" function
 * @return The query result
 */
void SQLRequestContext::getMessageTalksSpeakers(
		const ::cardroid::data::zerocice::UserTypPrx& usr, Int n,
		::cardroid::ResultSeq& first, Int& nrows,
		::cardroid::QueryResultPrx& result, const Current& current) {
	ResultSet* rs = getMessageTalksSpeakers(
			static_pointer_cast< ::Cardroid::Data::Oracle::User>(
					::Cardroid::Data::Oracle::User().newInstance(
					::Cardroid::Data::Ice::User::extractObject(usr))).get(),
			current);
	Statement* stmt = NULL;

	try {
		bool next = rs->next() == ResultSet::Status::DATA_AVAILABLE;
		stmt = rs->getStatement();

		first = vector<ObjectPrx>();
		for (int i = 0; next && i < n; ++i) {
			first.push_back(
					::Cardroid::Data::Ice::User::createProxy(
							*static_cast< ::Cardroid::Data::Oracle::User*>(rs->getObject(
									1)), current.adapter));
			next = rs->next() == ResultSet::Status::DATA_AVAILABLE;
		}
		if (next) {
			::Cardroid::Network::Server::QueryResultI* impl =
					new ::Cardroid::Network::Server::QueryResultI(*this, rs, current.adapter,
							::Cardroid::Network::Server::QueryType::GET_MESSAGE_TALKS_SPEAKERS);
			result = ::cardroid::QueryResultPrx::uncheckedCast(
					current.adapter->addWithUUID(impl));
			if (_queryResultListener)
				_queryResultListener->add(result, impl,
						QueryType::GET_MESSAGE_TALKS_SPEAKERS);
		}
	} catch (SQLException& e) {
		::Cardroid::occi::close(rs, stmt);
		throw ::cardroid::zerocice::OCCIException(e.getMessage().c_str(), e.getErrorCode());
	}
	// Release JDBC resources
	::Cardroid::occi::close(rs, stmt);
}

/**
 * Execute the "getMessageTalksSpeakers" function
 * @return The query result
 */
ResultSet* SQLRequestContext::getMessageTalksSpeakers(
		::Cardroid::Data::Oracle::User* usr, const Current& current) {
	Statement* cstmt = NULL;
	ResultSet* result = NULL;

	try {
		string sql92Style =
				"BEGIN :1 := callable_statements.get_message_talks_speakers(:2); END;";
		// Create the CallabeStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setObject(2, usr);
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCICURSOR, sizeof(ResultSet*));
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = cstmt->getCursor(1);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		::Cardroid::occi::close(result, cstmt);
	}

	return result;
}

/**
 * Execute the "getMessageTalksSpeakers" function and store its results in a List
 * @return The query result, stored in a List structure
 */
vector< ::Cardroid::Data::Oracle::User*> SQLRequestContext::getMessageTalksSpeakersList(
		::Cardroid::Data::Oracle::User* usr, const Current& current) {
	vector<PObject*> poList = resultSetToList(getMessageTalksSpeakers(usr, current));
	vector< ::Cardroid::Data::Oracle::User*> uList =
			vector< ::Cardroid::Data::Oracle::User*>();
	for (vector<PObject*>::iterator i = poList.begin(); i != poList.end(); i++)
		uList.push_back(static_cast< ::Cardroid::Data::Oracle::User*>(*i));
	return uList;
}

/**
 * Execute the "getMessageTalks" function
 * @return The query result
 */
void SQLRequestContext::getMessageTalks(
		const ::cardroid::data::zerocice::UserTypPrx& usr1,
		const ::cardroid::data::zerocice::UserTypPrx& usr2, Int n,
		::cardroid::ResultSeq& first, Int& nrows,
		::cardroid::QueryResultPrx& result, const Current& current) {
	ResultSet* rs = getMessageTalks(
			static_pointer_cast< ::Cardroid::Data::Oracle::User>(
					::Cardroid::Data::Oracle::User().newInstance(
					::Cardroid::Data::Ice::User::extractObject(usr1))).get(),
			static_pointer_cast< ::Cardroid::Data::Oracle::User>(
					::Cardroid::Data::Oracle::User().newInstance(
					::Cardroid::Data::Ice::User::extractObject(usr2))).get(),
			current);
	Statement* stmt = NULL;

	try {
		bool next = rs->next() == ResultSet::Status::DATA_AVAILABLE;
		stmt = rs->getStatement();

		first = vector<ObjectPrx>();
		for (int i = 0; next && i < n; ++i) {
			first.push_back(
					::Cardroid::Data::Ice::Message::createProxy(
							*static_cast< ::Cardroid::Data::Oracle::Message*>(rs->getObject(
									1)), current.adapter));
			next = rs->next() == ResultSet::Status::DATA_AVAILABLE;
		}
		if (next) {
			::Cardroid::Network::Server::QueryResultI* impl =
					new ::Cardroid::Network::Server::QueryResultI(*this, rs, current.adapter,
							::Cardroid::Network::Server::QueryType::GET_MESSAGE_TALKS);
			result = ::cardroid::QueryResultPrx::uncheckedCast(
					current.adapter->addWithUUID(impl));
			if (_queryResultListener)
				_queryResultListener->add(result, impl,
						QueryType::GET_MESSAGE_TALKS_SPEAKERS);
		}
	} catch (SQLException& e) {
		::Cardroid::occi::close(rs, stmt);
		throw ::cardroid::zerocice::OCCIException(e.getMessage().c_str(), e.getErrorCode());
	}
	// Release JDBC resources
	::Cardroid::occi::close(rs, stmt);
}

/**
 * Execute the "getMessageTalks" function
 * @return The query result
 */
ResultSet* SQLRequestContext::getMessageTalks(
		::Cardroid::Data::Oracle::User* usr1,
		::Cardroid::Data::Oracle::User* usr2, const Current&) {
	Statement* cstmt = NULL;
	ResultSet* result = NULL;

	try {
		string sql92Style =
				"BEGIN :1 := callable_statements.get_message_talks(:2, :3); END;";
		// Create the CallabeStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setObject(2, usr1);
		cstmt->setObject(3, usr2);
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCICURSOR, sizeof(ResultSet*));
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = cstmt->getCursor(1);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		::Cardroid::occi::close(result, cstmt);
	}

	return result;
}

/**
 * Execute the "getMessageTalksList" function and store its results in a List
 * @return The query result, stored in a List structure
 */
vector< ::Cardroid::Data::Oracle::Message*> SQLRequestContext::getMessageTalksList(
		::Cardroid::Data::Oracle::User* usr1,
		::Cardroid::Data::Oracle::User* usr2, const Current& current) {
	vector<PObject*> poList =
			resultSetToList(getMessageTalks(usr1, usr2, current));
	vector< ::Cardroid::Data::Oracle::Message*> mList =	vector< ::Cardroid::Data::Oracle::Message*>();
	for (vector<PObject*>::iterator i = poList.begin(); i != poList.end(); i++)
		mList.push_back(static_cast< ::Cardroid::Data::Oracle::Message*>(*i));
	return mList;
}

/**
 * Execute the "newMessage" procedure
 * @return The procedure result, if any
 */
::cardroid::data::zerocice::MessageTypPrx SQLRequestContext::newMessage(
		const ::cardroid::data::zerocice::UserTypPrx& usr1,
		const ::cardroid::data::zerocice::UserTypPrx& usr2,
		const string& message, const Current& current) {
	::Cardroid::Data::Oracle::Message* result = newMessage(
			static_pointer_cast< ::Cardroid::Data::Oracle::User>(
					::Cardroid::Data::Oracle::User().newInstance(
					::Cardroid::Data::Ice::User::extractObject(usr1))).get(),
			static_pointer_cast< ::Cardroid::Data::Oracle::User>(
					::Cardroid::Data::Oracle::User().newInstance(
					::Cardroid::Data::Ice::User::extractObject(usr2))).get(),
			message, current);
	::cardroid::data::zerocice::MessageTypPrx resultPrx =
			::Cardroid::Data::Ice::Message::createProxy(*result,
					current.adapter);
	sendMessage(resultPrx);
	return resultPrx;
}

/**
 * Execute the "newMessage" procedure
 * @return The procedure result, if any
 */
::Cardroid::Data::Oracle::Message* SQLRequestContext::newMessage(
		::Cardroid::Data::Oracle::User* usr1,
		::Cardroid::Data::Oracle::User* usr2, const string& message,
		const Current& current) {
	Statement* cstmt = NULL;
	::Cardroid::Data::Oracle::Message* result = NULL;

	try {
		string sql92Style =
				"BEGIN callable_statements.new_message(:1, :2, :3, :4); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setObject(1, usr1);
		cstmt->setObject(2, usr2);
		cstmt->setString(3, message);
		// Register output parameters
		cstmt->registerOutParam(4, Type::OCCIPOBJECT, sizeof(::MessageTyp*),
				Message::getSqlTypeName());
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = static_cast< ::Cardroid::Data::Oracle::Message*>(cstmt->getObject(4));
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);

	return result;
}

/**
 * Execute the "getUserActivity" function
 * @return The query result
 */
void SQLRequestContext::getUserActivity(
		const ::cardroid::data::zerocice::UserTypPrx& usr, Int n,
		::cardroid::ResultSeq& first, Int& nrows,
		::cardroid::QueryResultPrx& result, const Current& current) {
	ResultSet* rs = getUserActivity(static_pointer_cast< ::Cardroid::Data::Oracle::User>(
			::Cardroid::Data::Oracle::User().newInstance(
					::Cardroid::Data::Ice::User::extractObject(usr))).get(), current);
	Statement* stmt = NULL;

	try {
		bool next = rs->next() == ResultSet::Status::DATA_AVAILABLE;
		stmt = rs->getStatement();

		first = vector<ObjectPrx>();
		for (int i = 0; next && i < n; ++i) {
			first.push_back(
					::Cardroid::Data::Ice::UserActivity::createProxy(
							*static_cast< ::Cardroid::Data::Oracle::UserActivity*>(
									rs->getObject(1)), current.adapter));
			next = rs->next() == ResultSet::Status::DATA_AVAILABLE;
		}
		if (next) {
			::Cardroid::Network::Server::QueryResultI* impl =
					new ::Cardroid::Network::Server::QueryResultI(*this, rs, current.adapter,
							::Cardroid::Network::Server::QueryType::GET_USER_ACTIVITY);
			result = ::cardroid::QueryResultPrx::uncheckedCast(
					current.adapter->addWithUUID(impl));
			if (_queryResultListener)
				_queryResultListener->add(result, impl,
						QueryType::GET_USER_ACTIVITY);
		}
	} catch (SQLException& e) {
		::Cardroid::occi::close(rs, stmt);
		throw ::cardroid::zerocice::OCCIException(e.getMessage().c_str(), e.getErrorCode());
	}
	// Release JDBC resources
	::Cardroid::occi::close(rs, stmt);
}

/**
 * Execute the "getUserActivity" function
 * @return The query result
 */
ResultSet* SQLRequestContext::getUserActivity(
		::Cardroid::Data::Oracle::User* usr, const Current& current) {
	Statement* cstmt = NULL;
	ResultSet* result = NULL;

	try {
		string sql92Style =
				"BEGIN :1 := callable_statements.get_user_activity(:2); END;";
		// Create the CallabeStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setObject(2, usr);
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCICURSOR, sizeof(ResultSet*));
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = cstmt->getCursor(1);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		::Cardroid::occi::close(result, cstmt);
	}

	return result;
}

/**
 * Execute the "getUserActivityList" function and store its results in a List
 * @return The query result, stored in a List structure
 */
vector< ::Cardroid::Data::Oracle::UserActivity*> SQLRequestContext::getUserActivityList(
		::Cardroid::Data::Oracle::User* usr, const Current& current) {
	vector<PObject*> poList = resultSetToList(getUserActivity(usr, current));
	vector< ::Cardroid::Data::Oracle::UserActivity*> uaList =
			vector< ::Cardroid::Data::Oracle::UserActivity*>();
	for (vector<PObject*>::iterator i = poList.begin(); i != poList.end(); i++)
		uaList.push_back(static_cast< ::Cardroid::Data::Oracle::UserActivity*>(*i));
	return uaList;
}

/**
 * Execute the "getUserFromEmail" function
 * @return The function result
 */
::cardroid::data::zerocice::UserTypPrx SQLRequestContext::getUserFromEmail(
		const string& email, const Current& current) {
	Statement* cstmt = NULL;
	::cardroid::data::zerocice::UserTypPrx result;

	try {
		string sql92Style =
				"BEGIN :1 := callable_statements.get_user_from_email(:2); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setString(2, email);
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCIPOBJECT, sizeof(::UserTyp*),
				User::getSqlTypeName());
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = ::Cardroid::Data::Ice::User::createProxy(
				*static_cast< ::Cardroid::Data::Oracle::User*>(cstmt->getObject(1)),
				current.adapter);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);

	return result;
}

/**
 * Execute the "getCarFromPlate" function
 * @return The function result
 */
::cardroid::data::zerocice::CarTypPrx SQLRequestContext::getCarFromPlate(
		const string& plate,
		const ::cardroid::data::zerocice::UserTypPrx& owner,
		const Current& current) {
	Statement* cstmt = NULL;
	::cardroid::data::zerocice::CarTypPrx result;

	try {
		string sql92Style =
				"BEGIN :1 := callable_statements.get_car_from_plate(:2, :3); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setString(2, plate);
		cstmt->setObject(3, static_pointer_cast< ::Cardroid::Data::Oracle::User>(
				::Cardroid::Data::Oracle::User().newInstance(
						::Cardroid::Data::Ice::User::extractObject(owner))).get());
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCIPOBJECT, sizeof(::CarTyp*),
				Car::getSqlTypeName());
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = ::Cardroid::Data::Ice::Car::createProxy(owner->getEmail(),
				*static_cast< ::Cardroid::Data::Oracle::Car*>(cstmt->getObject(1)),
				current.adapter);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);

	return result;
}

/**
 * Execute the "getCarFromPlate" function
 * @return The function result
 */
::Cardroid::Data::Oracle::Car* SQLRequestContext::getCarFromPlate(
		const string& plate, ::Cardroid::Data::Oracle::User* owner,
		const Current& current) {
	Statement* cstmt = NULL;
	::Cardroid::Data::Oracle::Car* result = NULL;

	try {
		string sql92Style =
				"BEGIN :1 := callable_statements.get_car_from_plate(:2, :3); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setString(2, plate);
		cstmt->setObject(3, owner);
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCIPOBJECT, sizeof(::CarTyp*),
				Car::getSqlTypeName());
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = static_cast< ::Cardroid::Data::Oracle::Car*>(cstmt->getObject(1));
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);

	return result;
}

/**
 * Execute the "getCarFromPlateEmail" function
 * @return The function result
 */
::cardroid::data::zerocice::CarTypPrx SQLRequestContext::getCarFromPlateEmail(
		const string& plate, const string& ownerEmail, const Current& current) {
	return ::Cardroid::Data::Ice::Car::createProxy(ownerEmail,
			*getCarFromPlate(plate, ownerEmail, current), current.adapter);
}

/**
 * Execute the "getCarFromPlate" function
 * @return The function result
 */
::Cardroid::Data::Oracle::Car* SQLRequestContext::getCarFromPlate(
		const string& plate, const string& ownerEmail, const Current& current) {
	Statement* cstmt = NULL;
	::Cardroid::Data::Oracle::Car* result = NULL;

	try {
		string sql92Style =
				"BEGIN :1 := callable_statements.get_car_from_plate(:2, :3); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setString(2, plate);
		cstmt->setString(3, ownerEmail);
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCIPOBJECT, sizeof(::CarTyp*),
				Car::getSqlTypeName());
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = static_cast< ::Cardroid::Data::Oracle::Car*>(cstmt->getObject(1));
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);

	return result;
}

/**
 * Execute the "updateUserData" procedure
 * @return The procedure result, if any
 */
::cardroid::data::zerocice::UserTypPrx SQLRequestContext::updateUserData(
		const ::cardroid::data::zerocice::UserTypPtr& usr,
		const Current& current) {
	string sql92Style;

	Statement* cstmt = NULL;
	::cardroid::data::zerocice::UserTypPrx result;

	if (usr->hasAvatar()) {
		ResultSet* rset = NULL;

		vector<unsigned char> avatarBytes = usr->getAvatarBytes();
		usr->userAvatarBytes->clear();

		try {
			sql92Style =
					"SELECT avatar FROM users_tbl WHERE email = :1 FOR UPDATE";
			map<string, Type> paramsMap = {
					{ usr->getEmail(), Type::OCCISTRING } };
			rset = executeStatementQuery(sql92Style, 1, paramsMap);
			if (rset->next() != ResultSet::Status::END_OF_FETCH) {
				Blob blob = rset->getBlob(1);
				blob.open(LobOpenMode::OCCI_LOB_READWRITE);
				blob.writeChunk(avatarBytes.size(), avatarBytes.data(),
						avatarBytes.size());
				blob.trim(avatarBytes.size());
				blob.close();
			}
		} catch (SQLException& sqle) {
			::Cardroid::occi::printException(sqle);
			::Cardroid::occi::close(rset);
		} catch (ios_base::failure& ioe) {
			::Cardroid::occi::printException(ioe);
		}
		::Cardroid::occi::close(rset);
	}

	try {
		sql92Style = "BEGIN callable_statements.update_user_data(:1); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setObject(1, static_pointer_cast< ::Cardroid::Data::Oracle::User>(
				::Cardroid::Data::Oracle::User().newInstance(shared_ptr<IUser>(
						static_cast< ::Cardroid::Data::Ice::User*>(usr.get())))).get());
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCIPOBJECT,
				sizeof(::Cardroid::Data::Oracle::User*),
				User::getSqlTypeName());
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = ::Cardroid::Data::Ice::User::createProxy(
				*static_cast< ::Cardroid::Data::Oracle::User*>(cstmt->getObject(1)),
				current.adapter);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);

	return result;
}

/**
 * Execute the "updateCarData" procedure
 * @return The procedure result, if any
 */
::cardroid::data::zerocice::CarTypPrx SQLRequestContext::updateCarData(
		const ::cardroid::data::zerocice::CarTypPtr& car,
		const ::cardroid::data::zerocice::UserTypPtr& usr,
		const Current& current) {
	Statement* cstmt = NULL;
	::cardroid::data::zerocice::CarTypPrx result;

	try {
		string sql92Style =
				"BEGIN callable_statements.update_car_data(:1, :2); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setObject(1,	static_pointer_cast< ::Cardroid::Data::Oracle::Car>(
				::Cardroid::Data::Oracle::Car().newInstance(shared_ptr<ICar>(
						static_cast< ::Cardroid::Data::Ice::Car*>(car.get())))).get());
		cstmt->setObject(2, static_pointer_cast< ::Cardroid::Data::Oracle::User>(
				::Cardroid::Data::Oracle::User().newInstance(
						shared_ptr< ::Cardroid::Data::Ice::User>(
								static_cast< ::Cardroid::Data::Ice::User*>(usr.get())))).get());
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCIPOBJECT, sizeof(::CarTyp*),
				Car::getSqlTypeName());
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = ::Cardroid::Data::Ice::Car::createProxy(usr->getEmail(),
				*static_cast< ::Cardroid::Data::Oracle::Car*>(cstmt->getObject(1)),
				current.adapter);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);

	return result;
}

/**
 * Execute the "updateCarDataEmail" procedure
 * @return The procedure result, if any
 */
::cardroid::data::zerocice::CarTypPrx SQLRequestContext::updateCarDataEmail(
		const ::cardroid::data::zerocice::CarTypPtr& car,
		const string& usrEmail, const Current& current) {
	Statement* cstmt = NULL;
	::cardroid::data::zerocice::CarTypPrx result;

	try {
		string sql92Style =
				"BEGIN callable_statements.update_car_data(:1, :2); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setObject(1, static_pointer_cast< ::Cardroid::Data::Oracle::Car>(
				::Cardroid::Data::Oracle::Car().newInstance(
						shared_ptr< ::Cardroid::Data::Ice::Car>(
								static_cast< ::Cardroid::Data::Ice::Car*>(
										car.get())))).get());
		cstmt->setString(2, usrEmail);
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCIPOBJECT, sizeof(::CarTyp*),
				Car::getSqlTypeName());
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = ::Cardroid::Data::Ice::Car::createProxy(usrEmail,
				*static_cast< ::Cardroid::Data::Oracle::Car*>(
						cstmt->getObject(1)), current.adapter);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);

	return result;
}

::cardroid::data::zerocice::CarTypPrx SQLRequestContext::addCar(
		const ::cardroid::data::zerocice::CarTypPtr& car,
		const ::cardroid::data::zerocice::UserTypPrx& usr,
		const Current& current) {
	Statement* cstmt = NULL;
	::cardroid::data::zerocice::CarTypPrx result;

	try {
		string sql92Style = "BEGIN callable_statements.add_car(:1, :2); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setObject(1,	static_pointer_cast< ::Cardroid::Data::Oracle::Car>(
				::Cardroid::Data::Oracle::Car().newInstance(shared_ptr<ICar>(
						static_cast< ::Cardroid::Data::Ice::Car*>(car.get())))).get());
		cstmt->setObject(2, static_pointer_cast< ::Cardroid::Data::Oracle::User>(
				::Cardroid::Data::Oracle::User().newInstance(
						::Cardroid::Data::Ice::User::extractObject(usr))).get());
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = ::Cardroid::Data::Ice::Car::createProxy(usr->getEmail(),
				*(car.get()), current.adapter);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);

	return result;
}

/**
 * Execute the "addCar" procedure
 * @return The procedure result, if any
 */
::cardroid::data::zerocice::CarTypPrx SQLRequestContext::addCarEmail(
		const ::cardroid::data::zerocice::CarTypPtr& car,
		const string& usrEmail, const Current& current) {
	Statement* cstmt = NULL;
	::cardroid::data::zerocice::CarTypPrx result;

	try {
		string sql92Style = "BEGIN callable_statements.add_car(:1, :2); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setObject(1, static_pointer_cast< ::Cardroid::Data::Oracle::Car>(
				::Cardroid::Data::Oracle::Car().newInstance(shared_ptr<ICar>(
						static_cast< ::Cardroid::Data::Ice::Car*>(
								car.get())))).get());
		cstmt->setString(2, usrEmail);
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = ::Cardroid::Data::Ice::Car::createProxy(usrEmail, *(car.get()),
				current.adapter);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);

	return result;
}

/**
 * Execute the "removeCar" procedure
 * @return The procedure result, if any
 */
void SQLRequestContext::removeCar(
		const ::cardroid::data::zerocice::CarTypPrx& car,
		const ::cardroid::data::zerocice::UserTypPrx& usr,
		const Current& current) {
	removeCar(
			static_pointer_cast< ::Cardroid::Data::Oracle::Car>(
					::Cardroid::Data::Oracle::Car().newInstance(
					::Cardroid::Data::Ice::Car::extractObject(car))).get(),
			static_pointer_cast< ::Cardroid::Data::Oracle::User>(
					::Cardroid::Data::Oracle::User().newInstance(
					::Cardroid::Data::Ice::User::extractObject(usr))).get(),
			current);
}

/**
 * Execute the "removeCar" procedure
 * @return The procedure result, if any
 */
void SQLRequestContext::removeCar(::Cardroid::Data::Oracle::Car* car,
		::Cardroid::Data::Oracle::User* usr, const Current& current) {
	Statement* cstmt = NULL;

	try {
		string sql92Style = "BEGIN callable_statements.remove_car(:1, :2); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setObject(1, car);
		cstmt->setObject(2, usr);
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);
}

/**
 * Execute the "removeCarPlateEmail" procedure
 * @return The procedure result, if any
 */
void SQLRequestContext::removeCarPlateEmail(const string& plate,
		const string& ownerEmail, const Current& current) {
	Statement* cstmt = NULL;

	try {
		string sql92Style = "BEGIN callable_statements.remove_car(:1, :2); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setString(1, plate);
		cstmt->setString(2, ownerEmail);
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);
}

/**
 * Execute the "userTripRegistered" function
 * @return The function result
 */
bool SQLRequestContext::userTripRegistered(
		const ::cardroid::data::zerocice::UserTypPrx& usr,
		const ::cardroid::data::zerocice::TripTypPrx& trip,
		const Current& current) {
	return userTripRegistered(
			static_pointer_cast< ::Cardroid::Data::Oracle::User>(
					::Cardroid::Data::Oracle::User().newInstance(
					::Cardroid::Data::Ice::User::extractObject(usr))).get(),
			static_pointer_cast< ::Cardroid::Data::Oracle::Trip>(
					::Cardroid::Data::Oracle::Trip().newInstance(
					::Cardroid::Data::Ice::Trip::extractObject(trip))).get(),
			current);
}

/**
 * Execute the "userTripRegistered" function
 * @return The function result
 */
bool SQLRequestContext::userTripRegistered(
		::Cardroid::Data::Oracle::User* usr,
		::Cardroid::Data::Oracle::Trip* trip, const Current& current) {
	Statement* cstmt = NULL;
	bool result;

	try {
		string sql92Style =
				"BEGIN :1 := callable_statements.user_trip_registered(:2, :3); END;";
		// Create the CallableStatement object
		cstmt = _conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setObject(2, usr);
		cstmt->setObject(3, trip);
		// Register output parameters
		cstmt->registerOutParam(1, Type::OCCIINT, sizeof(int));
		// Execute CallableStatement and retrieve results
		cstmt->execute(sql92Style);
		result = cstmt->getInt(1) > 0;
	} catch (SQLException& e) {
		::Cardroid::occi::printException(e);
		result = true;
		cstmt->getConnection()->terminateStatement(cstmt);
	}
	cstmt->getConnection()->terminateStatement(cstmt);

	return result;
}

vector<PObject*> SQLRequestContext::resultSetToList(ResultSet* rset) {
	vector<PObject*> rlist = vector<PObject*>();

	if (rset) {
		try {
			while (rset->next() == ResultSet::Status::DATA_AVAILABLE)
				rlist.push_back(rset->getObject(0));

			if (rset->status() == ResultSet::Status::END_OF_FETCH) {
				Statement* stmt = rset->getStatement();
				stmt->closeResultSet(rset);
			}
		} catch (SQLException& e) {
			::Cardroid::occi::printException(e);
		}
		rlist.shrink_to_fit();
	}
	return rlist;
}

/**
 * Publish an user activity in the receipt user's topic
 * @param activity The object to be published
 */
void SQLRequestContext::notifyUserActivity(
		::cardroid::data::zerocice::UserActivityTypPrx activity) {
	// Retrieve the topic's publisher for the receipt user
	::cardroid::network::zerocice::CardroidEventStormPrx publisher =
			retrieveTopicPublisher(activity->getActivityUser()->getEmail());

	/* Publish the new user activity notification to its subscribers *
	 * via the IceStorm service										 */
	if (publisher)
		publisher->notify(activity);
}

/**
 * Publish a private message in the receipt's topic
 * @param msg The object to be published
 */
void SQLRequestContext::sendMessage(
		::cardroid::data::zerocice::MessageTypPrx msg) {
	// Retrieve the topic's publisher for the receipt user
	::cardroid::network::zerocice::CardroidEventStormPrx publisher =
			retrieveTopicPublisher(msg->getUser2()->getEmail());

	/* Publish the new message notification to its subscribers *
	 * via the IceStorm service								   */
	if (publisher)
		publisher->message(msg);

	// retrieveTopicPublisher(msg->getUser1()->getEmail())->message(msg);
}

/**
 * Retrieve the publisher object for the IceStorm topic identified by name
 * @param name	The id of the topic
 * @return		A proxy to the publisher object for this topic,
 * 					null if no topic with that name exists
 */
::cardroid::network::zerocice::CardroidEventStormPrx SQLRequestContext::retrieveTopicPublisher(
		const string& name) {
	IceStorm::TopicPrx topic;
	try {
		topic = _topicManager->retrieve(name);
	} catch (IceStorm::NoSuchTopic& nst) {
		// No sessions for the topic's user are currently active
		return NULL;
	}

	/* The _fwd context key, when encountered by Glacier2, *
	 * causes the router to forward the request			   *
	 * using compressed batch oneway messages			   */
	map<string, string> ctx { { "_fwd", "Oz" } };

	/* Obtain a proxy for the pubisher object and narrow it to the *
	 * CardroidEventStorm interface								*/
	return ::cardroid::network::zerocice::CardroidEventStormPrx::uncheckedCast(
			topic->getPublisher()->ice_oneway()->ice_context(ctx));
}
