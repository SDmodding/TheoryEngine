#pragma once

namespace UFG
{
    typedef bool(*Fn_AssertFail)(const char*, const char*, int, bool*, const char*, ...);
    extern Fn_AssertFail qAssertFail;

    THEORY_INLINE void qSetAssertHandler(Fn_AssertFail fn_assert) { qAssertFail = fn_assert; }
}
#if defined(THEORY_PLATFORM_PC) || !defined(THEORY_DISABLE_ASSERT)
    #define qAssertF(expression, format, ...) (void)((!!(expression)) || !UFG::qAssertFail(#expression, __FILE__, (int)(__LINE__), 0, format, ##__VA_ARGS__) || (__debugbreak(), 0))
#else
    #define qAssertF(expression, format, ...) (void)(0)
#endif

#define qAssert(expression) qAssertF(expression, 0)