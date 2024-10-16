#pragma once

namespace UFG
{
	template <typename T>
	THEORY_INLINE T qAlignUp(T n, T align) { return (n + (align - 1)) & ~(align - 1); }
}