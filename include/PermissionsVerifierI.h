#ifndef PERMISSIONSVERIFIERI_H_
#define PERMISSIONSVERIFIERI_H_

#include <Glacier2/PermissionsVerifier.h>
#include <string>

#include "Util.h"

using namespace std;

namespace Cardroid {
namespace Network {
namespace Server {

class PermissionsVerifierI: public ::Glacier2::PermissionsVerifier {

public:

	PermissionsVerifierI(
			Cardroid::occi::StatelessConnectionPoolHolder* connectionPool);
	virtual bool checkPermissions(const string& userId, const string& password,
			string& reason, const ::Ice::Current& = ::Ice::Current()) const;

private:
	Cardroid::occi::StatelessConnectionPoolHolder* _connectionPool;
	static const long long int serialVersionUID = 2574060917522212083L;
};
}
}
}

#endif /* PERMISSIONSVERIFIERI_H_ */
