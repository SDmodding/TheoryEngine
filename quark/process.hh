#pragma once

namespace UFG
{
	long qAtomicIncrement(volatile long* v);

	long qAtomicDecrement(volatile long* v);

	long qAtomicAdd(volatile long* v, long add_amount);

	void qSleep(int milliseconds);

	u64 qGetCurrentThreadID();

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

		~qMutex();

		void Lock(const char* name = 0, int unk = 0);

		void Unlock();
	};


	//-------------------------------------------------------------------
	// Thread
	//-------------------------------------------------------------------

	class qThread
	{
	public:
		const char* mName;
		void(*mThreadFunction)(void*);
		void* mThreadParam;
		int mStacksize;
		int mPriority;
		int mLogicalCoreID;
		void* mHandlePlat;
		u64 mHandlePlat64;
		u32 mThreadIDPlat;

		static inline int mDefaultThreadStackSize;
		static inline int mDefaultThreadPriority;

		qThread();

		qThread(const char* name);

		bool SetLogicalCoreID(int logical_core_id);

		void SetName(const char* name);

		bool SetThreadPriority(int priority);

		void Start(void(*thread_function)(void*), void* thread_param);

		void Stop();

		bool WaitForCompletion();
	};
}