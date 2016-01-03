#include <Ice/Ice.h>
#include <Glacier2/Glacier2.h>
#include <IceStorm/IceStorm.h>
#include <IceSSL/IceSSL.h>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

#include "PermissionsVerifierI.h"
#include "Glacier2SessionManagerI.h"
#include "ReapThread.h"
#include "SessionFactoryI.h"
#include "RegistrationI.h"
#include "LocatorI.h"
#include "DbTypesMap.h"

using namespace std;
using namespace ::Ice;

namespace Cardroid {
namespace Network {
namespace Server {

class PasswordCallback : public IceSSL::PasswordPrompt {

public:

	PasswordCallback(const string& passwd) {
		password = passwd;
	}

	~PasswordCallback() {
	}

	virtual string getPassword() {
		return password;
	}

private:

	string password;
};

/**
 * \class Server
 * CarDroid server main class.
 * It is an Ice Application, and initializes all the subsystems and components
 * of the runtime needed for the server operation. At the time the Communicator
 * for the application is shutdown, it commands the initialized modules to
 * perform the necessary cleanup before finishing the server execution in a
 * controlled manner
 */
class CardroidServer: public Application {

public:

	virtual int run(int argc, char* argv[]) {
		CommunicatorPtr communicator = Application::communicator();
		StringSeq args =
				communicator->getProperties()->parseCommandLineOptions("Oracle",
						argsToStringSeq(argc, argv));

		if (argc > 1) {
			cerr << appName() << ": too many arguments" << endl;
			return EXIT_FAILURE;
		}

		PropertiesPtr properties = communicator->getProperties();

		// Parse the database configuration properties from the config file
		string username = properties->getProperty("Oracle.Username");
		string password = properties->getProperty("Oracle.Password");
		string connectString = properties->getProperty("Oracle.ConnectString");
		int nConnections = properties->getPropertyAsIntWithDefault(
				"Oracle.NumConnections", 8);
		if (nConnections < 1) {
			nConnections = 1;
		}

		long timeout = properties->getPropertyAsIntWithDefault("SessionTimeout",
				240);

		/*InitializationData initData;
		initData.properties = createProperties(argc, argv);
		initData.properties->setProperty("IceSSL.Password", password);
		communicator = initialize(initData);*/

		PluginManagerPtr pluginMgr = communicator->getPluginManager();
		PluginPtr plugin = pluginMgr->getPlugin("IceSSL");
		IceSSL::PluginPtr sslPlugin = IceSSL::PluginPtr::dynamicCast(plugin);
		sslPlugin->setPasswordPrompt(new PasswordCallback(
				properties->getProperty("IceSSL.Password")));
		pluginMgr->initializePlugins();

		LoggerPtr logger = communicator->getLogger();
		::Cardroid::occi::EnvironmentHolder* env = 0;
		::Cardroid::occi::StatelessConnectionPoolHolder* pool = 0;

		try {
			env = new ::Cardroid::occi::EnvironmentHolder(true, true);
			DbTypesMap(env->environment());

			// Create the database connections pool
			pool = new ::Cardroid::occi::StatelessConnectionPoolHolder(env,
					username, password, connectString, nConnections, 2, 1);
		} catch (...) {
			if (pool) {
				delete pool;
			}
			if (env) {
				delete env;
			}
			communicator->destroy();
			cerr << "Failed to create connection pool: SQLException" << endl;
			throw;
			return EXIT_FAILURE;
		}

		// Create an object adapter
		ObjectAdapterPtr adapter = communicator->createObjectAdapter(
				"CardroidServer");

		/* Add the servant locator for every object category to the object *
		 * adapter. The calls to the newly created servants are redirected *
		 * via a dispatch interceptor                                      */
		adapter->addServantLocator(new PlaceLocatorI(), "place");
		adapter->addServantLocator(new CarLocatorI(), "car");
		adapter->addServantLocator(new UserLocatorI(adapter), "user");
		adapter->addServantLocator(new TripLocatorI(), "trip");
		adapter->addServantLocator(new TripOfferLocatorI(adapter),
				"trip_offer");
		adapter->addServantLocator(new TripRequestLocatorI(adapter),
				"trip_request");
		adapter->addServantLocator(new UserActivityLocatorI(adapter),
				"user_activity");
		adapter->addServantLocator(new MessageLocatorI(adapter), "message");

		/* Add the object factory for instantiating servants from proxies for *
		 * every datatype used, which distinguishes itself by the category of *
		 * the identity provided                                              */
		communicator->addObjectFactory(new Data::Ice::Car(),
				::cardroid::data::zerocice::CarTyp::ice_staticId());
		communicator->addObjectFactory(new Data::Ice::Date(),
				::cardroid::data::zerocice::DateTyp::ice_staticId());
		communicator->addObjectFactory(new Data::Ice::DateTime(),
				::cardroid::data::zerocice::DateTimeTyp::ice_staticId());
		communicator->addObjectFactory(new Data::Ice::DateTimePrefs(),
				::cardroid::data::zerocice::DateTimePrefsTyp::ice_staticId());
		communicator->addObjectFactory(new Data::Ice::Message(adapter),
				::cardroid::data::zerocice::MessageTyp::ice_staticId());
		communicator->addObjectFactory(new Data::Ice::Passenger(adapter),
				::cardroid::data::zerocice::PassengerTyp::ice_staticId());
		communicator->addObjectFactory(new Data::Ice::Place(),
				::cardroid::data::zerocice::PlaceTyp::ice_staticId());
		communicator->addObjectFactory(new Data::Ice::Trip(),
				::cardroid::data::zerocice::TripTyp::ice_staticId());
		communicator->addObjectFactory(new Data::Ice::TripOffer(adapter),
				::cardroid::data::zerocice::TripOfferTyp::ice_staticId());
		communicator->addObjectFactory(new Data::Ice::TripRequest(adapter),
				::cardroid::data::zerocice::TripRequestTyp::ice_staticId());
		communicator->addObjectFactory(new Data::Ice::User(adapter),
				::cardroid::data::zerocice::UserTyp::ice_staticId());
		communicator->addObjectFactory(new Data::Ice::UserActivity(adapter),
				::cardroid::data::zerocice::UserActivityTyp::ice_staticId());
		communicator->addObjectFactory(new Data::Ice::Waypoint(),
				::cardroid::data::zerocice::WaypointTyp::ice_staticId());

		/* Retrieve the topic manager used for the publishing and *
		 * subscription features by the IceStorm service          */
		IceStorm::TopicManagerPrx topicManager =
				IceStorm::TopicManagerPrx::checkedCast(
						communicator->propertyToProxy("TopicManager.Proxy"));
		if (!topicManager) {
			cerr << "Invalid topic manager proxy" << endl;
			return EXIT_FAILURE;
		}

		SQLRequestContext::initialize(logger, pool, topicManager);

		// Create the thread responsible for destroying inactive sessions
		ReapThread reaper(logger, topicManager, timeout);
		boost::thread boostReaper = boost::thread(
				boost::bind(&ReapThread::run, &reaper));

		/* Add to the object adapter the servant responsible for creating user *
		 * sessions for those clients running on a platform capable to work    *
		 * altogether with the Glacier2 router. Also add to the object adapter *
		 * the PermissionsVerifier object servant used by Glacier2 for user    *
		 * authentication                                                      */
		adapter->add(new Glacier2SessionManagerI(topicManager, logger, reaper),
				communicator->stringToIdentity("CardroidSessionManager"));
		adapter->add(new PermissionsVerifierI(pool),
				communicator->stringToIdentity("CardroidSessionVerifier"));

		/* Add to the object adapter the servant responsible for creating user *
		 * sessions for those clients running on a platform unable to work	   *
		 * altogether with Glacier2											   */
		if (properties->getPropertyAsIntWithDefault("SessionFactory", 0) > 0) {
			adapter->add(
					new SessionFactoryI(topicManager, logger, reaper, timeout),
					communicator->stringToIdentity("CardroidServer"));
		}

		// Everything ok, let's go
		adapter->activate();

		// Create another object adapter for the registration procedure
		ObjectAdapterPtr registrationAdapter =
				communicator->createObjectAdapter("CardroidRegistration");
		registrationAdapter->add(new RegistrationI(pool),
				communicator->stringToIdentity("Registration"));
		registrationAdapter->activate();

		/* Wait until any signal interruption, then shutdown the *
		 * Communicator and perform the necessary cleanup        */
		shutdownOnInterrupt();
		communicator->waitForShutdown();
		ignoreInterrupt();

		/* Send the terminate command to the reaper thread and wait for it *
		 * to finish the user sessions and IceStorm topics destruction process */
		reaper.terminate();
		boostReaper.join();

		/* Send the destroy command to the database connections pool. This     *
		 * operation will return when every transaction in progress has been   *
		 * committed or rolled back, and every connection in the pool has been *
		 * released and closed                                                 */
		if (pool && pool->pool()) {
			delete pool;
		}
		if (env && env->environment()) {
			delete env;
		}

		return EXIT_SUCCESS;
	}
};
}
}
}

int main(int argc, char* argv[]) {
	Cardroid::Network::Server::CardroidServer app;

	/* Start the server application, loading its configuration *
	 * from the config file and the command-line arguments     */
	return app.main(argc, argv, "config.server");
}
