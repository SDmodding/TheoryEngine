#pragma once

namespace UFG
{
	void qInit(qInitParams* init_params)
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
}