#include "Util.h"

#include <Ice/LocalException.h>
#include <occiControl.h>
#include <occiObjects.h>
#include <oci.h>
#include <ociap.h>
#include <oratypes.h>
#include <orl.h>
#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "Cardroid.h"

using namespace std;
using namespace oracle::occi;

namespace Cardroid {
namespace occi {

EnvironmentHolder::EnvironmentHolder(bool threaded, bool object) :
		_env(
				Environment::createEnvironment(
						Environment::Mode(
								(threaded ?
										Environment::THREADED_MUTEXED :
										Environment::THREADED_UNMUTEXED)
										| (object ? Environment::OBJECT : 0)))) {
}

EnvironmentHolder::~EnvironmentHolder() {
	try {
		Environment::terminateEnvironment(_env);
	} catch (const std::exception&) {
	}
}

StatelessConnectionPoolHolder::StatelessConnectionPoolHolder(Environment* env,
		const string& userName, const string& password,
		const string& connectString, int maxConn, int minConn,
		int incrConn) :
		_env(env), _pool(
				env->createStatelessConnectionPool(userName, password,
						connectString, maxConn, minConn, incrConn,
						StatelessConnectionPool::HOMOGENEOUS)) {
}

StatelessConnectionPoolHolder::StatelessConnectionPoolHolder(
		EnvironmentHolder* env, const string& userName, const string& password,
		const string& connectString, int maxConn, int minConn,
		int incrConn) :
		_env(env->environment()), _pool(
				env->environment()->createStatelessConnectionPool(userName,
						password, connectString, maxConn, minConn, incrConn,
						StatelessConnectionPool::HOMOGENEOUS)) {
}

StatelessConnectionPoolHolder::~StatelessConnectionPoolHolder() {
	try {
		_env->terminateStatelessConnectionPool(_pool);
	} catch (const exception&) {
	}
}

ConnectionHolder::ConnectionHolder(StatelessConnectionPool* pool) :
		_con(pool->getAnyTaggedConnection()), _txDone(false), _env(0), _pool(pool) {
}

ConnectionHolder::ConnectionHolder(Environment* env, const string& userName,
		const string& password, const string& connectString) :
		_con(env->createConnection(userName, password, connectString)), _txDone(
				false), _env(env), _pool(0) {
}

ConnectionHolder::ConnectionHolder(StatelessConnectionPoolHolder* pool) :
		_con(pool->pool()->getAnyTaggedConnection()), _txDone(false),  _env(0), _pool(
				pool->pool()) {
}

ConnectionHolder::ConnectionHolder(EnvironmentHolder* env,
		const string& userName, const string& password,
		const string& connectString) :
		_con(
				env->environment()->createConnection(userName, password,
						connectString)), _txDone(false), _env(
				env->environment()), _pool(0) {
}

void ConnectionHolder::commit() {
	_txDone = true;
	try {
		_con->commit();
	} catch (const SQLException& e) {
		throw cardroid::zerocice::OCCIException(e.getMessage().c_str(), e.getErrorCode());
	}
}

void ConnectionHolder::rollback() {
	_txDone = true;
	try {
		_con->rollback();
	} catch (const SQLException& e) {
		throw cardroid::zerocice::OCCIException(e.getMessage().c_str(), e.getErrorCode());
	}
}

ConnectionHolder::~ConnectionHolder() {
	if (!_txDone) {
		_txDone = true;
		try {
			_con->rollback();
		} catch (const std::exception&) {
		}
	}

	try {
		if (_pool) {
			_pool->releaseConnection(_con);
		}
		if (_env) {
			_env->terminateConnection(_con);
		}
	} catch (const std::exception&) {
	}
}

StatementHolder::StatementHolder(Connection* con, const string& sql) :
		_con(con), _stmt(con->createStatement(sql)) {
}

StatementHolder::StatementHolder(const ConnectionHolderPtr& conh,
		const string& sql) :
		_con(conh->connection()), _stmt(
				conh->connection()->createStatement(sql)) {
}

StatementHolder::~StatementHolder() {
	_con->terminateStatement(_stmt);
}

string encodeRef(const RefAny& ref, Environment* env) {
	string result;

	ub4 length = OCIRefHexSize(env->getOCIEnvironment(), ref.getRef());
	OraText* buffer = new OraText[length];

	OCIError* error = 0;
	OCIHandleAlloc(env->getOCIEnvironment(), reinterpret_cast<void**>(&error),
	OCI_HTYPE_ERROR, 0, 0);
	sword status = OCIRefToHex(env->getOCIEnvironment(), error, ref.getRef(),
			buffer, &length);

	if (status == OCI_SUCCESS) {
		result.assign(reinterpret_cast<char*>(buffer), length);
	} else {
		cerr << "encodeRef failed: ";
		sb4 errcode = 0;
		OraText buf[512];
		OCIErrorGet(error, 1, 0, &errcode, buf, 512, OCI_HTYPE_ERROR);
		cerr << reinterpret_cast<char*>(buf) << endl;
	}

	OCIHandleFree(error, OCI_HTYPE_ERROR);
	delete[] buffer;
	return result;
}

RefAny decodeRef(const string& str, Environment* env, Connection* con) {
	OCIRef* ref = 0;
	OCIError* error = 0;
	OCIHandleAlloc(env->getOCIEnvironment(), reinterpret_cast<void**>(&error),
	OCI_HTYPE_ERROR, 0, 0);

	sword status = OCIRefFromHex(env->getOCIEnvironment(), error,
			con->getOCIServiceContext(),
			reinterpret_cast<const OraText*>(str.c_str()), str.length(), &ref);

	if (status == OCCI_SUCCESS) {
		OCIHandleFree(error, OCI_HTYPE_ERROR);
		return RefAny(con, ref);
	} else {
		cerr << "decodeRef failed: ";
		sb4 errcode = 0;
		OraText buf[512];
		OCIErrorGet(error, 1, 0, &errcode, buf, 512, OCI_HTYPE_ERROR);
		cerr << reinterpret_cast<char*>(buf) << endl;

		OCIHandleFree(error, OCI_HTYPE_ERROR);
		throw Ice::ObjectNotExistException(__FILE__, __LINE__);
	}
}

/**
 * Close a ResultSet , the Statement that originated it, the database
 * Connection over which the Statement was executed, and the Environment
 * it was set up
 */
void close(ResultSet* resultSet, Statement* statement, Connection* connection,
		Environment* environment) {
	try {
		statement->closeResultSet(resultSet);
		connection->terminateStatement(statement);
		environment->terminateConnection(connection);
		Environment::terminateEnvironment(environment);
	} catch (const SQLException&) {
	}
}

/**
 * Close a ResultSet and the Statement that originated it
 */
void close(ResultSet* resultSet, Statement* statement) {
	try {
		statement->closeResultSet(resultSet);
		Connection* conn = statement->getConnection();
		conn->terminateStatement(statement);
	} catch (SQLException&) {
	}
}

/**
 * Close a ResultSet
 */
void close(ResultSet* resultSet) {
	try {
		Statement* stmt = resultSet->getStatement();
		stmt->closeResultSet(resultSet);
	} catch (SQLException&) {
	}
}

/**
 * Close a StatelessConnectionPool and the Environment it was set up
 */
void close(StatelessConnectionPool* pool, Environment* environment) {
	environment->terminateStatelessConnectionPool(pool);
	Environment::terminateEnvironment(environment);
}

/**
 * Print an exception, providing convenient format
 */
void printException(const exception& e) {
	cout << "Exception caught! Exiting..." << endl;
	cout << "error message: " << e.what() << endl;
}

void printExceptionAndRollback(const ConnectionHolderPtr& conn,
		const exception& e) {
	printException(e);
	conn->rollback();
}

void printExceptionAndRollback(Connection* conn, const exception& e) {
	printException(e);
	try {
		if (conn) {
			conn->rollback();
		}
	} catch (SQLException&) {
	}
}

/**
 * Starts SQL trace for a JDBC program. Also sets the timed statistics to
 * true. The SQL trace is automatically disabled when the program ends
 */
void startTrace(const ConnectionHolderPtr& conn) {
	startTrace(conn->connection());
}

/**
 * Starts SQL trace for a JDBC program. Also sets the timed statistics to
 * true. The SQL trace is automatically disabled when the program ends
 */
void startTrace(Connection* conn) {
	string setTimedStatisticsStmt = "alter session set timed_statistics=true";
	string setTraceStmt =
			"alter session set events '10046 trace name context forever, level 12'";
	Statement* stmt = NULL;
	try {
		stmt = conn->createStatement();
		stmt->execute(setTimedStatisticsStmt);
		stmt->execute(setTraceStmt);
	} catch (SQLException&) {
	}
	if (stmt) {
		conn->terminateStatement(stmt);
	}
}

void printStmtStatus(const Statement* stmt) {
	cout << "\tStatement status: ";
	Statement::Status statementStatus = stmt->status();

	switch (statementStatus) {
	case Statement::Status::NEEDS_STREAM_DATA:
		cout << "Needs stream data" << endl;
		break;
	case Statement::Status::PREPARED:
		cout << "Prepared" << endl;
		break;
	case Statement::Status::RESULT_SET_AVAILABLE:
		cout << "Result set available" << endl;
		break;
	case Statement::Status::STREAM_DATA_AVAILABLE:
		cout << "Stream data available" << endl;
		break;
	case Statement::Status::UNPREPARED:
		cout << "Unprepared" << endl;
		break;
	case Statement::Status::UPDATE_COUNT_AVAILABLE:
		cout << "Update count available" << endl;
		break;
	}
}

void printRsetStatus(const ResultSet* rset) {
	ResultSet::Status resultSetStatus = rset->status();
	cout << "\tResult set status: ";

	switch (resultSetStatus) {
	case ResultSet::Status::DATA_AVAILABLE:
		cout << "Data available" << endl;
		break;
	case ResultSet::Status::STREAM_DATA_AVAILABLE:
		cout << "Stream data available" << endl;
		break;
	case ResultSet::Status::END_OF_FETCH:
		cout << "End of fetch" << endl;
		break;
	}
}

/**
 *  Sets session cached cursor for the connection
 */
void setSessionCachedCursors(const ConnectionHolderPtr& conn,
		int sessionCachedCursors) {
	setSessionCachedCursors(conn->connection(), sessionCachedCursors);
}

/**
 *  Sets session cached cursor for the connection
 */
void setSessionCachedCursors(Connection* conn, int sessionCachedCursors) {
	string stmtStr = "alter session set session_cached_cursors="
			+ sessionCachedCursors;
	Statement* stmt = NULL;
	try {
		stmt = conn->createStatement();
		stmt->execute(stmtStr);
	} catch (SQLException&) {
	}
	if (stmt) {
		conn->terminateStatement(stmt);
	}
}

vector<PObject*> resultSetToList(ResultSet* rset) {
	vector<PObject*> rlist;
	if (!rset)
		return rlist;

	try {
		while (rset->next() == ResultSet::Status::DATA_AVAILABLE) {
			rlist.push_back(rset->getObject(0));
		}
		if (rset->status() == ResultSet::Status::END_OF_FETCH) {
			Statement* stmt = rset->getStatement();
			stmt->closeResultSet(rset);
		}
	} catch (SQLException& e) {
		printException(e);
	}
	rlist.shrink_to_fit();
	return rlist;
}

::oracle::occi::Blob& byteArrayToBlob(const vector<unsigned char>& byteArray) {
	typedef vector<unsigned char> byteArrayType;
	vector<unsigned char>& buf = const_cast<byteArrayType&>(byteArray);
	::oracle::occi::Blob* blob = new ::oracle::occi::Blob();
	try {
		blob->setEmpty();
		blob->write(buf.size(), buf.data(), buf.size());
	} catch (::oracle::occi::SQLException& sqle) {
		cerr << "SQLException: " << sqle.what() << endl;
	}
	return *blob;
}

vector<unsigned char>& blobToByteArray(const ::oracle::occi::Blob& blob) {
	::oracle::occi::Blob& b = const_cast< ::oracle::occi::Blob&>(blob);
	const vector<unsigned char>& ba = vector<unsigned char>();

	try {
		b.open(::oracle::occi::OCCI_LOB_READONLY);
		unsigned char buf[b.length()];
		b.read(b.length(), buf, b.length());
		const vector<unsigned char>& byteArray = vector<unsigned char>(buf, buf + b.length());
		b.close();
		return const_cast<vector<unsigned char>&>(byteArray);
	} catch (::oracle::occi::SQLException& sqle) {
		cerr << "SQLException: " << sqle.what() << endl;
	}
	return const_cast<vector<unsigned char>&>(ba);
}

::oracle::occi::Timestamp& millisToTimestamp(long long int millis) {
	::boost::posix_time::ptime p = ::boost::posix_time::from_time_t(
			millis / 1000) + ::boost::posix_time::millisec(millis % 1000);
	struct tm tm = ::boost::posix_time::to_tm(p);
	::oracle::occi::Environment* env =
			::oracle::occi::Environment::createEnvironment();
	::oracle::occi::Timestamp* ts = new ::oracle::occi::Timestamp(env, tm.tm_year, tm.tm_mon, tm.tm_mday,
			tm.tm_hour, tm.tm_min, tm.tm_sec, millis % 1000);
	::oracle::occi::Environment::terminateEnvironment(env);
	return *ts;
}

long long int timestampToMillis(const ::oracle::occi::Timestamp& timestamp) {
	struct tm tm;
	int &year = tm.tm_year;
	int &month = tm.tm_mon;
	int& day = tm.tm_mday;
	timestamp.getDate(year, (unsigned int&)month, (unsigned int&)day);
	int& hour = tm.tm_hour;
	int& minute = tm.tm_min;
	int& second = tm.tm_sec;
	int tm_msec;
	int& fs = tm_msec;
	timestamp.getTime((unsigned int&)hour, (unsigned int&)minute, (unsigned int&)second, (unsigned int&)fs);
	long long millis = tm.tm_year * 365.25 + tm.tm_mon * 12 + tm.tm_mday;
	millis *= 86400000;
	millis += tm.tm_hour * 3600000 + tm.tm_min * 60000 + tm.tm_sec * 1000 + tm_msec;
	return millis;
}
}
}
