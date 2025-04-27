#pragma once

class ActionID
{
public:
	u32 mUID;

	THEORY_INLINE operator u32() const { return mUID; }
}; 

class ActionPath
{
public:
	BinArray<ActionID, 0> mPath;

	const char* GetString_DBG();
};

#ifdef THEORY_IMPL

const char* ActionPath::GetString_DBG()
{
	if (!mPath.mCount) {
		return 0;
	}

	static u32 sPathIndex;
	static UFG::qString sPathHack[8];

	static const UFG::qSymbol symDot = UFG::qStringHashUpper32(".");
	static const UFG::qSymbol symDotDot = UFG::qStringHashUpper32("..");

	sPathIndex = (sPathIndex + 1) % 8;
	auto& str = sPathHack[sPathIndex];

	auto id = mPath.mData.Get();
	if (*id == symDot || *id == symDotDot) {
		str.Set("\\");
	}
	else {
		str.Set("");
	}

	for (int i = 0; mPath.mCount > i; ++i)
	{
		str += UFG::qSymbolRegistry::Get(*id++);
		if ((mPath.mCount - 1) > i) {
			str += "\\";
		}
	}

	return str;
}

#endif