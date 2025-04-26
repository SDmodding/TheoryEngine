#pragma once

namespace UFG
{
	class qInitParams
	{
	public:
		int mMemory_NumTransactions = 0;
		int mProfile_NumNodes = 0;
		bool mTool_EnableDebugExecutableCheck = 1;
		const char* mPS3_SysCacheName = 0;
		int mPS3_FileIOThreadStackSize = 0x10000;
		int mPS3_FileIOThreadPriority = -1;
		int mX360_FileIOThreadStackSize = 0x10000;
		int mX360_FileIOThreadPriority = -1;
		int mX360_FileIOThreadLogicalCoreID = 5;
	};

	void qInit(qInitParams* init_params);

	void qClose();
}