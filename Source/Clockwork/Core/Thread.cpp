#include "../Core/Thread.h"

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#include "../DebugNew.h"

namespace Clockwork
{

#ifdef WIN32
DWORD WINAPI ThreadFunctionStatic(void* data)
{
    Thread* thread = static_cast<Thread*>(data);
    thread->ThreadFunction();
    return 0;
}
#else
void* ThreadFunctionStatic(void* data)
{
    Thread* thread = static_cast<Thread*>(data);
    thread->ThreadFunction();
#ifdef EMSCRIPTEN
// note: emscripten doesn't have this function but doesn't use threading anyway
// so #ifdef it out to prevent linker warnings
    pthread_exit((void*)0);
#endif
    return 0;
}
#endif

ThreadID Thread::mainThreadID;

Thread::Thread() :
    handle_(0),
    shouldRun_(false)
{
}

Thread::~Thread()
{
    Stop();
}

bool Thread::Run()
{
    // Check if already running
    if (handle_)
        return false;

    shouldRun_ = true;
    #ifdef WIN32
    handle_ = CreateThread(0, 0, ThreadFunctionStatic, this, 0, 0);
//    #else
    #elif !defined(EMSCRIPTEN)
// note: emscripten doesn't have this function but doesn't use
// threading anyway so #ifdef it out to prevent linker warnings
    handle_ = new pthread_t;
    pthread_attr_t type;
    pthread_attr_init(&type);
    pthread_attr_setdetachstate(&type, PTHREAD_CREATE_JOINABLE);
    pthread_create((pthread_t*)handle_, &type, ThreadFunctionStatic, this);
    #endif
    return handle_ != 0;
}

void Thread::Stop()
{
    // Check if already stopped
    if (!handle_)
        return;

    shouldRun_ = false;
    #ifdef WIN32
    WaitForSingleObject((HANDLE)handle_, INFINITE);
    CloseHandle((HANDLE)handle_);
//    #else
    #elif !defined(EMSCRIPTEN)
 // note: emscripten doesn't have this function but doesn't use
 // threading anyway so #ifdef it out to prevent linker warnings
    pthread_t* thread = (pthread_t*)handle_;
    if (thread)
        pthread_join(*thread, 0);
    delete thread;
    #endif
    handle_ = 0;
}

void Thread::SetPriority(int priority)
{
    #ifdef WIN32
    if (handle_)
        SetThreadPriority((HANDLE)handle_, priority);
    #endif
    #if defined(__linux__) && !defined(ANDROID) && !defined(EMSCRIPTEN)
    pthread_t* thread = (pthread_t*)handle_;
    if (thread)
        pthread_setschedprio(*thread, priority);
    #endif
}

void Thread::SetMainThread()
{
    mainThreadID = GetCurrentThreadID();
}

ThreadID Thread::GetCurrentThreadID()
{
    #ifdef WIN32
    return GetCurrentThreadId();
    #else
    return pthread_self();
    #endif
}

bool Thread::IsMainThread()
{
    return GetCurrentThreadID() == mainThreadID;
}

}