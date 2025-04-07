#pragma once

#define THEORY_STRING_MAGIC		0x1020304

namespace UFG
{
	class qString : public qNode<qString>
	{
	public:
		u32 mMagic;
		int mLength;
		char* mData;
		u32 mStringHash32;
		u32 mStringHashUpper32;

		static inline const char* sEmptyString = "";

		qString() : mMagic(THEORY_STRING_MAGIC), mLength(0), mData(const_cast<char*>(sEmptyString)), mStringHash32(-1), mStringHashUpper32(-1) {}

		qString(const qString& text);

		qString(const char* format, ...);

		~qString()
		{
			Free();
		}

		THEORY_INLINE void SetMagic() { mMagic = THEORY_STRING_MAGIC; }

		THEORY_INLINE void ResetHash() { mStringHash32 = mStringHashUpper32 = -1; }

		void Format(const char* format, ...);

		qString GetFilePath();

		qString GetFilePathWithoutExtension();

		qString GetFilename();

		qString GetFilenameWithoutExtension();

		u32 GetStringHash32();

		u32 GetStringHashUpper32();

		THEORY_INLINE bool IsEmpty() { return mLength == 0; }

		THEORY_INLINE int Length() { return mLength; }

		void MakeLower();

		void MakeUpper();

		void ReplaceCharInPlace(char search_char, char replace_char);

		qString ReplaceExtension(const char* ext);

		void Free();

		void Set(const char* text);

		THEORY_INLINE void SetEmpty() { Set(sEmptyString); }

		void Set(const char* text, int length, const char* textb = 0, int lengthb = 0);

		bool StartsWith(const char* text, int length = -1);
		bool EndsWith(const char* text, int length = -1);

		qString Substring(int start, int length);

		qString ToLower();

		qString ToUpper();

		qString Trim();

		qString* append(const char* str, int len);

		/* Operators */

		operator const char*() { return mData; }

		bool operator!=(const qString& text);
		bool operator!=(const char* text);

		bool operator==(const qString& text);
		bool operator==(const char* text);

		const qString& operator=(const qString& text);
		const qString& operator=(const char* text);

		const qString& operator+=(const qString& text);
		const qString& operator+=(const char* text);

		char operator[](int pos) { return mData[pos]; }
	};

	int qPrintf(const char* fmt, ...);

	int qSPrintf(char* dest, const char* fmt, ...);

	int qSPrintf(char* dest, int max_len, const char* fmt, ...);

	int qVSPrintf(char* dest, int max_len, const char* fmt, char* arg_list);

	THEORY_INLINE bool qStringEmpty(const char* p) { return (!p || !*p); }

	int qStringCompare(const char* text_a, const char* text_b, int count = -1);

	int qStringCompareInsensitive(const char* text_a, const char* text_b, int count = -1);
	
	char* qStringFind(const char* text, const char* find);

	char* qStringFind(const char* text, int text_length, const char* find, int find_length, int insensitive);

	char* qStringFindInsensitive(const char* text, const char* find);

	char* qStringFindLast(const char* text, const char* find);

	char* qStringFindLast(const char* text, char c);

	char* qStringFindLastInsensitive(const char* text, const char* find);

	char* qStringCopy(char* dest, int dest_size, const char* text, int text_count = -1);

	char* qStringCopy(char* dest, const char* text) { return qStringCopy(dest, -1, text, -1); }

	int qStringLength(const char* text);

	void qStringToLower(char* text);

	void qStringToUpper(char* text);

	THEORY_INLINE bool qIsWhitespace(int c)
	{
		return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ' ? true : false);
	}

	qString qTrim(const char* text, int length = 0);

	bool qToBool(const char* text, bool default_value = 0);
}