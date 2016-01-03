#include "PermissionsVerifierI.h"

#include <occiCommon.h>
#include <occiControl.h>

#include "Cardroid.h"

using namespace std;
using namespace Cardroid::Network::Server;

/**
 * Default constructor
 * @param connectionPool	Reference to the database connection pool.
 * 								The permissions verifier will need to acquire
 * 								a database connection to check the user's
 * 								access credentials against the database
 */
PermissionsVerifierI::PermissionsVerifierI(
		Cardroid::occi::StatelessConnectionPoolHolder* connectionPool) :
		_connectionPool(connectionPool) {
}

/**
 * Check whether a user has permission to access the router
 * @param userId	The user id for which to check permission
 * @param password	The user's password
 * @param reason	The reason why access was denied
 * @return			True if access is granted, or false otherwise
 * @throws PermissionDeniedException
 */
bool PermissionsVerifierI::checkPermissions(const string& userId,
		const string& password, string& reason, const ::Ice::Current&) const {
	bool accessGranted;
	Cardroid::occi::ConnectionHolderPtr conn =
			new Cardroid::occi::ConnectionHolder(_connectionPool);
	::oracle::occi::Statement* cstmt = NULL;
	int result = -1;

	try {
		string sql92style =
				"BEGIN :1 := callable_statements.login(:2, :3); END;";
		// Create the CallableStatement object
		cstmt = conn->connection()->createStatement(sql92style);
		// Bind input parameters
		cstmt->setString(2, userId);
		cstmt->setString(3, password);
		// Register output parameters
		cstmt->registerOutParam(1, ::oracle::occi::Type::OCCIINT, sizeof(result));
		// Execute CallableStatement and retrieve results
		cstmt->execute();
		result = cstmt->getInt(1);
	} catch (const ::oracle::occi::SQLException& sqle) {
		conn->connection()->terminateStatement(cstmt);
		delete conn.get();
		throw cardroid::zerocice::OCCIException(sqle.getMessage().c_str(), sqle.getErrorCode());
	}

	conn->connection()->terminateStatement(cstmt);
	delete conn.get();

	switch (result) {
	case 0:
		accessGranted = true;
		reason = "Access granted.";
		break;
	case 1:
		accessGranted = false;
		reason = "Access denied. Incorrect login/password pair.";
		break;
	case 2:
		accessGranted = false;
		reason =
				"Access denied. Supplied user email is not registered in the system.";
		break;
	default:
		accessGranted = false;
		reason = "Access denied.";
	}

	return accessGranted;
}
