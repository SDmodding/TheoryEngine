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

typedef float				f32;
typedef double				f64;

#ifdef _WIN64

typedef unsigned __int64	usize;
typedef unsigned long long	uptr;

#else

typedef unsigned int		usize;
typedef unsigned long		uptr;

#endif

template <typename E, typename T>
class qEnum
{
public:
	T mValue;

	qEnum() {}
	qEnum(E value) : mValue(static_cast<T>(value)) {}
	qEnum(T value) : mValue(value) {}

	THEORY_INLINE operator E() const { return static_cast<E>(mValue); }
	THEORY_INLINE operator T() const { return mValue; }
};

template <typename T>
class qBuffer
{
public:
	THEORY_INLINE operator void*() const { return reinterpret_cast<const void*>(this); }
	THEORY_INLINE operator void*() { return reinterpret_cast<void*>(this); }

private:
	u8 mData[sizeof(T)];
};