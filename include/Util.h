#ifndef UTIL_H_
#define UTIL_H_

#include <IceUtil/Handle.h>
#include <IceUtil/Shared.h>
#include <occi.h>
#include <string>
#include <vector>

namespace Cardroid {
namespace occi {

class EnvironmentHolder: public IceUtil::Shared {
public:

	EnvironmentHolder(bool threaded = false, bool object = false);
	~EnvironmentHolder();

	oracle::occi::Environment* environment() const {
		return _env;
	}

private:
	oracle::occi::Environment* _env;
};

class StatelessConnectionPoolHolder: public IceUtil::Shared {
public:

	StatelessConnectionPoolHolder(oracle::occi::Environment* env,
			const std::string& userName, const std::string& password,
			const std::string& connectString, int maxCon = 10, int minCon = 5,
			int incrCon = 2);
	StatelessConnectionPoolHolder(EnvironmentHolder* env,
			const std::string& userName, const std::string& password,
			const std::string& connectString, int maxCon = 10, int minCon = 5,
			int incrCon = 2);
	~StatelessConnectionPoolHolder();

	oracle::occi::StatelessConnectionPool* pool() const {
		return _pool;
	}

private:
	oracle::occi::Environment* _env;
	oracle::occi::StatelessConnectionPool* _pool;
};

/**
 * \class ConnectionHolder
 * Grab a connection from a connection pool and ensure it is
 * properly released when the ConnectionHolder is destroyed
 */
class ConnectionHolder: public IceUtil::Shared {
public:

	ConnectionHolder(oracle::occi::StatelessConnectionPool* pool);
	ConnectionHolder(StatelessConnectionPoolHolder* pool);
	ConnectionHolder(oracle::occi::Environment* env,
			const std::string& userName, const std::string& password,
			const std::string& connectString);
	ConnectionHolder(EnvironmentHolder* env, const std::string& userName,
			const std::string& password, const std::string& connectString);
	~ConnectionHolder();

	oracle::occi::Connection* connection() const {
		return _con;
	}

	void commit();
	void rollback();

private:

	oracle::occi::Connection* _con;
	bool _txDone;
	oracle::occi::Environment* _env;
	oracle::occi::StatelessConnectionPool* _pool;
};

typedef IceUtil::Handle<ConnectionHolder> ConnectionHolderPtr;

/**
 * \class StatementHolder
 * Create a fresh exception-safe statement (typically on the stack)
 */
class StatementHolder {
public:
	StatementHolder(oracle::occi::Connection*, const ::std::string& sql);
	StatementHolder(const ConnectionHolderPtr&, const ::std::string& sql);
	~StatementHolder();

	oracle::occi::Statement* statement() const {
		return _stmt;
	}

private:

	oracle::occi::Connection* _con;
	oracle::occi::Statement* _stmt;
};

/// Encode/decode a RefAny into a string
std::string encodeRef(const oracle::occi::RefAny&, oracle::occi::Environment*);
oracle::occi::RefAny decodeRef(const std::string&, oracle::occi::Environment*v,
		oracle::occi::Connection*);

void close(oracle::occi::ResultSet* resultSet,
		oracle::occi::Statement* statement,
		oracle::occi::Connection* connection,
		oracle::occi::Environment* environment);
void close(oracle::occi::ResultSet* resultSet,
		oracle::occi::Statement* statement);
void close(oracle::occi::ResultSet* resultSet);
void close(oracle::occi::StatelessConnectionPool* pool,
		oracle::occi::Environment* environment);
void printException(const std::exception& e);
void printExceptionAndRollback(const ConnectionHolderPtr& conn,
		const std::exception& e);
void printExceptionAndRollback(oracle::occi::Connection* conn,
		const std::exception& e);
void startTrace(const ConnectionHolderPtr& conn);
void startTrace(oracle::occi::Connection* conn);
void printStmtStatus(const oracle::occi::Statement* stmt);
void printRsetStatus(const oracle::occi::ResultSet* rset);
void setSessionCachedCursors(const ConnectionHolderPtr& conn,
		int sessionCachedCursors);
void setSessionCachedCursors(oracle::occi::Connection* conn,
		int sessionCachedCursors);
std::vector<oracle::occi::PObject*> resultSetToList(
		oracle::occi::ResultSet* rset);
::oracle::occi::Blob& byteArrayToBlob(
		const ::std::vector<unsigned char>& byteArray);
::std::vector<unsigned char>& blobToByteArray(const ::oracle::occi::Blob& blob);
::oracle::occi::Timestamp& millisToTimestamp(long long int millis);
long long int timestampToMillis(const ::oracle::occi::Timestamp& timestamp);
}
}

#endif /* UTIL_H_ */
