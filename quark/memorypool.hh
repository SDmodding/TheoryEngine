#pragma once

namespace UFG
{
	class qMemoryPool : public qNode<qMemoryPool, qMemoryPool>
	{
	public:
		// TODO: This will be just wrapped for now.

		void* Allocate(size_t size, const char* name, u64 allocationParams, bool checkNull);

		void* Realloc(void* mem, size_t size, const char* name, u64 allocationParams);

		void Free(void* ptr);

		size_t Size(void* ptr);
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

	void* qMemoryPool::Allocate(size_t size, const char* name, u64 allocationParams, bool checkNull)
	{
		return ::malloc(size);
	}

	void* qMemoryPool::Realloc(void* mem, size_t size, const char* name, u64 allocationParams)
	{
		return realloc(mem, size);

		// Uncomment when this Size is implemented.
		/*void* newAlloc = Allocate(size, name, allocationParams, true);
		if (mem)
		{
			size_t copySize = Size(mem);
			if (size > copySize) {
				copySize = size;
			}

			qMemCopy(newAlloc, mem, copySize);
			Free(mem);
		}

		return newAlloc;*/
	}

	size_t qMemoryPool::Size(void* ptr)
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