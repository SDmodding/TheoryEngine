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
		gPropertySetInventory.Add(pPropResource);
	}

	bool Contains(const qPropertySetResource* pPropResource)
	{
		return gPropertySetInventory.mResourceDatas.Contains(pPropResource);
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
		return static_cast<qPropertySetResource*>(gPropertySetInventory.Get(propSetName));
	}

	void Remove(qPropertySetResource* pPropResource)
	{
		gPropertySetInventory.Remove(pPropResource);
	}
#endif
}