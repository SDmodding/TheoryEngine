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
			mCurrent %= SIZE;
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

        u32 GetInPlaceOffset() const { return static_cast<u32>((mUncompressedNumBytes + mInPlaceExtraNumBytes) - mCompressedNumBytes); }
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

	class qEncoderContextLZ : public qContextLZ
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
		void Encode(EncodeType type, const char* ip, int len, int dis);
		bool FinalFlush();
	};

	s64 qCompressBufferedLZ(void* input, s64 input_length, void* output, s64 output_length, qCompressFlushBufferCallback flush_callback = 0, void* callback_param = 0);
	s64 qDecompressLZ(void* input, s64 input_length, void* output, s64 output_length = 0);

#ifdef THEORY_IMPL

	void qCompressHeader::EndianSwap()
	{
		if (mID == HEADER_ID) {
			return;
		}

        qEndianSwapBytes(reinterpret_cast<u8*>(&mID), sizeof(mID));
        qEndianSwapBytes(reinterpret_cast<u8*>(&mType), sizeof(mType));
        qEndianSwapBytes(reinterpret_cast<u8*>(&mVersion), sizeof(mVersion));
        qEndianSwapBytes(reinterpret_cast<u8*>(&mDataOffset), sizeof(mDataOffset));
        qEndianSwapBytes(reinterpret_cast<u8*>(&mInPlaceExtraNumBytes), sizeof(mInPlaceExtraNumBytes));
        qEndianSwapBytes(reinterpret_cast<u8*>(&mCompressedNumBytes), sizeof(mCompressedNumBytes));
        qEndianSwapBytes(reinterpret_cast<u8*>(&mUncompressedNumBytes), sizeof(mUncompressedNumBytes));
        qEndianSwapBytes(reinterpret_cast<u8*>(&mUncompressedChecksum), sizeof(mUncompressedChecksum));
        qEndianSwapBytes(reinterpret_cast<u8*>(&pad0), sizeof(pad0));
        qEndianSwapBytes(reinterpret_cast<u8*>(&pad1), sizeof(pad1));
        qEndianSwapBytes(reinterpret_cast<u8*>(&pad2), sizeof(pad2));
        qEndianSwapBytes(reinterpret_cast<u8*>(&pad3), sizeof(pad3));
        qEndianSwapBytes(reinterpret_cast<u8*>(&pad4), sizeof(pad4));
        qEndianSwapBytes(reinterpret_cast<u8*>(&pad5), sizeof(pad5));
	}

    bool qEncoderContextLZ::Compress()
    {
        auto WriteLiterals = [this](const char* src, int len)
        {
            if (0 >= len) {
                return;
            }

            char* out = mOutput + mOutputPosition;
            *out++ = static_cast<u8>(len - 1);
            memcpy(out, src, len);
            out += len;

            s64 written = out - (mOutput + mOutputPosition);
            mOutputPosition += written;
            mCompressedLength += written;

            // in-place
            s64 dist = (src + len - mInput) - (mCompressedLength - sizeof(qCompressHeader));
            mInPlacePosition = mCompressedLength;
            if (dist + 1 > mInPlacePadding) {
                mInPlacePadding = static_cast<int>(dist + 1);
            }
        };

        if (mInputLength == 0) {
            return false;
        }

        qMemSet(&mHeader, 0, sizeof(mHeader));
        mHeader.mID = qCompressHeader::HEADER_ID;
        mHeader.mType = qCompressHeader::TYPE_LZ;
        mHeader.mVersion = VERSION1;
        mHeader.mDataOffset = sizeof(qCompressHeader);
        mHeader.mUncompressedNumBytes = mInputLength;
        mHeader.mUncompressedChecksum = qDataHash64(mInput, mInputLength);

        // reserve space for the header
        mOutputPosition = sizeof(qCompressHeader);
        mCompressedLength = sizeof(qCompressHeader);

        const char* inputEnd = mInput + mInputLength;
        const char* safeEnd = inputEnd - 16;
        const char* almostEnd = inputEnd - 2;

        // trivial case
        if (4 > mInputLength)
        {
            WriteLiterals(mInput, static_cast<int>(mInputLength));
            return FinalFlush();
        }

        // init hash table - every entry points to start of input
        __stosq(reinterpret_cast<u64*>(htab2), reinterpret_cast<u64>(mInput), sizeof(htab2) / 8);
        qMemSet(hpos2, 0, sizeof(hpos2));

        int litLen = 2;
        const char* litStart = mInput;
        const u8* ip = reinterpret_cast<const u8*>(mInput + 2);

        while (reinterpret_cast<const u8*>(safeEnd) > ip)
        {
            // streaming flush if needed
            if (mFlushCallback && mOutputPosition && mOutputPosition + 36 >= mOutputLength)
            {
                if (!mFlushCallback(mCallbackParam, mCompressedLength - mOutputPosition, mOutput, mOutputPosition)) {
                    return 0;
                }

                mOutputPosition = 0;
            }

            // ----- special case: long run of identical bytes -----
            u8 prev = ip[-1];
            if (prev == ip[0] && prev == ip[1] && prev == ip[2])
            {
                const u8* runEnd = reinterpret_cast<const u8*>(almostEnd);
                if (ip + 252 < runEnd)
                    runEnd = ip + 252;

                const char* matchStart = reinterpret_cast<const char*>(ip);
                while (ip < runEnd && *ip == prev)
                    ++ip;

                if (litLen > 0)
                {
                    WriteLiterals(litStart, litLen);
                    litLen = 0;
                }

                Encode(TYPE_MATCH, matchStart, static_cast<int>(ip - reinterpret_cast<const u8*>(matchStart)), 1);

                litStart = reinterpret_cast<const char*>(ip);
                continue;
            }

            // ----- normal match search -----
            u8 b0 = ip[0], b1 = ip[1], b2 = ip[2];

            u32 h = static_cast<u32>(-63) * (b0 | ((b1 | (b2 << 8)) << 8));
            h = static_cast<u32>(-7) * ((static_cast<u32>(-8176) * ((h >> 17) ^ h)) ^
                (static_cast<u32>(-511) * ((h >> 17) ^ h)));
            h = ((static_cast<u16>(h << 10) ^ static_cast<u16>(h)) ^
                (static_cast<u16>((h << 10) ^ h) >> 15)) & 0xFFF;

            const char** chain = htab2[h];
            int bestLen = 0;
            u16 bestOff = 0;

            const u8* limit = reinterpret_cast<const u8*>(almostEnd);
            if (limit > ip + 252) {
                limit = ip + 252;
            }

            for (int i = 0; i < 32; ++i)
            {
                const u8* ref = reinterpret_cast<const u8*>(chain[i]);
                u32 dist = static_cast<u32>(ip - ref);
                if (dist == 0 || dist - 1 > 0x1FFD) {
                    continue;
                }

                int matchLen = 0;
                if (ref[0] == b0)
                {
                    if (ref[1] == b1)
                    {
                        if (ref[2] == b2)
                        {
                            if (ref[3] == ip[3])
                            {
                                matchLen = 4;
                                const u8* a = ref + 4;
                                const u8* b = ip + 4;
                                while (b < limit && *a == *b) {
                                    ++a; ++b; ++matchLen;
                                }
                            }
                            else {
                                matchLen = 3;
                            }
                        }
                        else {
                            matchLen = 2;
                        }
                    }
                    else {
                        matchLen = 1;
                    }
                }

                if (matchLen > bestLen)
                {
                    bestLen = matchLen;
                    bestOff = static_cast<u16>(dist);
                }
            }

            // insert current position (newest at front)
            for (int i = 30; i >= 0; --i) {
                chain[i + 1] = chain[i];
            }

            chain[0] = reinterpret_cast<const char*>(ip);

            if (3 > bestLen)
            {
                ++ip;
                if (++litLen == 32)
                {
                    WriteLiterals(litStart, 32);
                    litStart = reinterpret_cast<const char*>(ip);
                    litLen = 0;
                }
            }
            else
            {
                if (litLen > 0)
                {
                    WriteLiterals(litStart, litLen);
                    litLen = 0;
                }

                Encode(TYPE_MATCH, reinterpret_cast<const char*>(ip), bestLen, bestOff);
                ip += bestLen;
                litStart = reinterpret_cast<const char*>(ip);
            }
        }

        // ----- tail -----
        int remaining = static_cast<int>(inputEnd - reinterpret_cast<const char*>(ip));
        int totalLit = remaining + litLen;

        if (totalLit > 0)
        {
            // we can emit it in one or two runs; one is fine
            if (litLen > 0) {
                WriteLiterals(litStart, litLen);
            }
            if (remaining > 0) {
                WriteLiterals(reinterpret_cast<const char*>(ip), remaining);
            }
        }

        return FinalFlush();
    }

    void qEncoderContextLZ::Encode(EncodeType type, const char* ip, int len, int dis)
    {
        char* out = mOutput + mOutputPosition;
        char* start = out;

        if (type != TYPE_LITERAL)
        {
            // Cache key is only the low 16 bits of dis + length in the high word
            u32 packed = (static_cast<u16>(dis)) | (static_cast<u32>(len) << 16);

            int cacheIdx = -1;
            for (int i = 0; mCache.SIZE > i; ++i)
            {
                if (mCache.mCache[i] == packed)
                {
                    cacheIdx = i;
                    break;
                }
            }

            int lenMinus1 = len - 1;

            if (0 > cacheIdx) // not in cache -> emit full match and store it
            {
                if (7 > lenMinus1)
                {
                    // control byte = (len-1)<<5 | high byte of dis
                    *out++ = static_cast<u8>((lenMinus1 << 5) | ((dis >> 8) & 0xFF));
                    *out++ = static_cast<u8>(dis); // low byte
                }
                else
                {
                    // control byte = 0xE0 | high byte of dis
                    *out++ = static_cast<u8>(0xE0 | ((dis >> 8) & 0xFF));
                    *out++ = static_cast<u8>(dis); // low byte
                    *out++ = static_cast<u8>(lenMinus1); // extra length byte
                }

                mCache[mCache++] = packed;
            }
            else { // cache hit -> single-byte command
                *out++ = static_cast<u8>(0x20 | cacheIdx);
            }
        }
        else
        {
            *out++ = static_cast<u8>(len - 1);
            for (int i = 0; len > i; ++i) {
                *out++ = *ip++;
            }
        }

        int inplace = static_cast<int>(reinterpret_cast<uptr>(ip)) - static_cast<int>(reinterpret_cast<uptr>(mInput)) - static_cast<int>(mCompressedLength);

        mInPlacePosition = mCompressedLength;
        if (inplace + 1 > mInPlacePadding) {
            mInPlacePadding = inplace + 1;
        }

        mOutputPosition += out - start;
        mCompressedLength += (out - start);
    }

    bool qEncoderContextLZ::FinalFlush()
    {
        // flush remaining data
        if (mFlushCallback && mOutputPosition > 0)
        {
            if (!mFlushCallback(mCallbackParam, mCompressedLength - mOutputPosition, mOutput, mOutputPosition)) {
                return false;
            }

            mOutputPosition = 0;
        }

        // final header values
        mHeader.mCompressedNumBytes = mCompressedLength;

        if (mCompressedLength > mInputLength) {
            mHeader.mInPlaceExtraNumBytes += static_cast<u32>(mCompressedLength - mInputLength);
        }

        const s64 inplaceEnd = mInPlacePosition + mInPlacePadding;
        if (inplaceEnd > mInputLength) {
            mHeader.mInPlaceExtraNumBytes += static_cast<u32>(inplaceEnd - mInputLength);
        }

        mHeader.EndianSwap();

        // 8-byte alignment

        u32 unaligned = mHeader.mInPlaceExtraNumBytes + static_cast<u32>(mHeader.mUncompressedNumBytes)  - static_cast<u32>(mHeader.mCompressedNumBytes);
        mHeader.mInPlaceExtraNumBytes += qAlignUp(unaligned, 8u) - unaligned;

        if (!mFlushCallback)
        {
            // non-streaming: write header at the beginning of the buffer
            qMemCopy(mOutput, &mHeader, sizeof(qCompressHeader));
            return true;
        }

        // streaming: send header as last block
        return mFlushCallback(mCallbackParam, 0, &mHeader, sizeof(qCompressHeader));
    }

    s64 qCompressBufferedLZ(void* input, s64 input_length, void* output, s64 output_length, qCompressFlushBufferCallback flush_callback, void* callback_param)
    {
        if (!input || 0 >= input_length) {
            return -1;
        }

        if (flush_callback && 128 > output_length) {
            return -1;
        }

        // Original uses stack allocation, but that could cause a stack overflow.
        qEncoderContextLZ* ctx = new ("qEncoderContextLZ") qEncoderContextLZ;
        if (!ctx) {
            return -1;
        }

        qMemSet(ctx, 0, sizeof(*ctx));

        ctx->mInput = static_cast<char*>(input);
        ctx->mInputLength = input_length;
        ctx->mOutput = static_cast<char*>(output);
        ctx->mOutputLength = output_length;
        ctx->mFlushCallback = flush_callback;
        ctx->mCallbackParam = callback_param;
        ctx->mCompressedLength = 0;
        ctx->mOutputPosition = 0;
        ctx->mInPlacePosition = 0;
        ctx->mInPlacePadding = 0;
        ctx->mCache.mCurrent = 0;

        s64 result = ctx->Compress() ? ctx->mHeader.mCompressedNumBytes : -1;
        qFree(ctx);

        return result;
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