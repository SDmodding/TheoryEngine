#pragma once

namespace UFG
{
	template <typename T>
	class qOffset32
	{
	public:
		i32 mOffset;

		inline T Get(i32 offset = 0)
		{
			return reinterpret_cast<T>(reinterpret_cast<uptr>(this) + mOffset + offset);
		}

		inline void Set(void* target)
		{
			if (target) {
				mOffset = reinterpret_cast<i32>(reinterpret_cast<uptr>(target) - reinterpret_cast<uptr>(this));
			}
			else {
				mOffset = 0;
			}
		}

		inline void operator=(const T* target) { Set(target); }
	};

	template <typename T>
	class qOffset64
	{
	public:
		i64 mOffset;

		inline T Get(i64 offset = 0)
		{
			return reinterpret_cast<T>(reinterpret_cast<uptr>(this) + mOffset + offset);
		}

		inline void Set(void* target)
		{
			if (target) {
				mOffset = reinterpret_cast<i64>(reinterpret_cast<uptr>(target) - reinterpret_cast<uptr>(this));
			}
			else {
				mOffset = 0;
			}
		}

		inline void operator=(const T* target) { Set(target); }
	};
}