/* Implementation of functions related to qString or string. */
#pragma once

#ifdef THEORY_IMPL
namespace UFG
{
	class qBufferedPutChar
	{
	public:
		int NumChars;
		char Buffer[256];

		qBufferedPutChar() : NumChars(0) {}
	};

	class qPrintInfo
	{
	public:
		qBufferedPutChar mStdOutBuffer;
		bool StdOut;
		char* OutString;
		int OutStringLen;
		int PrintChannel;
		int NumWritten;
		const char* OriginalFmt;

		qPrintInfo(char* out, int len) : OutString(out), OutStringLen(len), PrintChannel(-1), StdOut(false) {}
	};

	int qPrintEngine(qPrintInfo* info, const char* fmt, char* arg_list)
	{
		return vsnprintf(info->OutString, static_cast<usize>(info->OutStringLen), fmt, arg_list);
	}

	int qPrintf(const char* fmt, ...)
	{
		va_list va;
		va_start(va, fmt);

		int len = vprintf(fmt, va);

		va_end(va);

		return len;
	}

	void qFPrintf(qFile* file, const char* fmt, ...)
	{
		va_list va;
		va_start(va, fmt);

		char buf[2048 + 1];
		qPrintInfo info(buf, 0x4000); /* Don't ask me... */

		qWrite(file, buf, qPrintEngine(&info, fmt, va));

		va_end(va);
	}

	int qSPrintf(char* dest, const char* fmt, ...)
	{
		va_list va;
		va_start(va, fmt);

		qPrintInfo info(dest, INT_MAX);
		int len = qPrintEngine(&info, fmt, va);

		va_end(va);

		return len;
	}

	int qSPrintf(char* dest, int max_len, const char* fmt, ...)
	{
		va_list va;
		va_start(va, fmt);

		qPrintInfo info(dest, max_len);
		int len = qPrintEngine(&info, fmt, va);

		va_end(va);

		return len;
	}

	int qVSPrintf(char* dest, int max_len, const char* fmt, char* arg_list)
	{
		qPrintInfo info(dest, max_len);
		return qPrintEngine(&info, fmt, arg_list);
	}

	int qStringCompare(const char* text_a, const char* text_b, int count)
	{
		if (!text_a || !text_b) {
			return -1;
		}

		if (count < 0) {
			count = -1;
		}

		if (!count) {
			return 0;
		}

		while (*text_a && *text_a == *text_b)
		{
			++text_a;
			++text_b;

			if (!--count) {
				return 0;
			}
		}

		return static_cast<int>(*text_a - *text_b);
	}

	int qStringCompareInsensitive(const char* text_a, const char* text_b, int count)
	{
		if (!text_a || !text_b) {
			return -1;
		}

		if (count < 0) {
			count = -1;
		}

		if (!count) {
			return 0;
		}

		while (*text_a && qToUpper(*text_a) == qToUpper(*text_b))
		{
			++text_a;
			++text_b;

			if (!--count) {
				return 0;
			}
		}

		return static_cast<int>(*text_a - *text_b);
	}

	char* qStringFind(const char* text, const char* find)
	{
		if (qStringEmpty(text) || qStringEmpty(find)) {
			return nullptr;
		}

		int find_len = qStringLength(find);
		int max_len = (qStringLength(text) - find_len);
		if (max_len < 0) {
			return nullptr;
		}

		int index = 0;
		while (qStringCompare(&text[index], find, find_len))
		{
			if (++index > max_len) {
				return 0;
			}
		}

		return const_cast<char*>(&text[index]);
	}

	char* qStringFind(const char* text, int text_length, const char* find, int find_length, int insensitive)
	{
		if (qStringEmpty(text) || !text_length || qStringEmpty(find) || !find_length) {
			return nullptr;
		}

		int max_len = (text_length - find_length);
		if (max_len < 0) {
			return nullptr;
		}

		int index = 0;
		while ((insensitive ? qStringCompareInsensitive(&text[index], find, find_length) : qStringCompare(&text[index], find, find_length)))
		{
			if (++index > max_len) {
				return 0;
			}
		}

		return const_cast<char*>(&text[index]);
	}

