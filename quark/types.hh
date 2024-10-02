#pragma once

typedef char				i8;
typedef short				i16;
typedef int					i32;
typedef __int64				i64;

typedef char				s8;
typedef short				s16;
typedef int					s32;
typedef __int64				s64;

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned __int64	u64;

#ifdef _WIN64
typedef unsigned long long	uptr;
#else
typedef unsigned long		uptr;
#endif