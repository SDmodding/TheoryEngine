/* Implementation of functions related to qString or string. */
#pragma once

#ifdef THEORY_IMPL
namespace UFG
{
	int qPrintf(const char* format, ...)
	{
		va_list va;
		va_start(va, format);

		int len = vprintf(format, va);

		va_end(va);

		return len;
	}


	int qSPrintf(char* dest, const char* format, ...)
	{
		va_list va;
		va_start(va, format);

		int len = vsnprintf(dest, 0x7FFFFFFF, format, va);

		va_end(va);

		return len;
	}


	char* qStringCopy(char* dest, int dest_size, const char* text, int text_count)
	{
		if (!dest) {
			return nullptr;
		}

		if (dest_size <= 0) {
			return dest;
		}

		if (!text || !text_count)
		{
			*dest = '\0';
			return dest;
		}

		char* p = dest;
		if (text_count >= 0)
		{
			if (text_count >= dest_size) {
				text_count = (dest_size - 1);
			}

			for (; text_count; --text_count) {
				*p++ = *text++;
			}
		}
		else
		{
			for (; dest_size > 1; --dest_size)
			{
				if (!*text) {
					break;
				}

				*p++ = *text++;
			}
		}

		*p = '\0';

		return dest;
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
}
#endif