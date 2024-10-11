#pragma once

namespace UFG
{
	//-------------------------------------------------------------------
	// Event
	//-------------------------------------------------------------------

	class qEvent
	{
	public:
		const char* mName;
		void* mHandlePlat;
		bool mAutoReset;

		qEvent();

		qEvent(const char* name, bool auto_reset) { Create(name, auto_reset); }

		~qEvent();

		void Clear();

		void Create(const char* name, bool auto_reset);

		void Signal();

		bool Wait(u64 micro_seconds);

		bool Wait();
	};

	//-------------------------------------------------------------------
	// Mutex
	//-------------------------------------------------------------------

	class qMutex
	{
	public:
		u32 mMutexMemory[12];

		qMutex(const char* name = 0);

		void Lock(const char* name = 0, int unk = 0);

		void Unlock();
	};
}