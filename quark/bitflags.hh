#pragma once

namespace UFG
{
	class BitFlags128
	{
	public:
		u64 mFlags[2];

		THEORY_INLINE bool IsSet(int index)
		{
			if (index >= 64) {
				return (mFlags[1] & (1ull << static_cast<u64>(index - 64ull)));
			}

			return (mFlags[0] & (1ull << static_cast<u64>(index)));
		}

		THEORY_INLINE void Set(int index)
		{
			if (index >= 64) {
				mFlags[1] |= (1ull << static_cast<u64>(index - 64ull));
			}
			else {
				mFlags[0] |= (1ull << static_cast<u64>(index));
			}
		}

		THEORY_INLINE void Remove(int index)
		{
			if (index >= 64) {
				mFlags[1] &= ~(1ull << static_cast<u64>(index - 64ull));
			}
			else {
				mFlags[0] &= ~(1ull << static_cast<u64>(index));
			}
		}
	};
}