#pragma once

/*
*	NOTE:
*	- All ifdefs with THEORY_WINDOWS are custom implementation and wrappers!
*/

#define MemoryPool_UID				0x34566543

namespace UFG
{
	class qMemoryPoolData
	{
	public:
		//allocator mAllocator;
	};

	class qMemoryPool : public qNode<qMemoryPool, qMemoryPool>
	{
	public:
		u32 mInitializedUID;
		void* mAutoAllocatedBuffer;
		qMemoryPoolData* mData;
		s8 mDataBuffer[sizeof(qMemoryPoolData)];
		u32 mFlags;
		bool mUsePageBasedStompFinder;
		char* mStart;
		char* mEnd;
		qMemoryPool* mOverflowPool;
		int mOverflowOccurred;
		int mPrintWarningOnOverflow;
		int mAmountOfSpilledMemory;
		int mMaxAmountOfSpilledMemory;
		int mNumActiveSpilledAllocs;
		int mTotalSpilledAllocs;
		// TODO: This will be just wrapped for now.

#ifdef THEORY_WINDOWS
		HANDLE mHeap;
#endif
		void Init(const char* name, usize memory_byte_size, int small_block_byte_size, int can_small_block_overflow_into_large_block = 1, u32 InStatList = 1, qMemoryPool* overflow_pool = 0, int printWarningOnOverflow = 1, bool bInitializeAllocator = true);

		void Init(const char* name, char* memory, usize memory_byte_size, int small_block_byte_size, int can_small_block_overflow_into_large_block = 1, u32 InStatList = 1, qMemoryPool* overflow_pool = 0, int printWarningOnOverflow = 1, bool bInitializeAllocator = true);

		void* Allocate(usize size, const char* name, u64 allocationParams = 0, bool checkNull = 0);

		void* Realloc(void* mem, usize size, const char* name, u64 allocationParams = 0);

		void Free(void* ptr);

		usize Size(void* ptr);
	};

	inline qMemoryPool* gMainMemoryPool;
	inline qList<qMemoryPool> sMemoryPoolList;

	void InternalSetupMainMemoryPool();
	void InitMemorySystem();

	inline qMemoryPool* GetMainMemoryPool()
	{
		if (!gMainMemoryPool) {
			InternalSetupMainMemoryPool();
		}

		return gMainMemoryPool;
	}

#ifdef THEORY_IMPL

	qMemoryPool gMainMemoryPoolBuffer;

	void qMemoryPool::Init(const char* name, usize memory_byte_size, int small_block_byte_size, int can_small_block_overflow_into_large_block, u32 InStatList, qMemoryPool* overflow_pool, int printWarningOnOverflow, bool bInitializeAllocator)
	{
#ifdef THEORY_WINDOWS
		mHeap = HeapCreate(0, 0, memory_byte_size);
		if (!mHeap) {
			qPrintf("ERROR: qMemoryPool initilization failed due to heap been nullptr (pool = '%s')\n", name);
		}
#endif

		Init(name, 0, memory_byte_size, small_block_byte_size, can_small_block_overflow_into_large_block, InStatList, overflow_pool, printWarningOnOverflow, bInitializeAllocator);
	}

	void qMemoryPool::Init(const char* name, char* memory, usize memory_byte_size, int small_block_byte_size, int can_small_block_overflow_into_large_block, u32 InStatList, qMemoryPool* overflow_pool, int printWarningOnOverflow, bool bInitializeAllocator)
	{
		mInitializedUID = MemoryPool_UID;
		mAutoAllocatedBuffer = nullptr;
		mData = reinterpret_cast<qMemoryPoolData*>(mDataBuffer);
		mStart = memory;
		mEnd = &memory[memory_byte_size];
		mOverflowPool = overflow_pool;
		mOverflowOccurred = 0;
		mPrintWarningOnOverflow = printWarningOnOverflow;
		mAmountOfSpilledMemory = 0;
		mMaxAmountOfSpilledMemory = 0;
		mNumActiveSpilledAllocs = 0;
		mTotalSpilledAllocs = 0;
		mFlags = 0;

		sMemoryPoolList.Insert(this);
	}

	void* qMemoryPool::Allocate(usize size, const char* name, u64 allocationParams, bool checkNull)
	{
#ifdef THEORY_WINDOWS
		return HeapAlloc(mHeap, 0, size);
#else
		return ::malloc(size);
#endif
	}

	void* qMemoryPool::Realloc(void* mem, usize size, const char* name, u64 allocationParams)
	{
#ifdef THEORY_WINDOWS
		void* newAlloc = Allocate(size, name, allocationParams, true);
		if (mem)
		{
			usize copySize = Size(mem);
			if (size < copySize) {
				copySize = size;
			}

			qMemCopy(newAlloc, mem, copySize);
			Free(mem);
		}

		return newAlloc;
#else
		return realloc(mem, size);
#endif
	}

	usize qMemoryPool::Size(void* ptr)
	{
#ifdef THEORY_WINDOWS
		return HeapSize(mHeap, 0, ptr);
#else
		return 0; // TODO
#endif
	}

	void qMemoryPool::Free(void* ptr)
	{
#ifdef THEORY_WINDOWS
		HeapFree(mHeap, 0, ptr);
#else
		::free(ptr);
#endif
	}

	void InternalSetupMainMemoryPool()
	{
		gMainMemoryPool = &gMainMemoryPoolBuffer;
		gMainMemoryPool->Init("MainMemoryPool", 0, 1);
	}

	void InitMemorySystem()
	{
		if (!gMainMemoryPool) {
			InternalSetupMainMemoryPool();
		}
	}

#endif
}