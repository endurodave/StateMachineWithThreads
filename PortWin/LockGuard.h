#ifndef _LOCK_GUARD_H
#define _LOCK_GUARD_H

#include "DataTypes.h"

// On Windows, the lock is a critical section
#define LOCK CRITICAL_SECTION

/// @brief A software lock guard that implements RAII (Resourse Acquisition Is Initialization)
/// to control and manage the lock.
class LockGuard
{
public:
	/// Capture a software lock upon construction of LockGuard. 
	/// @param[in] lock - a software lock.
	/// @pre Call Create() to create the lock before using LockGuard constructor. 
	LockGuard(LOCK* lock);

	/// Release a software lock upon destruction of LockGuard. 
	~LockGuard();

	/// Called to create the software lock.
	/// @param[in] lock - a software lock.
	static void Create(LOCK* lock);

	/// Called to destroy the software lock.
	/// @param[in] lock - a software lock.
	static void Destroy(LOCK* lock);

private:
	LOCK* m_lock;
};

#endif 
