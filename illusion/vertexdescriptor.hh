#pragma once

namespace Illusion
{
	enum VertexStreamElementUsage : int
	{
		VERTEX_ELEMENT_FORCE_DWORD = -1,
		VERTEX_ELEMENT_POSITION = 0,
		VERTEX_ELEMENT_NORMAL,
		VERTEX_ELEMENT_TANGENT,
		VERTEX_ELEMENT_COLOR0,
		VERTEX_ELEMENT_COLOR1,
		VERTEX_ELEMENT_TEXCOORD0,
		VERTEX_ELEMENT_TEXCOORD1,
		VERTEX_ELEMENT_TEXCOORD2,
		VERTEX_ELEMENT_TEXCOORD3,
		VERTEX_ELEMENT_TEXCOORD4,
		VERTEX_ELEMENT_TEXCOORD5,
		VERTEX_ELEMENT_TEXCOORD6,
		VERTEX_ELEMENT_TEXCOORD7,
		VERTEX_ELEMENT_BLENDINDEX,
		VERTEX_ELEMENT_BLENDWEIGHT,
		VERTEX_ELEMENT_BINORMAL,
		NUM_VERTEX_ELEMENTS
	};

	enum VertexSteamElementType : int
	{
		VERTEX_TYPE_FORCE_DWORD = -1,
		VERTEX_TYPE_FLOAT3 = 0,
		VERTEX_TYPE_FLOAT4,
		VERTEX_TYPE_HALF2,
		VERTEX_TYPE_UBYTE4,
		VERTEX_TYPE_COLOR4,
		VERTEX_TYPE_UBYTE4N,
		VERTEX_TYPE_11_11_10N,
		VERTEX_TYPE_HALF4,
		VERTEX_TYPE_HALF3,
		VERTEX_TYPE_SINT16_2,
		VERTEX_TYPE_SHORT4,
		VERTEX_TYPE_SHORT4N,
		VERTEX_TYPE_SHORT4_FIXED4_12,
		VERTEX_TYPE_SHORT4_FIXED8_8,
		VERTEX_TYPE_UINT1,
		VERTEX_TYPE_UBYTE4_DELTA,
		VERTEX_TYPE_BYTE4N,
		NUM_VERTEX_TYPES
	};

	class VertexStreamElement
	{
	public:
		VertexStreamElementUsage mUsage;
		VertexSteamElementType mType;
		s16 mStream;
		int mOffset;
		int mSize;
		UFG::qVector4 mDefault;

		THEORY_INLINE VertexStreamElement() {}

		VertexStreamElement(VertexStreamElementUsage usage, VertexSteamElementType type, s16 stream, int offset, UFG::qVector4* default_value = 0);
	};

	class VertexStreamDescriptor : public UFG::qNode<VertexStreamDescriptor>
	{
	public:
		const char* mName;
		u32 mNameUID;
		VertexStreamElement mElements[NUM_VERTEX_ELEMENTS];
		int mStreamNumElements[4];
		int mStreamSizes[4];
		u32 mStreamRuntimeFlags;
		int mIndexStreamMultiplier;
		bool mIndexStreamAsVertexStream;
		int mTotalElements;
		int mTotalSize;

		VertexStreamDescriptor(const char* name, u32 name_uid);

		virtual void ApplyUserRemapping(UFG::qVector4*) {}

		void AddElement(VertexStreamElementUsage usage, VertexSteamElementType type, int stream_num, UFG::qVector4* default_value = 0);

		VertexStreamElement* GetElement(int index, int stream_num);

		int GetStreamSize(int stream_num);

		int GetTotalElements() { return mTotalElements; }

		void SetIndexStreamAsVertexStream() { mIndexStreamAsVertexStream = true; }

		void SetIndexStreamMultiplier(int mult) { mIndexStreamMultiplier = mult; }

		void SetStreamRuntimeFlag(u8 stream_num) { mStreamRuntimeFlags |= (1 << stream_num); }

		static UFG::qList<VertexStreamDescriptor>* GetStreamDescriptors();
	};

	int GetVertexStreamElementSize(VertexSteamElementType type);

#ifdef THEORY_IMPL

