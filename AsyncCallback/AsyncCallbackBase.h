#ifndef _ASYNC_CALLBACK_BASE_H
#define _ASYNC_CALLBACK_BASE_H

#include "Fault.h"
#include "Callback.h"
#include "CallbackMsg.h"
#include <mutex>

/// @brief A non-template base class for the async callbacks. This class is 
/// thread-safe.
/// @details Since the AsyncCallback template class inherits from this class, 
/// as much code is placed into this base class as possible to minimize the
/// template code instantiation. 
class AsyncCallbackBase
{
public:
	/// Constructor
	AsyncCallbackBase();

	/// Destructor
	virtual ~AsyncCallbackBase();

	/// Called to invoke the callback by the destination thread of control. 
	/// @param[in] msg - the incoming callback message. 
	virtual void TargetInvoke(CallbackMsg** msg) const = 0;

protected:
	struct InvocationNode
	{
		InvocationNode() : Next(NULL), CallbackElement(NULL) { }
		InvocationNode* Next;
		const Callback* CallbackElement;
	};

	/// Register for an asynchronous callback. Inserts the into the invocation list.
	/// @param[in] func - the target callback function.
	/// @param[in] thread - target thread to execute the asynchronous callback.
	/// @param[in] userData - optional user data returned as-is on the callback. 
	///		Callback framework doesn't use userData other than passing it back
	///		upon CallbackFunc invocation. The userData can point to anything
	///		or NULL. 
	void Register(Callback::CallbackFunc func, CallbackThread* thread, void* userData=NULL);

	/// Unregister from a previously registered callback. 
	/// @param[in] callback - a callback to unregister. 
	void Unregister(Callback::CallbackFunc func, CallbackThread* thread, void* userData=NULL);

	/// Get the head of the callback invocation list. 
	/// @return Pointer to the head of the invocation list. 
	InvocationNode* GetInvocationHead() { return m_invocationHead; }

	/// Get the software lock.
	/// @return The software lock instance.
	std::mutex& GetLock() { return m_lock; }

private:
	// Safe bool idiom
    typedef void (AsyncCallbackBase::*bool_type)() const;
    void this_type_does_not_support_comparisons() const {}
public:
	operator bool_type() const {
		return Empty()? 0 : &AsyncCallbackBase::this_type_does_not_support_comparisons;
    }
	bool operator !() const { return !Empty(); }
	bool Empty() const { return !m_invocationHead; }
	void Clear();

private:
	/// Head pointer to the callback invocation list
	InvocationNode* m_invocationHead;

	/// Lock to make the class thead-safe
	std::mutex m_lock;
};

#endif
