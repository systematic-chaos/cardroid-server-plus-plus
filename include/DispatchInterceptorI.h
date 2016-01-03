#ifndef DISPATCHINTERCEPTORI_H_
#define DISPATCHINTERCEPTORI_H_

#include <Ice/DispatchInterceptor.h>
#include <Ice/Object.h>

namespace Cardroid {
namespace Network {
namespace Server {

/**
 * \class DispatchInterceptorI
 * Requests interceptor. Is used to automatically commit or rollback requests
 * forwarded to a servant, depending on the success of their completion
 */
class DispatchInterceptorI: public ::Ice::DispatchInterceptor {
public:

	DispatchInterceptorI(const ::Ice::ObjectPtr& servant);
	virtual ::Ice::DispatchStatus dispatch(::Ice::Request& request);

private:

	::Ice::ObjectPtr _servant;
	static const long long int serialVersionUID = -3418216384163689271L;
};

typedef IceInternal::Handle< ::Ice::DispatchInterceptor> DispatchInterceptorPtr;

}
}
}

#endif /* DISPATCHINTERCEPTORI_H_ */
