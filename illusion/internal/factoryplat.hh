#pragma once

namespace Illusion
{
	class Factory
	{
	public:
		static void ClearFinalDeleteQueueAll();

		static void ProcessWaitDeleteQueueAll();

		static Buffer* NewBuffer(const char* name, u32 name_uid, u32 byte_size, MemImageSchema* schema, const char* alloc_name, UFG::qMemoryPool* memory_pool = 0, u64 allocation_params = 0);

		static Material* NewMaterial(const char* name, u32 name_uid, u32 num_params, MemImageSchema* schema = 0, UFG::qMemoryPool* memory_pool = 0, u64 allocation_params = 0);
	};

#ifdef THEORY_IMPL

	void Factory::ClearFinalDeleteQueueAll()
	{
		// TODO
	}

	void Factory::ProcessWaitDeleteQueueAll()
	{
		// TODO
	}

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


	Material* Factory::NewMaterial(const char* name, u32 name_uid, u32 num_params, MemImageSchema* schema, UFG::qMemoryPool* memory_pool, u64 allocation_params)
	{
		if (!schema) {
			schema = GetSchema();
		}

		Material* pMaterial = nullptr;
		MaterialUser* pMaterialUser = nullptr;

		schema->Init();
		schema->Add("Illusion.Material", &pMaterial);

		for (u32 i = 0; num_params > i; ++i) {
			schema->Add<MaterialParam>("Illusion.MaterialParam");
		}

		schema->Align16();
		schema->Add("Illusion.MaterialUser", &pMaterialUser, &pMaterial->mMaterialUser);
		schema->Allocate(memory_pool, allocation_params);

		if (pMaterial) {
			new (pMaterial) Material(name, name_uid, num_params);
		}

		if (pMaterialUser) {
			new (pMaterialUser) MaterialUser();
		}

		return pMaterial;
	}

#endif
}