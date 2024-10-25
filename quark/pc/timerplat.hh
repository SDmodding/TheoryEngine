#pragma once

namespace UFG
{
#ifdef THEORY_IMPL

	u64 qGetSystemTime()
	{
		SYSTEMTIME lt;
		GetLocalTime(&lt);

		u64 st;
		SystemTimeToFileTime(&lt, reinterpret_cast<LPFILETIME>(&st));

		return st;
	}

#endif
}