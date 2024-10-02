#pragma once

namespace UFG
{
	class qMemoryPool : public qNode<qMemoryPool, qMemoryPool>
	{
	public:
		// TODO: This will be just wrapped for now.

		void* Allocate(usize size, const char* name, u64 allocationParams, bool checkNull);

		void* Realloc(void* mem, usize size, const char* name, u64 allocationParams);

		void Free(void* ptr);

		usize Size(void* ptr);
	};

	inline qMemoryPool* gMainMemoryPool;

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

	inline qMemoryPool gMainMemoryPoolBuffer;

	void* qMemoryPool::Allocate(usize size, const char* name, u64 allocationParams, bool checkNull)
	{
		return ::malloc(size);
	}

	void* qMemoryPool::Realloc(void* mem, usize size, const char* name, u64 allocationParams)
	{
		return realloc(mem, size);

		// Uncomment when this Size is implemented.
		/*void* newAlloc = Allocate(size, name, allocationParams, true);
		if (mem)
		{
			usize copySize = Size(mem);
			if (size > copySize) {
				copySize = size;
			}

			qMemCopy(newAlloc, mem, copySize);
			Free(mem);
		}

		return newAlloc;*/
	}

	usize qMemoryPool::Size(void* ptr)
	{
		return 0; // TODO
	}

	void qMemoryPool::Free(void* ptr)
	{
		::free(ptr);
	}

	void InternalSetupMainMemoryPool()
	{
		gMainMemoryPool = &gMainMemoryPoolBuffer;
	}

	void InitMemorySystem()
	{
		if (!gMainMemoryPool) {
			InternalSetupMainMemoryPool();
		}
	}

#endif
}