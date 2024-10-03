#pragma once

namespace Illusion
{
	class Factory
	{
	public:
		static Buffer* NewBuffer(const char* name, u32 name_uid, u32 byte_size, MemImageSchema* schema, const char* alloc_name, UFG::qMemoryPool* memory_pool = 0, u64 allocation_params = 0);
	};

#ifdef THEORY_IMPL

	Buffer* Factory::NewBuffer(const char* name, u32 name_uid, u32 byte_size, MemImageSchema* schema, const char* alloc_name, UFG::qMemoryPool* memory_pool, u64 allocation_params)
	{
		if (!schema) {
			schema = GetSchema();
		}

		if (!alloc_name) {
			alloc_name = "Illusion.Buffer";
		}

		Buffer* pBuffer = nullptr;
		BufferPlat* pBufferPlat = nullptr;
		BufferUser* pBufferUser = nullptr;
		BufferUserPlat* pBufferUserPlat = nullptr;
		void* pBufferData = nullptr;

		schema->Init();
		schema->Add(alloc_name, &pBuffer);
		schema->Add("Illusion.BufferPlat", &pBufferPlat);
		schema->Add("Illusion.BufferUser", &pBufferUser, &pBuffer->mBufferUser);
		schema->Add("Illusion.BufferUserPlat", &pBufferUserPlat, &pBufferPlat->mBufferUserPlat);

		schema->Align16();
		schema->Add("Illusion.BufferData", byte_size, &pBufferData, &pBuffer->mData);
		schema->Allocate(memory_pool, allocation_params);

		if (pBuffer)
		{
			new (pBuffer) Buffer(name_uid, name, byte_size);
			pBuffer->mRunTimeCreated = true;
			pBuffer->mMemoryPool = memory_pool;
		}

		if (pBufferPlat) {
			new (pBufferPlat) BufferPlat();
		}

		return pBuffer;
	}

#endif
}