#pragma once

namespace Illusion
{
	class StateParam
	{
	public:
		u32 mNameUID;
		u32 mTypeUID;
		s16 mParamIndex;
		s16 pad0;
	};

	class StateParamDesc : public StateParam
	{
	public:
		s8 mName[63];
		s8 mIsResourceData;
	};

	class StateParamLookup
	{
	public:
		u32 mNameUID;
		u32 mParamIndex;

		inline StateParamLookup() : mNameUID(0), mParamIndex(0) {}
	};

	//-------------------------------------------------------------------
	// StateSystem
	//-------------------------------------------------------------------

	class IStateSystem
	{
	public:
	};

	class StateSystem : public IStateSystem
	{
	public:
		StateParamDesc mStateParamDescs[128];
		StateParamLookup mStateParamLookup[128];
		int mCurrentParamIndex;

		StateSystem();

		s16 AddParam(u32 uid, const char* name, bool is_resource_data);

		inline s16 AddParam(StateParam* param, const char* name, bool is_resource_data)
		{
			param->mParamIndex = AddParam(UFG::qStringHash32(name), name, is_resource_data);
			return param->mParamIndex;
		}

		inline bool AddParam(const char* name, bool is_resource_data)
		{
			return AddParam(UFG::qStringHash32(name), name, is_resource_data);
		}

		u64 GetParam(u32 uid);

		inline u64 GetParam(const char* name)
		{
			return GetParam(UFG::qStringHash32(name));
		}

		void Init();
	};

	extern StateSystem gStateSystem;

	THEORY_INLINE StateSystem* GetStateSystem() { return &gStateSystem; }

#ifdef THEORY_IMPL

	//-------------------------------------------------------------------
	// StateSystem
	//-------------------------------------------------------------------

	StateSystem gStateSystem;

	StateSystem::StateSystem()
	{
		UFG::qMemSet(mStateParamDescs, 0, sizeof(mStateParamDescs));
		mCurrentParamIndex = 0;
	}

	s16 StateSystem::AddParam(u32 uid, const char* name, bool is_resource_data)
	{
		/* Check if already defined.
		*  - Should use same loop logic as the 'GetParam', but we don't add params each frame right? */
		for (int i = 0; mCurrentParamIndex > i; ++i)
		{
			auto pDesc = &mStateParamDescs[i];
			if (pDesc->mNameUID == uid)
			{
				if (pDesc->mName[0] == '\0') 
				{
					pDesc->mIsResourceData = is_resource_data;
					UFG::qStringCopy(pDesc->mName, sizeof(pDesc->mName), name);
				}

				return pDesc->mParamIndex;
			}
		}

		if (mCurrentParamIndex == 128)
		{
			UFG::qPrintf("Illusion: Run out of state blocks.  Size=%d\n (tried to add param name '%s')", 128, name);
			return -1;
		}

		auto pDesc = &mStateParamDescs[mCurrentParamIndex];
		pDesc->mNameUID = uid;
		pDesc->mParamIndex = mCurrentParamIndex;
		pDesc->mIsResourceData = is_resource_data;
		UFG::qStringCopy(pDesc->mName, sizeof(pDesc->mName), name);

		int lookupIndex = 0;
		if (mCurrentParamIndex > 0)
		{
			/* Check if desc uid is lower than lookup uid */
			for (auto pLookup = mStateParamLookup; pDesc->mNameUID >= pLookup->mNameUID; ++pLookup)
			{
				++lookupIndex;
				if (lookupIndex >= mCurrentParamIndex) {
					break;
				}
			}

			/* Sort lookup if param index is higher than lookup index */
			if (mCurrentParamIndex > lookupIndex)
			{
				auto pLookup = &mStateParamLookup[mCurrentParamIndex - 1];
				for (int i = (mCurrentParamIndex - lookupIndex); i; --i)
				{
					pLookup[1] = pLookup[0];
					--pLookup;
				}
			}
		}

		auto pLookup = &mStateParamLookup[lookupIndex];
		pLookup->mNameUID = pDesc->mNameUID;
		pLookup->mParamIndex = pDesc->mParamIndex;

		++mCurrentParamIndex;

		return pDesc->mParamIndex;
	}

	u64 StateSystem::GetParam(u32 uid)
	{
		int maxIndex = mCurrentParamIndex;
		if (maxIndex)
		{
			for (int i = 0; maxIndex >= i;)
			{
				int lookupIndex = (maxIndex + i) / 2;
				auto pLookup = &mStateParamLookup[lookupIndex];
				if (pLookup->mNameUID >= uid)
				{
					if (pLookup->mNameUID > uid) 
					{
						maxIndex = (lookupIndex - 1);
						continue;
					}
					
					return pLookup->mParamIndex;
				}

				i = (lookupIndex + 1);
			}
		}

		return 0;
	}

	void StateSystem::Init()
	{
		AddParam(0u, "iUnknown", false);
		AddParam("iShader", true);
		AddParam("iVertexDecl", true);
		AddParam("iIndexBuffer", true);
		AddParam("iVertexBuffer0", true);
		AddParam("iVertexBuffer1", true);
		AddParam("iVertexBuffer2", true);
		AddParam("iVertexBuffer3", true);
		AddParam("iInstanceBuffer", true);
		AddParam("iAlphaState", true);
		AddParam("iRasterState", true);
		AddParam("iTarget", false);
		AddParam("iViewportScissor", false);
	}

#endif
}