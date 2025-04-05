#pragma once

namespace UFG
{
	void qInit();

	void qClose();

#ifdef THEORY_IMPL

	void qInit()
	{
		InitMemorySystem();

		gQuarkFileSystem.InitPlat();

		qResourceWarehouse::Instance()->Init();
	}

	void qClose()
	{
		qValidateChunkFileBuilders();

		gQuarkFileSystem.ClosePlat();
	}

#endif
}