#pragma once

typedef bool(__fastcall* qCompressFlushBufferCallback)(void* param, s64 position, void*  buffer, s64 buffer_size);

namespace UFG
{
	template <int i>
	struct Cache
	{
		u32 mCache[i];
		int mCurrent;
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

	s64 qCompressBufferedLZ(void* input, s64 input_length, void* output, s64 output_length, qCompressFlushBufferCallback flush_callback, void* callback_param)
	{
		// TODO: Implement this...
		return 0;
	}

	s64 qDecompressLZ(void* input, s64 input_length, void* output, s64 output_length)
	{
		// TODO: Implement this...
		return 0;
	}

#endif
}