#pragma once

namespace UFG
{
#ifdef THEORY_IMPL

	bool qAssertHandlerPlat(const char* condition, const char* file, int line, bool* ignoreAlways, const char* msg, ...)
	{
		char buf[2056];

		va_list va;
		va_start(va, msg);

		qPrintf("\n\n"
			"*********************************\n"
			"********** ASSERT FAIL **********\n"
			"*********************************\n"
			"%s(%d):\n"
			"CONDITION = %s\n",
			file, line,
			condition);

		if (msg) 
		{
			buf[0] = '\0';
			qVSPrintf(buf, 2048, msg, va);
			qPrintf("MESSAGE   = ");
			qPrintf(buf);
			qPrintf("\n\n");
		}

		va_end(va);

		return true;
	}

	Fn_AssertFail qAssertFail = qAssertHandlerPlat;

#endif
}