#pragma once

namespace Illusion
{
	class StateBlock : public UFG::qResourceData, public UFG::qNode<StateBlock>
	{
	public:
		struct StateBlockHeader
		{
			u16 dataOffset;
			u16 size;
		};

		u32 mParentUID;
		u32 mDataByteSize;
		u32 mNumValues;
		u32 mNameUID;

		StateBlock(u32 name_uid, const char* name) : UFG::qResourceData(RTypeUID_StateBlock, name_uid, name) {}

		THEORY_INLINE u8* GetData() { 
			return reinterpret_cast<u8*>(reinterpret_cast<uptr>(this) + UFG::qAlignUp<uptr>(sizeof(StateBlock), 16)); 
		}

		THEORY_INLINE StateBlockHeader* GetHeaders() { 
			return reinterpret_cast<StateBlockHeader*>(&GetData()[UFG::qAlignUp(mDataByteSize, 16u)]);
		}
	};

	class StateBlockHandle : public UFG::qTypedResourceHandle<RTypeUID_StateBlock, StateBlock> {};
}