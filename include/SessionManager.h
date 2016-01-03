#ifndef SESSIONMANAGER_H_
#define SESSIONMANAGER_H_

#include <string>

#include "SessionI.h"

using namespace std;

namespace Cardroid {
namespace Network {
namespace Server {

class SessionI;

/**
 * \interface SessionManager
 * Specification of operations for a SessionManager,
 * in addition to those of Glacier2.SessionManager
 */
class SessionManager {

public:

	/**
	 * Destroy an existing session
	 * @param session	The user session to be destroyed
	 * @param userName	The user id for the session
	 */
	virtual void destroy(const SessionI& session, const ::std::string& userName) = 0;

protected:

	virtual ~SessionManager() {
	}
};
}
}
}

#endif /* SESSIONMANAGER_H_ */
