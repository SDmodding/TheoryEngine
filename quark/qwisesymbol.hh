#pragma once

namespace UFG
{
	u32 qWiseSymbolUIDFromString(const char* str, u32 prevHash = 0x811C9DC5);

	class qWiseSymbol
	{
	public:
		u32 mUID;

		qWiseSymbol() {}
		qWiseSymbol(u32 uid) : mUID(uid) {}
		qWiseSymbol(const qWiseSymbol& source) : mUID(source.mUID) {}

		static qWiseSymbol create_from_string(const char* pszSymbolString) { return qWiseSymbolUIDFromString(pszSymbolString); }

		THEORY_INLINE operator u32() const { return mUID; }
	};

#ifdef THEORY_IMPL

	u32 qWiseSymbolUIDFromString(const char* str, u32 prevHash)
	{
		// This is slightly modified version as original is using char array in stack with size 1032.
		// Modify this if the size is for some reason really needed.

		while (*str)
		{
			char c = qToLower(*str++);
			prevHash = static_cast<u8>(c) ^ (0x1000193 * prevHash);
		}

		return prevHash;
	}

#endif
}