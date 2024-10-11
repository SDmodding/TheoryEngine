#pragma once

namespace UFG
{
#ifdef THEORY_IMPL

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