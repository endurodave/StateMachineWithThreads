#include "AsyncCallbackBase.h"

//------------------------------------------------------------------------------
// Constructor
//------------------------------------------------------------------------------
AsyncCallbackBase::AsyncCallbackBase() :
	m_invocationHead(NULL)
{
	LockGuard::Create(&m_lock);
}

//------------------------------------------------------------------------------
// Destructor
//------------------------------------------------------------------------------
AsyncCallbackBase::~AsyncCallbackBase()
{
	LockGuard::Destroy(&m_lock);
}

//------------------------------------------------------------------------------
// Register
//------------------------------------------------------------------------------
void AsyncCallbackBase::Register(Callback::CallbackFunc func, CallbackThread* thread, void* userData)
{
	LockGuard lockGuard(&m_lock);
	
	InvocationNode* node = new InvocationNode();
	node->CallbackElement = new Callback(func, thread, userData);
	
	// First element in the list?
	if (m_invocationHead == NULL)
	{
		// Set the head element
		m_invocationHead = node;
	}
	else
	{
		// Iterate over list until the end of list is found
		InvocationNode* curr = m_invocationHead;
		while (curr->Next != NULL)
			curr = curr->Next;
		
		// Set the info pointer at the end of the list
		curr->Next = node;
	}
}

//------------------------------------------------------------------------------
// Unregister
//------------------------------------------------------------------------------
void AsyncCallbackBase::Unregister(Callback::CallbackFunc func, CallbackThread* thread, void* userData)
{
	LockGuard lockGuard(&m_lock);
	
	// Iterate over list to find callback to remove
	InvocationNode* curr = m_invocationHead;
	InvocationNode* prev = NULL;

	const Callback callback(func, thread, userData);
	while (curr != NULL)
	{
		// Is this the callback to remove?
		if (*curr->CallbackElement == callback)
		{
			if (curr == m_invocationHead)
				m_invocationHead = curr->Next;
			else
				prev->Next = curr->Next;
			
			delete curr->CallbackElement;
			delete curr;
			break;
		}
		prev = curr;
		curr = curr->Next;
	}	
}

//------------------------------------------------------------------------------
// Clear
//------------------------------------------------------------------------------
void AsyncCallbackBase::Clear()
{
	LockGuard lockGuard(&m_lock);	
	while (m_invocationHead)
	{
		InvocationNode* curr = m_invocationHead;
		m_invocationHead = curr->Next;
		delete curr->CallbackElement;
		delete curr;
	}
}

