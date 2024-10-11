#pragma once

namespace UFG
{
	class qMutex
	{
	public:
		u32 mMutexMemory[12];

		qMutex(const char* name = 0);

		void Lock(const char* name = 0, int unk = 0);

		void Unlock();
	};

}