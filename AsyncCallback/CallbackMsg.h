#ifndef _CALLBACK_MSG_H
#define _CALLBACK_MSG_H

#include "DataTypes.h"
#include "Fault.h"

class Callback;
class AsyncCallbackBase;

/// @brief A class containing the callback information passed through 
/// the message queue. 
class CallbackMsg 
{
public:
	/// Constructor
	/// @param[in] asyncCallback - the async callback instance the callback is registered
	///		with.
	/// @param[in] callback - the callback instance. 
	/// @param[in] callbackData - the data sent as callback function argument.
	CallbackMsg(AsyncCallbackBase* asyncCallback, const Callback* callback, const void* callbackData) :
		m_asyncCallback(asyncCallback),
	  	m_callback(callback),
		m_callbackData(callbackData)
	{
		ASSERT_TRUE(m_asyncCallback != NULL);
		ASSERT_TRUE(m_callback != NULL);
		ASSERT_TRUE(m_callbackData != NULL);
	}

	/// Get the async callback instance the callback is registered with.
	/// @return The async callback instance. 
	const AsyncCallbackBase* GetAsyncCallback() const
	{
		return m_asyncCallback;
	}
	
	/// Get the callback.
	/// @return The callback instance. 
	const Callback* GetCallback() const
	{
		return m_callback;
	}

	/// Get the callback data passed into the callback function. 
	/// @return The callback data. 
	const void* GetCallbackData() const 
	{
		return m_callbackData;
	}

private:
	/// The AsyncCallback instance
	AsyncCallbackBase* m_asyncCallback;

	/// The callback instance
	const Callback* m_callback;

	/// The data argument passed into the callback function
	const void* m_callbackData;
};

#endif