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
		allocator mAllocator;
	};

	class qMemoryPool : public qNode<qMemoryPool>
	{
	public:
		u32 mInitializedUID;
		void* mAutoAllocatedBuffer;
		qMemoryPoolData* mData;
		s8 mDataBuffer[sizeof(qMemoryPoolData)];
		u32 mFlags;
		bool mUsePageBasedStompFinder;
		u8* mStart;
		u8* mEnd;
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

		qMemoryPool() : mInitializedUID(0), mUsePageBasedStompFinder(false) {}

		void Init(const char* name, u64 memory_byte_size, int small_block_byte_size, int can_small_block_overflow_into_large_block = 1, u32 InStatList = 1, qMemoryPool* overflow_pool = 0, int printWarningOnOverflow = 1, bool bInitializeAllocator = true);

		void Init(const char* name, void* memory, u64 memory_byte_size, int small_block_byte_size, int can_small_block_overflow_into_large_block = 1, u32 InStatList = 1, qMemoryPool* overflow_pool = 0, int printWarningOnOverflow = 1, bool bInitializeAllocator = true);

		void* Allocate(usize size, const char* name, u64 allocationParams = 0, bool checkNull = 0);

		void* Realloc(void* mem, usize size, const char* name, u64 allocationParams = 0);

		void Free(void* ptr);

		usize Size(void* ptr);

		const char* GetName() { return mData->mAllocator.mName; }
	};

	inline qMemoryPool* gMainMemoryPool;
	inline qList<qMemoryPool, qMemoryPool, 0> sMemoryPoolList;
	inline const char* gGlobalNewName = "Global New";

	void InternalSetupMainMemoryPool();
	void InitMemorySystem();

	THEORY_INLINE qMemoryPool* GetMainMemoryPool()
	{
		if (!gMainMemoryPool) {
			InternalSetupMainMemoryPool();
		}

		return gMainMemoryPool;
	}

	THEORY_INLINE void* qMalloc(usize size, const char* name = gGlobalNewName, u64 allocationParams = 0) { return GetMainMemoryPool()->Allocate(size, name, allocationParams, true); }

	THEORY_INLINE void qFree(void* ptr) { gMainMemoryPool->Free(ptr); }

	template <typename T>
	THEORY_INLINE void qDelete(T* ptr) { ptr->~T(); gMainMemoryPool->Free(ptr); }

#ifdef THEORY_IMPL

	qBuffer<qMemoryPool> gMainMemoryPoolBuffer;

	void qMemoryPool::Init(const char* name, u64 memory_byte_size, int small_block_byte_size, int can_small_block_overflow_into_large_block, u32 InStatList, qMemoryPool* overflow_pool, int printWarningOnOverflow, bool bInitializeAllocator)
	{
#ifdef THEORY_WINDOWS
		mHeap = HeapCreate(0, 0, memory_byte_size);
		if (!mHeap) {
			qPrintf("ERROR: qMemoryPool initilization failed due to heap been nullptr (pool = '%s')\n", name);
		}
#endif

		Init(name, 0, memory_byte_size, small_block_byte_size, can_small_block_overflow_into_large_block, InStatList, overflow_pool, printWarningOnOverflow, bInitializeAllocator);
	}

	void qMemoryPool::Init(const char* name, void* memory, u64 memory_byte_size, int small_block_byte_size, int can_small_block_overflow_into_large_block, u32 InStatList, qMemoryPool* overflow_pool, int printWarningOnOverflow, bool bInitializeAllocator)
	{
		auto dataBuffer = reinterpret_cast<qMemoryPoolData*>(mDataBuffer);

		mInitializedUID = MemoryPool_UID;
		mAutoAllocatedBuffer = nullptr;
		mData = dataBuffer;
		mStart = reinterpret_cast<u8*>(memory);
		mEnd = &mStart[memory_byte_size];
		mOverflowPool = overflow_pool;
		mOverflowOccurred = 0;
		mPrintWarningOnOverflow = printWarningOnOverflow;
		mAmountOfSpilledMemory = 0;
		mMaxAmountOfSpilledMemory = 0;
		mNumActiveSpilledAllocs = 0;
		mTotalSpilledAllocs = 0;
		mFlags = 0;

		if (!dataBuffer->mAllocator.mName) {
			new(&dataBuffer->mAllocator)(allocator);
		}

		if (bInitializeAllocator) {
			mData->mAllocator.init(name, mStart, static_cast<u64>(mEnd - mStart), small_block_byte_size, can_small_block_overflow_into_large_block);
		}

		sMemoryPoolList.Insert(this);
	}

	void* qMemoryPool::Allocate(usize size, const char* name, u64 allocationParams, bool checkNull)
	{
		void* mem = 0;

#ifdef THEORY_WINDOWS
		mem = HeapAlloc(mHeap, 0, size);
#else
		mem = ::malloc(size);
#endif

		if (!mem && checkNull && size)
		{
			const char* dbgName = (name ? name : "NULL name passed in");
			qPrintf("\nERROR: *** Out of memory ***\nERROR: Pool name      = %s\nERROR: Requested size = %d [%s]\n", GetName(), size, dbgName);
			qDebugBreak();
		}

		return mem;
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
		if (!ptr) {
			return 0;
		}

#ifdef THEORY_WINDOWS
		return HeapSize(mHeap, 0, ptr);
#else
		return 0; // TODO
#endif
	}

	void qMemoryPool::Free(void* ptr)
	{
		if (!ptr) {
			return;
		}

#ifdef THEORY_WINDOWS
		HeapFree(mHeap, 0, ptr);
#else
		::free(ptr);
#endif
	}

	void InternalSetupMainMemoryPool()
	{
		gMainMemoryPool = new (gMainMemoryPoolBuffer)(qMemoryPool);
		gMainMemoryPool->Init("Main Pool", 0, 1);
	}

	void InitMemorySystem()
	{
		if (!gMainMemoryPool) {
			InternalSetupMainMemoryPool();
		}
	}

#endif
}

THEORY_INLINE void* operator new(usize size, const char* name) noexcept
{
	return UFG::qMalloc(size, name);
}

THEORY_INLINE void operator delete(void* ptr, const char* name) noexcept
{
	UFG::qFree(ptr);
}