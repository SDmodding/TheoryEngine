/*
*	NOTE:
*	- Don't assign constructor or default value for offset due to how engine
*	  handles allocations especially resources around Illusion.
*/
#pragma once

namespace UFG
{
	template <typename T>
	class qOffset32
	{
	public:
		i32 mOffset;

		THEORY_INLINE T Get(i32 offset = 0)
		{
			if (mOffset) {
				return reinterpret_cast<T>(reinterpret_cast<uptr>(this) + mOffset + offset);
			}
			return 0;
		}

		THEORY_INLINE void Set(void* target)
		{
			if (target) {
				mOffset = reinterpret_cast<i32>(reinterpret_cast<uptr>(target) - reinterpret_cast<uptr>(this));
			}
			else {
				mOffset = 0;
			}
		}

		THEORY_INLINE void operator=(const T* target) { Set(target); }
	};

	template <typename T>
	class qOffset64
	{
	public:
		i64 mOffset;

		THEORY_INLINE T Get(i64 offset = 0)
		{
			if (mOffset) {
				return reinterpret_cast<T>(reinterpret_cast<uptr>(this) + mOffset + offset);
			}
			return 0;
		}

		THEORY_INLINE void Set(void* target)
		{
			if (target) {
				mOffset = reinterpret_cast<i64>(reinterpret_cast<uptr>(target) - reinterpret_cast<uptr>(this));
			}
			else {
				mOffset = 0;
			}
		}

		THEORY_INLINE void operator=(const T* target) { Set(target); }

		THEORY_INLINE T operator[](u32 index) { return &Get()[index]; }

		THEORY_INLINE T operator->() { return Get(); }
	};

	template <typename T>
	using qOffset = qOffset64<T>;
}