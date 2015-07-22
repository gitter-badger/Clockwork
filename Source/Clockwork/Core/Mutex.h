

#pragma once

#ifdef CLOCKWORK_IS_BUILDING
#include "Clockwork.h"
#else
#include <Clockwork/Clockwork.h>
#endif

namespace Clockwork
{

/// Operating system mutual exclusion primitive.
class CLOCKWORK_API Mutex
{
public:
    /// Construct.
    Mutex();
    /// Destruct.
    ~Mutex();

    /// Acquire the mutex. Block if already acquired.
    void Acquire();
    /// Release the mutex.
    void Release();

private:
    /// Mutex handle.
    void* handle_;
};

/// Lock that automatically acquires and releases a mutex.
class CLOCKWORK_API MutexLock
{
public:
    /// Construct and acquire the mutex.
    MutexLock(Mutex& mutex);
    /// Destruct. Release the mutex.
    ~MutexLock();

private:
    /// Prevent copy construction.
    MutexLock(const MutexLock& rhs);
    /// Prevent assignment.
    MutexLock& operator =(const MutexLock& rhs);

    /// Mutex reference.
    Mutex& mutex_;
};

}
