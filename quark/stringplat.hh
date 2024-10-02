#pragma once

namespace UFG
{
	inline char qToLower(char c)
	{
		return (c >= 'A' && c <= 'Z' ? c + ('a' - 'A') : c);
	}

	inline char qToUpper(char c)
	{
		return (c >= 'a' && c <= 'z' ? c - ('a' - 'A') : c);
	}

	inline int qMemCompare(const void* source_a, const void* source_b, size_t count)
	{
		return memcmp(source_a, source_b, count);
	}

	inline void qMemCopy(void* dest, const void* source, size_t count)
	{
		memmove(dest, source, count);
	}

	inline void qMemSet(void* dest, int pattern, size_t count)
	{
		memset(dest, pattern, count);
	}
}