#ifndef _CALLBACK_THREAD_H
#define _CALLBACK_THREAD_H

#include "DataTypes.h"
#include "CallbackMsg.h"

/// @brief Each platform specific implementation must inherit from CallbackThread
/// and provide an implementation for DispatchCallback().
class CallbackThread
{
public:
	/// Destructor
	virtual ~CallbackThread() {}

	/// Dispatch a CallbackMsg onto this thread. The implementer is responsible
	/// for getting the CallbackMsg into an OS message queue. Once CallbackMsg
	/// is on the correct thread of control, the AysncCallbackBase::TargetInvoke() function
	/// must be called to execute the callback. 
	/// @param[in] msg - a pointer to the callback message that must be created dynamically
	///		using operator new. 
	/// @pre Caller *must* create the CallbackMsg argument dynamically using operator new.
	/// @post The destination thread must delete the msg instance by calling TargetInvoke().
	virtual void DispatchCallback(CallbackMsg* msg) = 0;
};

#endif
