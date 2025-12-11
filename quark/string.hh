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

		THEORY_SINLINE const char* sEmptyString = "";

		/* Constructors, Destructor */

		qString() : mMagic(THEORY_STRING_MAGIC), mLength(0), mData(const_cast<char*>(sEmptyString)), mStringHash32(-1), mStringHashUpper32(-1) {}
		qString(const qString& text);
		qString(const char* format, ...);

		~qString() { Free(); }

		THEORY_INLINE void SetMagic() { mMagic = THEORY_STRING_MAGIC; }
		THEORY_INLINE void ResetHash() { mStringHash32 = mStringHashUpper32 = -1; }
		THEORY_INLINE bool IsEmpty() const { return mLength == 0; }
		THEORY_INLINE int Length() const { return mLength; }
		THEORY_INLINE void SetEmpty() { Set(sEmptyString); }

		/* Functions */

		void Format(const char* format, ...);

		qString GetFilePath() const;
		qString GetFilePathWithoutExtension() const;

		qString GetFilename() const;
		qString GetFilenameWithoutExtension() const;

		u32 GetStringHash32();
		u32 GetStringHashUpper32();

		void MakeLower();
		void MakeUpper();

		int ReplaceString(const char* find_text, const char* replace_text, bool ignore_case);
		void ReplaceCharInPlace(char search_char, char replace_char);
		qString ReplaceExtension(const char* ext);

		void Free();

		void Set(const char* text);
		void Set(const char* text, int length, const char* textb = 0, int lengthb = 0);

		bool StartsWith(const char* text, int length = -1) const;
		bool EndsWith(const char* text, int length = -1) const;

		qString Substring(int start, int length) const;

		qString ToLower() const;
		qString ToUpper() const;

		qString Trim() const;

		qString* append(const char* str, int len);

		/* Operators */

		operator const char*() const { return mData; }

		bool operator!=(const qString& text) const;
		bool operator!=(const char* text) const;

		bool operator==(const qString& text) const;
		bool operator==(const char* text) const;

		qString& operator=(const qString& text);
		qString& operator=(const char* text);

		qString& operator+=(const qString& text);
		qString& operator+=(const char* text);

		qString operator+(const char* text)
		{
			auto str = *this;
			str += text;
			return str;
		}

		char operator[](int pos) { return mData[pos]; }
	};

	class qStringBuilder
	{
	public:
		char* mBuffer;
		int mBufferSize;
		int mStringLength;

		qStringBuilder();
		~qStringBuilder();

		void Add(const char* text, int length = -1);

		void Format(const char* format, ...);

		qStringBuilder& operator<<(const qString& v) { Add(v, v.mLength); return *this; }
		qStringBuilder& operator<<(bool v) 
		{
			if (v) {
				Add("true", 4);
			}
			else {
				Add("false", 5);
			}
			return *this;
		}

		qStringBuilder& operator<<(const char* v) { Format("%s", v); return *this; }
		qStringBuilder& operator<<(u64 v) { Format("%u64", v); return *this; }	
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

	THEORY_INLINE char* qStringCopy(char* dest, const char* text) { return qStringCopy(dest, INT_MAX, text, -1); }

	int qStringLength(const char* text);

	void qStringToLower(char* text);

	void qStringToUpper(char* text);

	THEORY_INLINE bool qIsWhitespace(int c)
	{
		return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r' || c == ' ');
	}

	qString qTrim(const char* text, int length = 0);

	bool qToBool(const char* text, bool default_value = 0);
	f32 qToFloat(const char* text, f32 default_value = 0.f);
	int qToInt32(const char* text, int default_value = 0);
	u32 qToUInt32(const char* text, u32 default_value = 0);
}