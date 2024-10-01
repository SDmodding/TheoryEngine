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
	};

}