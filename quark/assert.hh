#pragma once

namespace UFG
{
    typedef bool(*Fn_AssertFail)(const char*, const char*, int, bool*, const char*, ...);
    extern Fn_AssertFail qAssertFail;
}

#ifdef THEORY_PLATFORM_PC
    #define qAssertF(expression, format, ...) (void)((!!(expression)) || !qAssertFail(#expression, __FILE__, (int)(__LINE__), 0, format, ##__VA_ARGS__) || (__debugbreak(), 0))
#else
    #define qAssertF(expression, format, ...) (void)(0)
#endif

#define qAssert(expression) qAssertF(expression, 0)