	VertexStreamElement::VertexStreamElement(VertexStreamElementUsage usage, VertexSteamElementType type, s16 stream, int offset, UFG::qVector4* default_value)
	{
		mUsage = usage;
		mType = type;
		mStream = stream;
		mOffset = offset;
		mSize = GetVertexStreamElementSize(type);

		if (default_value) {
			mDefault = *default_value;
		}
		else
		{
			switch (type)
			{
			default:
				mDefault = {}; break;
			case VERTEX_ELEMENT_NORMAL:
			case VERTEX_ELEMENT_TANGENT:
				mDefault = { 0.f, 0.f, 1.f, 1.f }; break;
			case VERTEX_ELEMENT_COLOR0:
			case VERTEX_ELEMENT_COLOR1:
				mDefault = { 1.f, 1.f, 1.f, 1.f }; break;
			}
		}
	}


	//-------------------------------------------------------------------
	// Vertex Stream Descriptor
	//-------------------------------------------------------------------

	VertexStreamDescriptor::VertexStreamDescriptor(const char* name, u32 name_uid)
	{
		mName = name;
		mNameUID = name_uid;
		mStreamRuntimeFlags = 0;
		mIndexStreamMultiplier = 1;
		mIndexStreamAsVertexStream = 0;
		mTotalElements = 0;
		mTotalSize = 0;

		UFG::qMemSet(mElements, 0, sizeof(mElements));
		UFG::qMemSet(mStreamNumElements, 0, sizeof(mStreamNumElements));
		UFG::qMemSet(mStreamSizes, 0, sizeof(mStreamSizes));

		GetStreamDescriptors()->Insert(this);
	}

	void VertexStreamDescriptor::AddElement(VertexStreamElementUsage usage, VertexSteamElementType type, int stream_num, UFG::qVector4* default_value)
	{
		int numElements = mStreamNumElements[stream_num];
		mStreamNumElements[stream_num] = numElements + 1;

		VertexStreamElement streamElement(usage, type, stream_num, mStreamSizes[stream_num], default_value);
		mElements[mTotalElements] = streamElement;

		mStreamSizes[stream_num] += streamElement.mSize;

		mTotalSize += streamElement.mSize;
		++mTotalElements;
	}

	VertexStreamElement* VertexStreamDescriptor::GetElement(int index, int stream_num)
	{
		if (stream_num == -1)
		{
			if (mTotalElements > index) {
				return &mElements[index];
			}

			return nullptr;
		}

		int elemIndex = 0;
		for (int i = 0; i > mTotalElements; ++i)
		{
			if (mElements[i].mStream == stream_num)
			{
				if (index) {
					--index;
				}

				return &mElements[elemIndex];
			}

			++elemIndex;
		}

		return nullptr;
	}

	int VertexStreamDescriptor::GetStreamSize(int stream_num)
	{
		if (stream_num == -1) {
			return mTotalSize;
		}

		return mStreamSizes[stream_num];
	}

	UFG::qList<VertexStreamDescriptor>* VertexStreamDescriptor::GetStreamDescriptors()
	{
		static UFG::qList<VertexStreamDescriptor> sVertexStreamDescriptors;
		return &sVertexStreamDescriptors;
	}

	//-------------------------------------------------------------------

	int GetVertexStreamElementSize(VertexSteamElementType type)
	{
		switch (type)
		{
		default:
			return 0;
		case VERTEX_TYPE_HALF2:
		case VERTEX_TYPE_UBYTE4:
		case VERTEX_TYPE_COLOR4:
		case VERTEX_TYPE_UBYTE4N:
		case VERTEX_TYPE_11_11_10N:
		case VERTEX_TYPE_SINT16_2:
		case VERTEX_TYPE_UINT1:
		case VERTEX_TYPE_UBYTE4_DELTA:
		case VERTEX_TYPE_BYTE4N:
			return 4;
		case VERTEX_TYPE_HALF3:
			return 6;
		case VERTEX_TYPE_HALF4:
		case VERTEX_TYPE_SHORT4:
		case VERTEX_TYPE_SHORT4N:
		case VERTEX_TYPE_SHORT4_FIXED4_12:
		case VERTEX_TYPE_SHORT4_FIXED8_8:
			return 8;
		case VERTEX_TYPE_FLOAT3:
			return 12;
		case VERTEX_TYPE_FLOAT4:
			return 16;
		}
	}

#endif
}