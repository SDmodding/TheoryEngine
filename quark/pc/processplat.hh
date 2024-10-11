#pragma once

namespace UFG
{
#ifdef THEORY_IMPL

	long qAtomicIncrement(volatile long* v)
	{
		return _InterlockedIncrement(v);
	}

	long qAtomicDecrement(volatile long* v)
	{
		return _InterlockedDecrement(v);
	}

	long qAtomicAdd(volatile long* v, long add_amount)
	{
		_m_prefetchw(v);

		long comperand, exchange;

		do
		{
			comperand = *v;
			exchange = *v + add_amount;
		} while (comperand != _InterlockedCompareExchange(v, exchange, *v));

		return comperand;
	}

	void qSleep(int milliseconds)
	{
		Sleep(static_cast<DWORD>(milliseconds));
	}

	u64 qGetCurrentThreadID()
	{
		return static_cast<u64>(GetCurrentThreadId());
	}

	//-------------------------------------------------------------------
	// Event
	//-------------------------------------------------------------------

	qEvent::qEvent()
	{
		mName = nullptr;
		mHandlePlat = INVALID_HANDLE_VALUE;
	}

	qEvent::~qEvent()
	{
		if (mHandlePlat != INVALID_HANDLE_VALUE) {
			CloseHandle(mHandlePlat);
		}

		mHandlePlat = INVALID_HANDLE_VALUE;
	}

	void qEvent::Clear()
	{
		ResetEvent(mHandlePlat);
	}

	void qEvent::Create(const char* name, bool auto_reset)
	{
		mName = name;
		mAutoReset = auto_reset;
		mHandlePlat = CreateEventA(0, !auto_reset, 0, 0);
	}

	void qEvent::Signal()
	{
		SetEvent(mHandlePlat);
	}

	bool qEvent::Wait(u64 micro_seconds)
	{
		return (WaitForSingleObject(mHandlePlat, static_cast<DWORD>((micro_seconds + 999) / 1000)) == 0);
	}

	bool qEvent::Wait()
	{
		return (WaitForSingleObject(mHandlePlat, INFINITE) == 0);
	}

	//-------------------------------------------------------------------
	// Mutex
	//-------------------------------------------------------------------

	qMutex::qMutex(const char* name)
	{
		InitializeCriticalSectionAndSpinCount(reinterpret_cast<LPCRITICAL_SECTION>(this), 4000);
	}

	void qMutex::Lock(const char* name, int)
	{
		EnterCriticalSection(reinterpret_cast<LPCRITICAL_SECTION>(this));
	}

	void qMutex::Unlock()
	{
		LeaveCriticalSection(reinterpret_cast<LPCRITICAL_SECTION>(this));
	}

	//-------------------------------------------------------------------
	// Thread
	//-------------------------------------------------------------------

	qThread::qThread()
	{
		mName = nullptr;
		mThreadFunction = nullptr;
		mThreadParam = nullptr;
		mStacksize = 0;
		mPriority = -1;
		mLogicalCoreID = -1;
		mHandlePlat = INVALID_HANDLE_VALUE;
		mHandlePlat64 = 0;
		mThreadIDPlat = 0;
	}

	qThread::qThread(const char* name) : qThread()
	{
		mName = name;
	}

	bool qThread::SetLogicalCoreID(int logical_core_id)
	{
		mLogicalCoreID = logical_core_id;
		return (mHandlePlat == INVALID_HANDLE_VALUE 
			|| logical_core_id == -1 
			|| SetThreadIdealProcessor(mHandlePlat, static_cast<DWORD>(logical_core_id)));
	}

	void qThread::SetName(const char* name)
	{
		mName = name;
	}

	bool qThread::SetThreadPriority(int priority)
	{
		mPriority = priority;

		if (priority == -1) {
			priority = mDefaultThreadPriority;
		}

		return (mHandlePlat == INVALID_HANDLE_VALUE || ::SetThreadPriority(mHandlePlat, priority));
	}

	void qThread::Start(void(*thread_function)(void*), void* thread_param)
	{
		mThreadFunction = thread_function;
		mThreadParam = thread_param;

		int stackSize = mStacksize;
		if (stackSize <= 0) {
			stackSize = mDefaultThreadStackSize;
		}

		mHandlePlat = CreateThread(0, static_cast<SIZE_T>(stackSize), reinterpret_cast<LPTHREAD_START_ROUTINE>(mThreadFunction), mThreadParam, 0, reinterpret_cast<LPDWORD>(&mThreadIDPlat));

		int priority = mPriority;
		if (priority == -1) {
			priority = mDefaultThreadPriority;
		}

		if (mHandlePlat != INVALID_HANDLE_VALUE) {
			::SetThreadPriority(mHandlePlat, priority);
		}

		if (mHandlePlat != INVALID_HANDLE_VALUE && mLogicalCoreID != -1) {
			SetThreadIdealProcessor(mHandlePlat, static_cast<DWORD>(mLogicalCoreID));
		}
	}

	void qThread::Stop()
	{
		if (mHandlePlat == INVALID_HANDLE_VALUE) {
			return;
		}

		DWORD dwExitCode;
		GetExitCodeThread(mHandlePlat, &dwExitCode);

		if (dwExitCode == STILL_ACTIVE)
		{
			qPrintf("ERROR: Thread is still active at shutdown!!  Forcefully stopping a thread like this will almost certainly screw the OS!  thread=0x%08x - %s\n", mHandlePlat, mName);
			TerminateThread(mHandlePlat, 0);
		}

		CloseHandle(mHandlePlat);
		mHandlePlat = INVALID_HANDLE_VALUE;
	}

	bool qThread::WaitForCompletion()
	{
		return (mHandlePlat == INVALID_HANDLE_VALUE || WaitForSingleObject(mHandlePlat, INFINITE) == 0);
	}

#endif
}