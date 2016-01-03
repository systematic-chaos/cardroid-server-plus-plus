#include "RegistrationI.h"

#include <IceUtil/Handle.h>
#include <occiCommon.h>
#include <occiControl.h>
#include <memory>

#include "Cardroid.h"
#include "DbTypes.h"
#include "IUser.h"

using namespace std;
using namespace Cardroid::Network::Server;

RegistrationI::RegistrationI(
		Cardroid::occi::StatelessConnectionPoolHolder* connectionPool) :
		_connectionPool(connectionPool) {
}

bool RegistrationI::registerNewUser(
		const cardroid::data::zerocice::UserTypPtr& newUser,
		const string& password, const ::Ice::Current&) {
	bool successfulRegistration;
	Cardroid::occi::ConnectionHolderPtr conn =
			new Cardroid::occi::ConnectionHolder(_connectionPool);
	oracle::occi::Statement* cstmt = NULL;
	int result = -1;
	string reason;

	Cardroid::Data::Oracle::User* u = static_pointer_cast< ::Cardroid::Data::Oracle::User>(
			Cardroid::Data::Oracle::User().newInstance(
					shared_ptr<Cardroid::Data::Oracle::IUser>(
							static_cast< ::Cardroid::Data::Ice::User*>(newUser.get())))).get();
	try {
		string sql92Style =
				"BEGIN callable_statements.register(:1, :2, :3, :4); END;";
		// Create the CallableStatement object
		cstmt = conn->connection()->createStatement(sql92Style);
		// Bind input parameters
		cstmt->setObject(1, u);
		cstmt->setString(2, password);
		// Register output parameters
		cstmt->registerOutParam(1, oracle::occi::Type::OCCIPOBJECT,
				sizeof(UserTyp*), User::getSqlTypeName());
		cstmt->registerOutParam(3, oracle::occi::Type::OCCIINT, sizeof(result));
		cstmt->registerOutParam(4, oracle::occi::Type::OCCICHAR,
				sizeof(reason) * 512);
		// Execute CallableStatement and retrieve results
		cstmt->execute();
		u = static_cast< ::Cardroid::Data::Oracle::User*>(cstmt->getObject(1));
		result = cstmt->getInt(3);
		reason = cstmt->getString(4);

		successfulRegistration = result == 0;
		if (successfulRegistration) {
			conn->commit();
		} else {
			conn->rollback();
			throw cardroid::network::zerocice::RegistrationDeniedException(
					reason);
		}
	} catch (const oracle::occi::SQLException& sqle) {
		successfulRegistration = false;
		conn->connection()->terminateStatement(cstmt);
		delete conn.get();
		throw cardroid::zerocice::OCCIException(
				("Registration failed. SQLException: " + sqle.getMessage()).c_str(),
				sqle.getErrorCode());
	}

	conn->connection()->terminateStatement(cstmt);
	delete conn.get();

	return successfulRegistration;
}
