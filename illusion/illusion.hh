#pragma once

namespace Illusion
{
	class IEnginePlat
	{
	public:
		void WaitUntilGPUDone();
	};

	class Engine : public IEnginePlat
	{
	public:
		u32 mFrameCount;
		u32 mSafeToDeleteFrameCount;
		//UFG::qMemoryStreamer* MemoryStreamer;
		UFG::qLinearAllocator* FrameMemory;
		UFG::qLinearAllocator* PrevFrameMemory;
		UFG::qLinearAllocator* FrameWriteMemory;
		UFG::qLinearAllocator* PrimitiveFrameWriteMemory;
		UFG::qLinearAllocator* FramePatchMemory;
		UFG::qLinearAllocator* FrameIndexMemory;

		void Init();

		void BeginFrame();

		void EndFrame();

		void RenderThreadProcessDeleteQueue();

		void WaitUntilSafeToDelete();
	};

	extern Engine gEngine;

#ifdef THEORY_IMPL

	Engine gEngine;
	//UFG::qMemoryStreamer gDefaultMemoryStreamer;

	void IEnginePlat::WaitUntilGPUDone()
	{
		// TODO
	}

	void Engine::Init()
	{
		mFrameCount = 3;
		mSafeToDeleteFrameCount = 0;
		//MemoryStreamer = &gDefaultMemoryStreamer;
		FrameMemory = 0;
		PrevFrameMemory = 0;
		FrameWriteMemory = 0;
		PrimitiveFrameWriteMemory = 0;
		FramePatchMemory = 0;
		FrameIndexMemory = 0;

		gStateSystem.Init();
	}

	void Engine::BeginFrame()
	{
		++mFrameCount;
		++mSafeToDeleteFrameCount;

		if (mFrameCount == -1)
		{
			mFrameCount = -1 - mSafeToDeleteFrameCount;
			mSafeToDeleteFrameCount = 0;
		}
	}

	void Engine::EndFrame()
	{
		Factory::ClearFinalDeleteQueueAll();
	}

	void Engine::RenderThreadProcessDeleteQueue()
	{
		Factory::ProcessWaitDeleteQueueAll();
	}

	void Engine::WaitUntilSafeToDelete()
	{
		WaitUntilGPUDone();
	}

#endif
}