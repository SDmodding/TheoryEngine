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
		char mName[63];
		char mIsResourceData;
	};

	class StateParamLookup
	{
	public:
		u32 mNameUID;
		u32 mParamIndex;

		inline StateParamLookup() : mNameUID(0), mParamIndex(0) {}
	};

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

		bool AddParam(u32 uid, const char* name, bool is_resource_data);

		inline bool AddParam(const char* name, bool is_resource_data)
		{
			return AddParam(UFG::qStringHash32(name), name, is_resource_data);
		}

		void Init();
	};

#ifdef THEORY_IMPL

	StateSystem::StateSystem()
	{
		UFG::qMemSet(mStateParamDescs, 0, sizeof(mStateParamDescs));
		mCurrentParamIndex = 0;
	}

	bool StateSystem::AddParam(u32 uid, const char* name, bool is_resource_data)
	{
		// TODO
		return false;
	}

	void StateSystem::Init()
	{
		AddParam(0, "iUnknown", false);
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