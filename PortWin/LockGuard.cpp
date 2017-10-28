#include "LockGuard.h"
#include "Fault.h"

//------------------------------------------------------------------------------
// LockGuard
//------------------------------------------------------------------------------
LockGuard::LockGuard(LOCK* lock)
{
	m_lock = lock;
	EnterCriticalSection(m_lock); 
}

//------------------------------------------------------------------------------
// ~MutexLockGuard
//------------------------------------------------------------------------------
LockGuard::~LockGuard()
{
	LeaveCriticalSection(m_lock);
}

//------------------------------------------------------------------------------
// Create
//------------------------------------------------------------------------------
void LockGuard::Create(LOCK* lock)
{
	BOOL lockSuccess = InitializeCriticalSectionAndSpinCount(lock, 0x00000400); 
	ASSERT_TRUE(lockSuccess != 0);
}

//------------------------------------------------------------------------------
// Destroy
//------------------------------------------------------------------------------
void LockGuard::Destroy(LOCK* lock)
{
	DeleteCriticalSection(lock);
}