#pragma once

#define THEORY_STRING_MAGIC		0x1020304

namespace UFG
{
	class qString : public qNode<qString, qString>
	{
	public:
		u32 mMagic;
		int mLength;
		char* mData;
		u32 mStringHash32;
		u32 mStringHashUpper32;

		static inline const char* sEmptyString = "";

		inline qString() : mMagic(THEORY_STRING_MAGIC), mLength(0), mData(const_cast<char*>(sEmptyString)), mStringHash32(-1), mStringHashUpper32(-1) {}

		qString(const qString& text);

		qString(const char* format, ...);

		inline ~qString()
		{
			Free();
		}

		inline void SetMagic() { mMagic = THEORY_STRING_MAGIC; }

		inline void ResetHash() { mStringHash32 = mStringHashUpper32 = -1; }

		void Format(const char* format, ...);

		qString GetFilePath();

		qString GetFilePathWithoutExtension();

		qString GetFilename();

		qString GetFilenameWithoutExtension();

		u32 GetStringHash32();

		u32 GetStringHashUpper32();

		inline bool IsEmpty() { return mLength == 0; }

		void MakeLower();

		void MakeUpper();

		void ReplaceCharInPlace(char search_char, char replace_char);

		qString ReplaceExtension(const char* ext);

		void Free();

		void Set(const char* text);

		void Set(const char* text, int length, const char* textb = 0, int lengthb = 0);

		bool StartsWith(const char* text, int length);
		bool EndsWith(const char* text, int length);

		qString Substring(int start, int length);

		qString ToLower();

		qString ToUpper();

		qString Trim();

		const qString& append(const char* str, int len);

		/* Operators */

		bool operator!=(const qString& text);
		bool operator!=(const char* text);

		const qString& operator=(const qString& text);
		const qString& operator=(const char* text);

		const qString& operator+=(const qString& text);
		const qString& operator+=(const char* text);

		bool operator==(const qString& text);
		bool operator==(const char* text);

		inline char operator[](int pos) { return mData[pos]; }
	};

	int qPrintf(const char* format, ...);

	int qStringLength(const char* text);

	void qStringToLower(char* text);

	void qStringToUpper(char* text);

	_inline bool qIsWhitespace(int c)
	{
		return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ' ? true : false);
	}

	qString qTrim(const char* text, int length = 0);

#ifdef THEORY_IMPL

	int qPrintf(const char* format, ...)
	{
		va_list va;
		va_start(va, format);

		int len = vprintf(format, va);

		va_end(va);

		return len;
	}

	int qStringLength(const char* text)
	{
		const char* p = text;
		while (*p++) {}

		return static_cast<int>(p - text) - 1;
	}

	void qStringToLower(char* text)
	{
		if (text) 
		{
			for (char* p = text; *p; ++p) {
				*p = qToLower(*p);
			}
		}
	}

	void qStringToUpper(char* text)
	{
		if (text)
		{
			for (char* p = text; *p; ++p) {
				*p = qToUpper(*p);
			}
		}
	}

	qString qTrim(const char* text, int length)
	{
		qString res;

		if (text)
		{
			if (length < 0) {
				length = qStringLength(text);
			}

			int start = 0;
			int end = length;

			/* Trim start */

			for (int i = 0; length > i; ++i)
			{
				if (!qIsWhitespace(text[i])) {
					break;
				}

				++start;
			}

			/* Trim end */

			for (int i = 0; length > i; ++i)
			{
				if (!qIsWhitespace(text[end - 1])) {
					break;
				}

				--end;
			}

			int len = (end - start);
			if (len) {
				res.Set(&text[start], len);
			}
		}

		return res;
	}

	//-------------------------------------------------------------------
	// [Class] qString
	//-------------------------------------------------------------------

	qString::qString(const qString& text) : qString()
	{
		Set(text.mData, text.mLength, 0, 0);
		mStringHash32 = text.mStringHash32;
		mStringHashUpper32 = text.mStringHashUpper32;
	}

	qString::qString(const char* format, ...) : qString()
	{
		char buf[8192];
		va_list va;
		va_start(va, format);

		int len = vsnprintf(buf, sizeof(buf), format, va);
		if (len) {
			Set(buf, len);
		}

		va_end(va);
	}

	void qString::Format(const char* format, ...)
	{
		char buf[8192];
		va_list va;
		va_start(va, format);

		int len = vsnprintf(buf, sizeof(buf), format, va);
		if (len) {
			Set(buf, len);
		}

		va_end(va);
	}

	u32 qString::GetStringHash32()
	{
		if (!mStringHash32) {
			mStringHash32 = qStringHash32(mData);
		}
		return mStringHash32;
	}

	u32 qString::GetStringHashUpper32()
	{
		if (!mStringHashUpper32) {
			mStringHashUpper32 = qStringHashUpper32(mData);
		}
		return mStringHashUpper32;
	}

	void qString::MakeLower()
	{
		qStringToLower(mData);
		ResetHash();
	}

	void qString::MakeUpper()
	{
		qStringToUpper(mData);
		ResetHash();
	}

	void qString::ReplaceCharInPlace(char search_char, char replace_char)
	{
		for (int i = 0; mLength > i; ++i)
		{
			if (mData[i] == search_char) {
				mData[i] = replace_char;
			}
		}
	}

	void qString::Free()
	{
		if (mData != sEmptyString && mData) {
			GetMainMemoryPool()->Free(mData);
		}
	}

	void qString::Set(const char* text)
	{
		int len = 0;
		if (text && text != sEmptyString) {
			len = qStringLength(text);
		}
		Set(text, len);
	}

	void qString::Set(const char* text, int length, const char* textb, int lengthb)
	{
		SetMagic();

		char* pData = const_cast<char*>(sEmptyString);
		int nLen = 0;

		if (text && *text) {
			nLen += length;
		}

		if (textb && *textb) {
			nLen += lengthb;
		}

		if (nLen)
		{
			pData = reinterpret_cast<char*>(GetMainMemoryPool()->Allocate(static_cast<usize>(nLen + 1), "qString.Set", 0, true));
			if (pData)
			{
				if (length) {
					qMemCopy(pData, text, static_cast<usize>(length));
				}
				if (lengthb) {
					qMemCopy(&pData[length], textb, static_cast<usize>(lengthb));
				}

				pData[nLen] = '\0';
			}
		}

		Free();
		mData = pData;
		mLength = nLen;
		ResetHash();
	}

	qString qString::Substring(int start, int length)
	{
		qString res;

		if (mLength > start && start >= 0)
		{
			if (length < 0 || start + length >= mLength) {
				length = mLength - start;
			}

			res.Set(&mData[start], length);
		}

		return res;
	}

	qString qString::ToLower()
	{
		qString res(*this);
		qStringToLower(res.mData);
		return res;
	}

	qString qString::ToUpper()
	{
		qString res(*this);
		qStringToUpper(res.mData);
		return res;
	}

	qString qString::Trim()
	{
		return qTrim(mData, mLength);
	}

	/* Operators */

	const qString& qString::operator=(const qString& text)
	{
		Set(text.mData, text.mLength);
		return *this;
	}

	const qString& qString::operator=(const char* text)
	{
		Set(text);
		return *this;
	}

	const qString& qString::operator+=(const qString& text)
	{
		if (text.mLength) {
			Set(mData, mLength, text.mData, text.mLength);
		}
		return *this;
	}

	const qString& qString::operator+=(const char* text)
	{
		if (text) {
			Set(mData, mLength, text, qStringLength(text));
		}
		return *this;
	}

#endif
}