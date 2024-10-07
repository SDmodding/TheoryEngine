#pragma once

namespace UFG
{
	class qSymbol
	{
	public:
		u32 mUID;

		inline qSymbol() : mUID(-1) {}
		inline qSymbol(u32 uid) : mUID(uid) {}
		inline qSymbol(const qSymbol& source) : mUID(source.mUID) {}

		THEORY_INLINE bool is_null() { return mUID == -1; }
		THEORY_INLINE void set_null() { mUID = -1; }

		static qSymbol create_from_string(const char* pszSymbolString);

		qSymbol create_increment(int incrementValue = 1);

		qSymbol create_suffix(const char* suffix);

		inline bool operator!=(const qSymbol& sym) { return mUID != sym.mUID; }
		inline bool operator==(const qSymbol& sym) { return mUID == sym.mUID; }
	};

	class qSymbolUC
	{
	public:
		u32 mUID;
	};

#ifdef THEORY_IMPL

	qSymbol qSymbol::create_from_string(const char* pszSymbolString)
	{
		return qSymbol(pszSymbolString ? qStringHash32(pszSymbolString) : -1);
	}

	qSymbol qSymbol::create_increment(int incrementValue)
	{
		return qSymbol(mUID + incrementValue);
	}

	qSymbol qSymbol::create_suffix(const char* suffix)
	{
		return qSymbol(qStringHash32(suffix, mUID));
	}

#endif
}