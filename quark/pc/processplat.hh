#pragma once

namespace UFG
{
#ifdef THEORY_IMPL

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

#endif
}