	char* qStringFindInsensitive(const char* text, const char* find)
	{
		if (qStringEmpty(text) || qStringEmpty(find)) {
			return nullptr;
		}

		int find_len = qStringLength(find);
		int max_len = (qStringLength(text) - find_len);
		if (max_len < 0) {
			return nullptr;
		}

		int index = 0;
		while (qStringCompareInsensitive(&text[index], find, find_len))
		{
			if (++index > max_len) {
				return 0;
			}
		}

		return const_cast<char*>(&text[index]);
	}

	char* qStringFindLast(const char* text, const char* find)
	{
		if (qStringEmpty(text) || qStringEmpty(find)) {
			return nullptr;
		}

		int find_len = qStringLength(find);
		int max_len = (qStringLength(text) - find_len);
		if (max_len < 0) {
			return nullptr;
		}

		int index = (max_len - 1);
		while (qStringCompare(&text[index], find, find_len))
		{
			if (--index < 0) {
				return 0;
			}
		}

		return const_cast<char*>(&text[index]);
	}

	char* qStringFindLast(const char* text, char c)
	{
		if (qStringEmpty(text) || !c) {
			return nullptr;
		}

		int last_index = -1;
		int index = 0;

		for (const char* p = text; *p; ++p)
		{
			if (*p == c) {
				last_index = index;
			}

			++index;
		}

		if (last_index < 0) {
			return nullptr;
		}

		return const_cast<char*>(&text[last_index]);
	}

