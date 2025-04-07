#pragma once

namespace UFG
{
	template <typename T, int WHAT = 0>
	class qArray
	{
	public:
		u32 size = 0;
		u32 capacity = 0;
		T* p = 0;

		THEORY_INLINE T& operator[](int i) { return p[i]; }

		THEORY_INLINE T* begin() { return p; }
		THEORY_INLINE T* end() { return &p[size]; }
	};
}