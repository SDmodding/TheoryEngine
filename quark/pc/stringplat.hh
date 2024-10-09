#pragma once

namespace UFG
{
	THEORY_INLINE char qToLower(char c)
	{
		return (c >= 'A' && c <= 'Z' ? c + ('a' - 'A') : c);
	}

	THEORY_INLINE char qToUpper(char c)
	{
		return (c >= 'a' && c <= 'z' ? c - ('a' - 'A') : c);
	}

	THEORY_INLINE int qMemCompare(const void* source_a, const void* source_b, usize count)
	{
		return memcmp(source_a, source_b, count);
	}

	THEORY_INLINE void qMemCopy(void* dest, const void* source, usize count)
	{
		memcpy(dest, source, count);
	}

	THEORY_INLINE void qMemMove(void* dest, const void* source, usize count)
	{
		memmove(dest, source, count);
	}

	THEORY_INLINE void qMemSet(void* dest, int pattern, usize count)
	{
		memset(dest, pattern, count);
	}
}