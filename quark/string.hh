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

		qString() : mMagic(THEORY_STRING_MAGIC), mLength(0), mData(const_cast<char*>(sEmptyString)), mStringHash32(-1), mStringHashUpper32(-1) {}

		qString(const qString& text);

		qString(const char* format, ...);

		~qString()
		{
			Free();
		}

		void SetMagic() { mMagic = THEORY_STRING_MAGIC; }

		void ResetHash() { mStringHash32 = mStringHashUpper32 = -1; }

		void Format(const char* format, ...);

		qString GetFilePath();

		qString GetFilePathWithoutExtension();

		qString GetFilename();

		qString GetFilenameWithoutExtension();

		u32 GetStringHash32();

		u32 GetStringHashUpper32();

		bool IsEmpty() { return mLength == 0; }

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

		char operator[](int pos) { return mData[pos]; }
	};

	int qPrintf(const char* format, ...);

	int qSPrintf(char* dest, const char* format, ...);

	THEORY_INLINE bool qStringEmpty(const char* p) { return (!p || !*p); }

	char* qStringCopy(char* dest, int dest_size, const char* text, int text_count = -1);

	int qStringLength(const char* text);

	void qStringToLower(char* text);

	void qStringToUpper(char* text);

	inline bool qIsWhitespace(int c)
	{
		return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ' ? true : false);
	}

	qString qTrim(const char* text, int length = 0);
}