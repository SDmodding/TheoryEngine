#pragma once

class BinString
{
public:
	s64 mOffset;

	THEORY_SINLINE const char* sEmptyString = "";

	~BinString() { Release(); }

	void Set(const char* new_text);

	void SetOffset(const char* ptr)
	{
		mOffset = static_cast<s64>(reinterpret_cast<uptr>(ptr) - reinterpret_cast<uptr>(this));
	}

	char* Get()
	{
		return (reinterpret_cast<char*>(this) + (mOffset & ~1));
	}

	void Release()
	{
		if ((mOffset & 1) == 0 || (mOffset & ~1) == 0) {
			return;
		}

		auto str = Get();
		if (str != sEmptyString) {
			UFG::qFree(str);
		}
	}
};

#ifdef THEORY_IMPL

void BinString::Set(const char* new_text)
{
	Release();

	bool dynamic = (mOffset & 1);

	if (UFG::qStringEmpty(new_text)) {
		mOffset = 0;
	}
	else
	{
		int len = UFG::qStringLength(new_text);
		char* str = static_cast<char*>(UFG::qMalloc(len + 1, "BinString"));
		UFG::qStringCopy(str, new_text);
		SetOffset(str);
	}

	if (dynamic) {
		mOffset |= 1;
	}
}

#endif