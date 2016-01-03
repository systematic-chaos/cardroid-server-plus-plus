#ifndef REGISTRATIONI_H_
#define REGISTRATIONI_H_

#include <string>

#include "Registration.h"
#include "Util.h"

using namespace std;

namespace Cardroid {
namespace Network {
namespace Server {

class RegistrationI: public cardroid::network::zerocice::Registration {

public:

	RegistrationI(
			Cardroid::occi::StatelessConnectionPoolHolder* connectionPool);
	bool registerNewUser(const cardroid::data::zerocice::UserTypPtr& newUser,
			const std::string& password, const ::Ice::Current& = ::Ice::Current()) override;

private:

	Cardroid::occi::StatelessConnectionPoolHolder* _connectionPool;
	static const long long int serialVersionUID = 3502112289481155093L;
};
}
}
}

#endif /* REGISTRATIONI_H_ */
