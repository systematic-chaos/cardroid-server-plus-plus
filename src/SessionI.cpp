#include "SessionI.h"

#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/date_time/microsec_time_clock.hpp>
#include <boost/date_time/posix_time/posix_time_config.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/time_duration.hpp>
#include <Ice/Communicator.h>
#include <Ice/Current.h>
#include <Ice/Identity.h>
#include <Ice/LocalException.h>
#include <Ice/Proxy.h>
#include <Ice/ProxyHandle.h>
#include <memory>
#include <algorithm>
#include <vector>

#include "DispatchInterceptorI.h"

using namespace std;
using namespace Cardroid::Network::Server;
using namespace ::Ice;

boost::mutex SessionI::_synchronized;

SessionI::SessionI(const LoggerPtr& logger, bool trace,
		const ObjectAdapterPtr& adapter, const IceStorm::TopicPrx& topic,
		const string& userId, SessionManager& sessionManager, bool glacier2) :
		_logger(logger), _topic(topic), _userId(userId), _sessionManager(
				sessionManager), _glacier2(glacier2), _cardroidManagerServant(
				*(new CardroidManagerI())), _cardroidManagerProxy(
				cardroid::zerocice::CardroidManagerPrx::uncheckedCast(
						adapter->addWithUUID(
								new DispatchInterceptorI(
										&_cardroidManagerServant)))) {
	refresh();
}

cardroid::zerocice::CardroidManagerPrx SessionI::getCardroidManager(
		const Current&) {
	_synchronized.lock();

	if (_destroyed) {
		_synchronized.unlock();
		throw ObjectNotExistException(__FILE__, __LINE__);
	}
	cardroid::zerocice::CardroidManagerPrx cardroidManagerProxy =
			_cardroidManagerProxy;

	_synchronized.unlock();
	return cardroidManagerProxy;
}

IceStorm::TopicPrx SessionI::getTopic(const Current&) {
	return _topic;
}

void SessionI::refresh(const Current&) {
	_synchronized.lock();

	if (_destroyed) {
		_synchronized.unlock();
		throw ObjectNotExistException(__FILE__, __LINE__);
	}
	boost::posix_time::time_duration diff =
			boost::posix_time::microsec_clock::universal_time()
					- boost::posix_time::ptime(
							boost::gregorian::date(1970, 1, 1));
	_timeout = diff.total_milliseconds();

	_synchronized.unlock();
}

/**
 * @return The timeout for this session
 */
long SessionI::getSessionTimeout() {
	_synchronized.lock();

	if (_destroyed) {
		_synchronized.unlock();
		throw ObjectNotExistException(__FILE__, __LINE__);
	}
	long timeout = _timeout;

	_synchronized.unlock();
	return timeout;
}

void SessionI::destroy(const Current& current) {
	_synchronized.lock();

	if (_destroyed) {
		_synchronized.unlock();
		throw ObjectNotExistException(__FILE__, __LINE__);
	}

	_destroyed = true;
	_logger->trace("Session",
			"Session "
					+ current.adapter->getCommunicator()->identityToString(
							current.id) + " is now destroyed.");

	// This method is never called on shutdown of the server
	_cardroidManagerServant.destroy();
	current.adapter->remove(_cardroidManagerProxy->ice_getIdentity());
	current.adapter->remove(current.id);
	if (!_glacier2) {
		_sessionManager.destroy(*this, _userId);
	}

	_synchronized.unlock();
}

/**
 * Called on application shutdown
 */
void SessionI::shutdown() {
	_synchronized.lock();

	if (!_destroyed) {
		_destroyed = true;
		_cardroidManagerServant.shutdown();
	}

	_synchronized.unlock();
}

namespace {
const string __Cardroid__Network__Server__SessionI_ids[4] = { "::Ice::Object",
		"::Glacier2::Session", "::cardroid::network::zerocice::Session",
		"::Cardroid::Network::Server::SessionI" };

const string __Cardroid__Network__Server__SessionI_all[] = {
		"getCardroidManager", "getTopic", "refresh", "destroy", "ice_isA",
		"ice_ids", "ice_id" };
}

bool SessionI::ice_isA(const string& _s, const Current&) const {
	return binary_search(__Cardroid__Network__Server__SessionI_ids,
			__Cardroid__Network__Server__SessionI_ids + 4, _s);
}

vector<string> SessionI::ice_ids(const Current&) const {
	return vector<string>(&__Cardroid__Network__Server__SessionI_ids[0],
			&__Cardroid__Network__Server__SessionI_ids[4]);
}

const string& SessionI::ice_id(const Current&) const {
	return __Cardroid__Network__Server__SessionI_ids[3];
}

DispatchStatus SessionI::__dispatch(IceInternal::Incoming& in,
		const Current& current) {
	pair<const string*, const string*> r = equal_range(
			__Cardroid__Network__Server__SessionI_all,
			__Cardroid__Network__Server__SessionI_all + 7, current.operation);

	if (r.first == r.second) {
		throw OperationNotExistException(__FILE__, __LINE__, current.id,
				current.facet, current.operation);
	}

	switch (r.first - __Cardroid__Network__Server__SessionI_all) {
	case 0: {
		return ___getCardroidManager(in, current);
	}
	case 1: {
		return ___getTopic(in, current);
	}
	case 2: {
		return ___refresh(in, current);
	}
	case 3: {
		return ___destroy(in, current);
	}
	case 4: {
		return ___ice_isA(in, current);
	}
	case 5: {
		return ___ice_ids(in, current);
	}
	case 6: {
		return ___ice_id(in, current);
	}
	}

	assert(false);
	throw OperationNotExistException(__FILE__, __LINE__, current.id,
			current.facet, current.operation);
}

DispatchStatus SessionI::___destroy(IceInternal::Incoming& __inS,
		const Current& __current) {
	__checkMode(Idempotent, __current.mode);
	__inS.readEmptyParams();
	destroy(__current);
	__inS.__writeEmptyParams();
	return DispatchOK;
}

const string& SessionI::ice_staticId() {
	return __Cardroid__Network__Server__SessionI_ids[3];
}

void SessionI::__writeImpl(IceInternal::BasicStream* __os) const {
	__os->startWriteSlice(ice_id(), -1, true);
	__os->endWriteSlice();
}

void SessionI::__readImpl(IceInternal::BasicStream* __is) {
	__is->startReadSlice();
	__is->endReadSlice();
}
