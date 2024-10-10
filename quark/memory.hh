#pragma once

namespace UFG
{
	template <typename T>
	THEORY_INLINE T qAlingUp(T n, T align) { return (n + (align - 1)) & ~(align - 1); }
}