#ifndef _ASYNC_CALLBACK_H
#define _ASYNC_CALLBACK_H

#include "AsyncCallbackBase.h"
#include "Callback.h"
#include "CallbackThread.h"

// See http://www.codeproject.com/Articles/1092727/Asynchronous-Multicast-Callbacks-with-Inter-Thread

struct NoData {};

/// @brief The AsyncCallback class provides asynchronous callbacks to registered
/// functions matching the CallbackFunc signature. When the AsyncCallback is invoked, 
/// each callback within the invocation list is invoked. This class is thread-safe. 
template<typename TData = NoData>
class AsyncCallback : public AsyncCallbackBase
{
public:
	/// Callback function signature
	typedef void (*CallbackFunc)(const TData& cbData, void* userData);

	/// @see AsyncCallbackBase::Register 
	void Register(CallbackFunc func, CallbackThread* thread, void* userData=NULL)
	{
		AsyncCallbackBase::Register(reinterpret_cast<Callback::CallbackFunc>(func), thread, userData);
	}

	/// @see AsyncCallbackBase::Unregister
	void Unregister(CallbackFunc func, CallbackThread* thread, void* userData=NULL)
	{
		AsyncCallbackBase::Unregister(reinterpret_cast<Callback::CallbackFunc>(func), thread, userData);
	}

	/// @see Invoke
	void operator()(const TData& data) 
	{
		Invoke(data);
	}

	/// Called to invoke callbacks on all registered clients.
	/// @param[in] data - the data to pass to each client callback function
	///		argument.
	void Invoke(const TData& data) 
	{
		const std::lock_guard<std::mutex> lock(GetLock());

		// For each registered callback 
		InvocationNode* node = GetInvocationHead();
		while (node != NULL)
		{
			// Create a new instance of callback and copy
			const Callback* callback = new Callback(*node->CallbackElement);

			// Create a new instance of the callback data and copy
			const TData* callbackData = new TData(data);

			// Create a new message  instance 
			CallbackMsg* msg = new CallbackMsg(this, callback, callbackData);

			// Dispatch message onto the callback destination thread. TargetInvoke()
			// will be called by the target thread. 
			callback->GetCallbackThread()->DispatchCallback(msg);

			// Get the next registered callback subscriber 
			node = node->Next;
		}
	}

	/// Called from the destination callback thread of control. 
	/// @param[in] msg - the incoming callback message. The object
	///		must be dynamically created. 
	/// @post The msg object is deleted before this function returns. 
	virtual void TargetInvoke(CallbackMsg** msg) const
	{
		const Callback* callback = (*msg)->GetCallback();

		// Typecast the void* back to a TData type
		const TData* callbackData = static_cast<const TData*>((*msg)->GetCallbackData());

		// Typecast a generic callback function pointer to the CallbackFunc type
		CallbackFunc func = reinterpret_cast<CallbackFunc>(callback->GetCallbackFunction());

		// Execute the registered callback function
		(*func)(*callbackData, callback->GetUserData());

		// Delete dynamically data sent through the message queue
		delete callbackData;
		delete callback;
		delete *msg;
		*msg = NULL;
	}
};

#endif