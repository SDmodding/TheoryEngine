#pragma once

namespace UFG
{
	class SymbolTableEntry
	{
	public:
		u32 mUID;
		qOffset64<char*> mString;
	};

	class SymbolTableResource : public qResourceData
	{
	public:
		u32 mNumEntries;
		qOffset64<SymbolTableEntry*> mEntries;
	};

	inline class SymbolTableResourceInventory* gSymbolTableResourceInventory = 0;

	class SymbolTableResourceInventory : public qResourceInventory
	{
	public:
		bool mbAutoUnpackSymbols;
		SymbolTableResource* mpLastLoadedResource;

		SymbolTableResourceInventory() : qResourceInventory("SymbolTableResourceInventory", RTypeUID_SymbolTableResource, ChunkUID_SymbolTableResource)
		{
			mbAutoUnpackSymbols = 0;
			gSymbolTableResourceInventory = this;
		}

		void Add(qResourceData* data) override;
		void Remove(qResourceData* data) override;

		static SymbolTableResourceInventory* Get()
		{
			if (gSymbolTableResourceInventory) {
				return gSymbolTableResourceInventory;
			}

			return new ("SymbolTblRscInv") SymbolTableResourceInventory;
		}
	};

	class qSymbol
	{
	public:
		u32 mUID;

		qSymbol() {}
		qSymbol(u32 uid) : mUID(uid) {}
		qSymbol(const qSymbol& source) : mUID(source.mUID) {}

		THEORY_INLINE bool is_null() const { return mUID == -1; }
		THEORY_INLINE void set_null() { mUID = -1; }

		static qSymbol create_from_string(const char* pszSymbolString);

		qSymbol create_increment(int incrementValue = 1) const;

		qSymbol create_suffix(const char* suffix) const;

		const char* as_cstr_dbg() const;

		bool operator!=(const qSymbol& sym) const { return mUID != sym.mUID; }
		bool operator==(const qSymbol& sym) const { return mUID == sym.mUID; }

		THEORY_INLINE operator u32() const { return mUID; }
	};

	const inline qSymbol gNullQSymbol;

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

	//----------------------------------------------
	//	Symbol Table Resource Inventory
	//----------------------------------------------

#ifdef THEORY_QSYMBOL_TABLE_INVENTORY
	SymbolTableResourceInventory gSymbolTableResourceInventoryInstance;
#endif

	void SymbolTableResourceInventory::Add(qResourceData* resource_data)
	{
		auto symbolResource = new (resource_data) SymbolTableResource;
		qResourceInventory::Add(resource_data);
		mpLastLoadedResource = symbolResource;
	}

	void SymbolTableResourceInventory::Remove(qResourceData* resource_data)
	{
		qResourceInventory::Remove(resource_data);

		auto symbolResource = static_cast<SymbolTableResource*>(resource_data);
		symbolResource->~SymbolTableResource();

		if (mpLastLoadedResource == symbolResource) {
			mpLastLoadedResource = 0;
		}
	}

	//----------------------------------------------
	//	Symbol
	//----------------------------------------------

	qSymbol qSymbol::create_from_string(const char* pszSymbolString) { return { pszSymbolString ? qStringHash32(pszSymbolString) : -1 }; }

	qSymbol qSymbol::create_increment(int incrementValue) const { return { mUID + incrementValue }; }

	qSymbol qSymbol::create_suffix(const char* suffix) const { return qStringHash32(suffix, mUID); }

	const char* qSymbol::as_cstr_dbg() const { return qSymbolRegistry::Get(mUID); }

	//----------------------------------------------
	//	Symbol Registry
	//----------------------------------------------

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
		auto inventory = SymbolTableResourceInventory::Get();
		for (auto resource_data : inventory->mResourceDatas)
		{
			auto symbolResource = static_cast<SymbolTableResource*>(resource_data);

			auto entries = symbolResource->mEntries.Get();

			int maxIndex = symbolResource->mNumEntries - 1;
			for (int i = 0; maxIndex >= i;)
			{
				int lookupIndex = (maxIndex + i) / 2;
				auto entry = &entries[lookupIndex];
				if (entry->mUID >= uid)
				{
					if (entry->mUID > uid)
					{
						maxIndex = (lookupIndex - 1);
						continue;
					}
					
					if (auto str = entry->mString.Get()) {
						return str;
					}

					break;
				}

				i = (lookupIndex + 1);
			}
		}

		return 0;
	}

#endif
}