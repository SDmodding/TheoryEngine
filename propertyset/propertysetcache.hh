#pragma once

namespace UFG::PropertySetCache
{
	void Add(qPropertySetResource* pPropResource);

	bool Contains(const qPropertySetResource* pPropResource);

	bool ContainsSet(const qPropertySet* propSet);

	qPropertySet* GetPropSet(const qSymbol& propSetName);

	qPropertySetResource* GetResource(const qSymbol& propSetName);

	void Remove(qPropertySetResource* pPropResource);

#ifdef THEORY_IMPL

	void Add(qPropertySetResource* pPropResource)
	{
#ifdef THEORY_IMPL_PROPERTYSET_INVENTORY
		gPropertySetInventory.Add(pPropResource);
#endif
	}

	bool Contains(const qPropertySetResource* pPropResource)
	{
#ifdef THEORY_IMPL_PROPERTYSET_INVENTORY
		return gPropertySetInventory.mResourceDatas.Contains(pPropResource);
#else
		return false;
#endif
	}

	bool ContainsSet(const qPropertySet* propSet)
	{
		return Contains(propSet->GetResource());
	}

	qPropertySet* GetPropSet(const qSymbol& propSetName)
	{
		if (auto propResource = GetResource(propSetName)) {
			return propResource->GetPropertySet();
		}

		return 0;
	}

	qPropertySetResource* GetResource(const qSymbol& propSetName)
	{
#ifdef THEORY_IMPL_PROPERTYSET_INVENTORY
		return static_cast<qPropertySetResource*>(gPropertySetInventory.Get(propSetName));
#else
		return 0;
#endif
	}

	void Remove(qPropertySetResource* pPropResource)
	{
#ifdef THEORY_IMPL_PROPERTYSET_INVENTORY
		gPropertySetInventory.Remove(pPropResource);
#endif
	}
#endif
}