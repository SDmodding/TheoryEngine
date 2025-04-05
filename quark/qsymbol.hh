#pragma once

namespace UFG
{
	class qSymbol
	{
	public:
		u32 mUID;

		qSymbol() : mUID(-1) {}
		qSymbol(u32 uid) : mUID(uid) {}
		qSymbol(const qSymbol& source) : mUID(source.mUID) {}

		THEORY_INLINE bool is_null() { return mUID == -1; }
		THEORY_INLINE void set_null() { mUID = -1; }

		static qSymbol create_from_string(const char* pszSymbolString);

		qSymbol create_increment(int incrementValue = 1);

		qSymbol create_suffix(const char* suffix);

		bool operator!=(const qSymbol& sym) { return mUID != sym.mUID; }
		bool operator==(const qSymbol& sym) { return mUID == sym.mUID; }
	};

	class qSymbolUC
	{
	public:
		u32 mUID;
	};

	class qSymbolRegistry
	{
	public:
		static const char* Get(u32 uid);
	};

	const char* qSymbolLookupStringFromSymbolTableResources(u32 uid);

#ifdef THEORY_IMPL

	//-------------------------------------------------------------------
	// Symbol
	//-------------------------------------------------------------------

	qSymbol qSymbol::create_from_string(const char* pszSymbolString)
	{
		return { pszSymbolString ? qStringHash32(pszSymbolString) : -1 };
	}

	qSymbol qSymbol::create_increment(int incrementValue)
	{
		return { mUID + incrementValue };
	}

	qSymbol qSymbol::create_suffix(const char* suffix)
	{
		return qStringHash32(suffix, mUID);
	}

	//-------------------------------------------------------------------
	// Symbol Registry
	//-------------------------------------------------------------------

	const char* qSymbolRegistry::Get(u32 uid)
	{
		if (uid == -1) {
			return "";
		}

		const char* str = qSymbolLookupStringFromSymbolTableResources(uid);
		if (!str)
		{
			static int sIndex;
			static char sBuffer[16 * 8];

			char* pBuffer = &sBuffer[16 * (sIndex++ % 8)];
			qSPrintf(pBuffer, "·%08x·", uid);

			str = pBuffer;
		}

		return str;
	}

	//-------------------------------------------------------------------

	const char* qSymbolLookupStringFromSymbolTableResources(u32 uid)
	{
		// TODO: Implement this.
		return 0;
	}

#endif
}