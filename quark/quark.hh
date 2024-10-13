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
	}

	void qClose()
	{
		gQuarkFileSystem.ClosePlat();
	}

#endif
}