	char* qStringFindLastInsensitive(const char* text, const char* find)
	{
		if (qStringEmpty(text) || qStringEmpty(find)) {
			return nullptr;
		}

		int find_len = qStringLength(find);
		int max_len = (qStringLength(text) - find_len);
		if (max_len < 0) {
			return nullptr;
		}

		int index = (max_len - 1);
		while (qStringCompareInsensitive(&text[index], find, find_len))
		{
			if (--index < 0) {
				return 0;
			}
		}

		return const_cast<char*>(&text[index]);
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

	bool qToBool(const char* text, bool default_value)
	{
		if (text && *text)
		{
			char c = qToLower(*text);

			if (c == 'T' || c == '1') {
				return true;
			}
			if (c == 'F' || c == '0') {
				return false;
			}

			/*if (qToInt32(text, 0)) {
				return true;
			}*/
		}

		return default_value;
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

	qString qString::GetFilePath()
	{
		int len = mLength - 1;
		if (len >= 0 && mData[len] == ':') {
			--len;
		}

		while (len >= 0 && mData[len] != '\\' && mData[len] != '/') {
			--len;
		}

		qString res;

		if (len > 0) {
			res.Set(mData, len);
		}
		else {
			res.Set(sEmptyString, 0);
		}

		return res;
	}

	qString qString::GetFilePathWithoutExtension()
	{
		int len = mLength - 1;
		if (len >= 0 && mData[len] == ':') {
			--len;
		}

		while (len >= 0 && mData[len] != '\\' && mData[len] != '/' && mData[len] != '.') {
			--len;
		}

		qString res;

		if (len) {
			res.Set(mData, len);
		}

		return res;
	}

	qString qString::GetFilename()
	{
		int len = mLength;
		while (len >= 0 && mData[len] != '\\' && mData[len] != '/') {
			--len;
		}

		qString res;
		res.Set(&mData[len + 1], mLength - len - 1);

		return res;
	}

	qString qString::GetFilenameWithoutExtension()
	{
		int len = mLength;
		int total_len = mLength;

		while (len >= 0 && mData[len] != '\\' && mData[len] != '/') 
		{
			if (mData[len] == '.') {
				total_len = len;
			}
			--len;
		}

		qString res;
		res.Set(&mData[len + 1], total_len - len - 1);

		return res;
	}

	u32 qString::GetStringHash32()
	{
		if (mStringHash32 == -1) {
			mStringHash32 = qStringHash32(mData);
		}
		return mStringHash32;
	}

	u32 qString::GetStringHashUpper32()
	{
		if (mStringHashUpper32 == -1) {
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

	int qString::ReplaceString(const char* find_text, const char* replace_text, bool ignore_case)
	{
		int find_len = qStringLength(find_text);
		int replace_len = qStringLength(replace_text);

		if (!find_len) {
			return 0;
		}

		char* data = mData;
		int len = mLength;
		char* find = qStringFind(data, len, find_text, find_len, ignore_case);
		if (!find) {
			return 0;
		}

		int numFound = 0;

		qStringBuilder stringBuilder;
		do
		{
			int copy_len = static_cast<int>(find - data);
			stringBuilder.Add(data, copy_len);
			stringBuilder.Add(replace_text, replace_len);

			data = &find[find_len];
			len -= copy_len - find_len;

			++numFound;
			find = qStringFind(data, len, find_text, find_len, ignore_case);

		} while (find);

		Set(stringBuilder.mBuffer, stringBuilder.mStringLength);

		return numFound;
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

	qString qString::ReplaceExtension(const char* ext)
	{
		if (qStringEmpty(ext)) {
			return {};
		}

		if (char* last_dot = qStringFindLast(mData, '.'))
		{
			int ext_length = qStringLength(ext);
			Set(mData, static_cast<int>(last_dot - mData), ext, ext_length);
		}

		return *this;
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

	bool qString::StartsWith(const char* text, int length)
	{
		if (!mData || !text) {
			return false;
		}

		if (length < 0) {
			length = qStringLength(text);
		}

		if (!length) {
			return true;
		}

		char* data = mData;
		while (*data && *data == *text)
		{
			++data;
			++text;
			if (!--length) {
				return true;
			}
		}

		return (*data == *text);
	}

	bool qString::EndsWith(const char* text, int length)
	{
		if (!mData || !text) {
			return false;
		}

		if (length < 0) {
			length = qStringLength(text);
		}

		if (!length) {
			return true;
		}

		char* data = &mData[mLength - length];
		while (*data && *data == *text)
		{
			++data;
			++text;
			if (!--length) {
				return true;
			}
		}

		return (*data == *text);
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

	qString* qString::append(const char* str, int len)
	{
		if (str) {
			Set(mData, mLength, str, len);
		}

		return this;
	}

	/* Operators */

	bool qString::operator!=(const qString& text)
	{
		if (!mLength && !text.mLength) {
			return false;
		}

		if (mLength != text.mLength || !mData || !text.mData) {
			return true;
		}

		return qStringCompare(mData, text.mData);
	}

	bool qString::operator!=(const char* text)
	{
		if (!text) {
			return mLength != 0;
		}

		if (!mData) {
			return true;
		}

		return qStringCompare(mData, text);
	}

	bool qString::operator==(const qString& text)
	{
		if (!mLength && !text.mLength) {
			return true;
		}

		if (mLength != text.mLength || !mData || !text.mData) {
			return false;
		}

		return !qStringCompare(mData, text.mData);
	}

	bool qString::operator==(const char* text)
	{
		if (!text) {
			return mLength == 0;
		}

		if (!mData) {
			return false;
		}

		return !qStringCompare(mData, text);
	}

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

	//----------------------------
	//	String Builder
	//----------------------------

	qStringBuilder::qStringBuilder()
	{
		mBuffer = static_cast<char*>(qMalloc(1, "qStringBuilder.ctor"));
		mBufferSize = 1;
		mStringLength = 0;
	}

	qStringBuilder::~qStringBuilder()
	{
		if (mBuffer) {
			qFree(mBuffer);
		}

		mBuffer = 0;
		mBufferSize = 0;
		mStringLength = 0;
	}

	void qStringBuilder::Add(const char* text, int length)
	{
		if (qStringEmpty(text)) {
			return;
		}

		if (length == -1) {
			length = qStringLength(text);
		}

		int requiredSize = length + mStringLength + 1;
		if (requiredSize > mBufferSize)
		{
			while (requiredSize > mBufferSize) {
				mBufferSize *= 2;
			}

			auto buffer = static_cast<char*>(qMalloc(mBufferSize, "StringBuilder::Add"));
			qMemCopy(buffer, mBuffer, mStringLength + 1);
			qFree(mBuffer);
			mBuffer = buffer;
		}
		
		qMemCopy(&mBuffer[mStringLength], text, length);
		mStringLength += length;
		mBuffer[mStringLength] = 0;
	}

	void qStringBuilder::Format(const char* format, ...)
	{
		va_list va;
		va_start(va, format);

		char text[0x2008];
		qPrintInfo info(text, 0x2000);

		if (int length = qPrintEngine(&info, format, va)) {
			Add(text, length);
		}

		va_end(format);
	}
}
#endif