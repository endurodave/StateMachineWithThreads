#ifndef _CALLBACK_H
#define _CALLBACK_H

#include "DataTypes.h"

class CallbackThread;

/// @brief Callback stores information about a registered callback client. 
class Callback
{
public:
	/// Generic callback function signature
	typedef void (*CallbackFunc)(const void* cbData, void* userData);

	/// Constructor
	/// @param[in] func - the target callback function.
	/// @param[in] thread - target thread to execute the asynchronous callback.
	/// @param[in] userData - optional user data returned as-is on the callback. 
	///		Callback framework doesn't use userData other than passing it back
	///		upon CallbackFunc invocation. The userData can point to anything
	///		or NULL. 
	Callback(CallbackFunc func, CallbackThread* thread, void* userData = NULL) :
		m_thread(thread),
		m_func(func),
		m_userData(userData)
	{
	}

	/// Get the thread instance
	/// @return The thread instance to invoke the callback. 
	CallbackThread* GetCallbackThread() const
	{ 
		return m_thread;
	}
	
	/// Get the user data
	/// @return The user data
	void* GetUserData() const
	{
		return m_userData;
	}
	
	/// Get a pointer to the callback function.  The callback function must be 
	/// typecast to a AsyncCallback::CallbackFunc before invoking.
	/// @return A pointer to the callback function
	CallbackFunc GetCallbackFunction() const
	{
		return m_func;
	}	

	bool operator==(const Callback& rhs) const
	{
		return m_thread == rhs.m_thread &&
			m_func == rhs.m_func &&
			m_userData == rhs.m_userData;
	}

	bool operator!=(const Callback& rhs) const
	{
		return !(*this == rhs);
	}

private:
	/// Pointer to the thread the callback is to be invoked from.
	CallbackThread* m_thread;

	/// Callback user data passed on each callback function call.
	void* m_userData;

	/// A pointer to the callback function which will need to be typecast
	/// to a AsyncCallback::CallbackFunc type before invoking the 
	/// function callback
	CallbackFunc m_func;
};

#endif
