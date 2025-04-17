#pragma once

namespace UFG
{
	class BitFlags128
	{
	public:
		u64 mFlags[2];

		THEORY_INLINE void Reset() { mFlags[0] = mFlags[1] = 0ull; }

		bool HasAnyFlag() const { return mFlags[0] || mFlags[1]; }

		THEORY_INLINE bool IsSet(u64 index) const
		{
			if (index >= 64) {
				return mFlags[1] & (1ull << (index - 64ull));
			}

			return mFlags[0] & (1ull << index);
		}

		THEORY_INLINE void Set(u64 index)
		{
			if (index >= 64) {
				mFlags[1] |= (1ull << (index - 64ull));
			}
			else {
				mFlags[0] |= (1ull << index);
			}
		}

		THEORY_INLINE void Remove(u64 index)
		{
			if (index >= 64) {
				mFlags[1] &= ~(1ull << (index - 64ull));
			}
			else {
				mFlags[0] &= ~(1ull << index);
			}
		}
	};
}