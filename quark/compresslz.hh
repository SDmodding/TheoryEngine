#pragma once

typedef bool(__fastcall* qCompressFlushBufferCallback)(void* param, s64 position, void*  buffer, s64 buffer_size);

namespace UFG
{
	template <int i>
	struct Cache
	{
		enum { SIZE = i };

		u32 mCache[i];
		int mCurrent;

		int operator++(int)
		{
			int cur = mCurrent++;
			mCurrent %= i;
			return cur;
		}

		u32& operator[](int i) { return mCache[i]; }
	};
}

namespace UFG
{
	struct qCompressHeader
	{
		enum HeaderID
		{
			HEADER_ID = 0x51434D50,
			HEADER_ID_SWAPPED = 0x504D4351
		}; 
		
		enum Type
		{
			TYPE_LZ = 0x1
		};

		u32 mID;
		u16 mType;
		u16 mVersion;
		u32 mDataOffset;
		u32 mInPlaceExtraNumBytes;
		s64 mCompressedNumBytes;
		s64 mUncompressedNumBytes;
		s64 mUncompressedChecksum;
		u32 pad0;
		u32 pad1;
		u32 pad2;
		u32 pad3;
		u32 pad4;
		u32 pad5;

		void SwapBytes(u8* bytes, u32 size);

		template <typename T>
		void SwapBytes(T* type) { SwapBytes(reinterpret_cast<u8*>(type), sizeof(type)); }

		void EndianSwap();
	};

	class qContextLZ
	{
	public:
		enum EncodeType
		{
			TYPE_LITERAL,
			TYPE_MATCH,
		};

		enum Version
		{
			VERSION1 = 1,
		};

		Cache<32> mCache;
		char* mInput;
		char* mOutput;
		s64 mInputLength;
		s64 mOutputLength;
		qCompressHeader mHeader;
	};

	class qEncoderContextLZ : qContextLZ
	{
	public:
		__declspec(align(16)) const char* htab2[4096][32];
		char hpos2[4096];
		qCompressFlushBufferCallback mFlushCallback;
		void* mCallbackParam;
		s64 mCompressedLength;
		s64 mOutputPosition;
		s64 mInPlacePosition;
		s64 mInPlacePadding;

		bool Compress();
		void Encode(EncodeType type, const char* ip, int len, u16 dis);
		bool FinalFlush();
	};

	s64 qCompressBufferedLZ(void* input, s64 input_length, void* output, s64 output_length, qCompressFlushBufferCallback flush_callback, void* callback_param);
	s64 qDecompressLZ(void* input, s64 input_length, void* output, s64 output_length = 0);

#ifdef THEORY_IMPL

	void qCompressHeader::SwapBytes(u8* bytes, u32 size)
	{
		if (2 > size) {
			return;
		}

		u8* start = bytes;
		u8* end = &bytes[size - 1];

		while (end > start)
		{
			u8 b0 = *start;
			u8 b1 = *end;
			*start = b1;
			*end = b0;
			++start;
			--end;
		}
	}

	void qCompressHeader::EndianSwap()
	{
		if (mID == HEADER_ID) {
			return;
		}

		SwapBytes(&mID);   
		SwapBytes(&mType);
		SwapBytes(&mVersion);
		SwapBytes(&mDataOffset);
		SwapBytes(&mInPlaceExtraNumBytes);
		SwapBytes(&mCompressedNumBytes);
		SwapBytes(&mUncompressedNumBytes);
		SwapBytes(&mUncompressedChecksum);
		SwapBytes(&pad0);
		SwapBytes(&pad1);
		SwapBytes(&pad2);
		SwapBytes(&pad3);
		SwapBytes(&pad4);
		SwapBytes(&pad5);
	}

	s64 qCompressBufferedLZ(void* input, s64 input_length, void* output, s64 output_length, qCompressFlushBufferCallback flush_callback, void* callback_param)
	{
		// TODO: Implement this...
		return 0;
	}

	s64 qDecompressLZ(void* input, s64 input_length, void* output, s64 output_length)
	{
		auto header = static_cast<qCompressHeader*>(input);
		if (header->mID != qCompressHeader::HEADER_ID_SWAPPED && header->mID != qCompressHeader::HEADER_ID) {
			return -1;
		}

		header->EndianSwap();

		const char* name = "?"; // Seems to be nullptr in 'TextureScriberPC64.exe'.
		qAssertF(input_length >= header->mCompressedNumBytes, "Decompression error - %s", name);
		qAssertF(output_length >= header->mUncompressedNumBytes, "Decompression error - %s", name);

		Cache<32> cache;
		for (auto& c : cache.mCache) {
			c = 0;
		}
		cache.mCurrent = 0;

		u8* out = static_cast<u8*>(output);

		header->EndianSwap();

		u8* in = &static_cast<u8*>(input)[header->mDataOffset];

		header->EndianSwap();

		u8* end = &in[header->mCompressedNumBytes - header->mDataOffset];

		u16 offset, length;

		while (end > in)
		{
			if (out > in && out < end) {
				qDebugBreak();
			}

			u8 bits = *in++;
			if (cache.SIZE > bits)
			{
				for (int i = static_cast<int>(bits) + 1; i; --i) {
					*out++ = *in++;
				}
				continue;
			}

			int index = bits & (cache.SIZE - 1);
			int encode_type = (bits >> 5);
			if (encode_type == qContextLZ::TYPE_MATCH)
			{
				auto offsetLength = reinterpret_cast<u16*>(&cache[index]);
				offset = offsetLength[0];
				length = offsetLength[1];
			}
			else
			{
				offset = static_cast<u16>(*in++) | (index << 8);
				length = (encode_type == 7 ? static_cast<u16>(*in++) : encode_type) + 1;

				auto offsetLength = reinterpret_cast<u16*>(&cache[cache++]);
				offsetLength[0] = offset;
				offsetLength[1] = length;
			}

			u8* dictionary = &out[-offset];
			for (u16 i = 0; length > i; ++i) {
				*out++ = *dictionary++;
			}
		}

		return static_cast<s64>(out - static_cast<u8*>(output));
	}

#endif
}