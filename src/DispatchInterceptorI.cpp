#include "Cardroid.h"
#include "DispatchInterceptorI.h"
#include "SQLRequestContext.h"

using namespace Cardroid::Network::Server;
using namespace ::Ice;

/**
 * Default constructor
 * @param servant The servant the intercepted requests will be forwarded to
 */
DispatchInterceptorI::DispatchInterceptorI(const ObjectPtr& servant) :
	_servant(servant) {
}

/**
 * Called by the Ice run time to dispatch an incoming request.
 * Dispatches the request to the actual servant.
 */
DispatchStatus DispatchInterceptorI::dispatch(Request& request) {
	// Allocate a new SQLRequestContext associated with this request thread
	SQLRequestContext* context = new SQLRequestContext();
	try {
		DispatchStatus status = _servant->ice_dispatch(request);

		// An exception causes the currentEntry transaction to rollback
		context->destroyFromDispatch(status == DispatchStatus::DispatchOK);

		return status;
	} catch (cardroid::zerocice::OCCIException& ex) {
		// Log the error
		Current c = request.getCurrent();
		context->error(
				"Call of " + c.operation + " on id " + c.id.category + "/"
						+ c.id.name + " failed", ex);

		// A SqlException causes the currentEntry transaction to rollback
		context->destroyFromDispatch(false);

		// Translate the exception to UnknownException
		throw UnknownException(ex.ice_file(), ex.ice_line(), ex.ice_stackTrace());
	} catch (...) {
		// Any other exception causes the transaction to rollback
		context->destroyFromDispatch(false);
		throw;
	}
}
