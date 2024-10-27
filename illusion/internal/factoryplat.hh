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

		static Model* NewModel(const char* name, u32 name_uid, u32 num_meshes, MemImageSchema* schema = 0, UFG::qMemoryPool* memory_pool = 0, u64 allocation_params = 0);

		static StateBlock* NewStateBlock(const char* name, u32 name_uid, u32 num_values, u32 byte_size, MemImageSchema* schema = 0, UFG::qMemoryPool* memory_pool = 0, u64 allocation_params = 0);
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
		schema->AddAlign("Illusion.BufferUser", &pBufferUser, &pBuffer->mBufferUser);
		schema->AddAlign("Illusion.BufferUserPlat", &pBufferUserPlat, &pBufferPlat->mBufferUserPlat);

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
		schema->AddAlign("Illusion.MaterialUser", &pMaterialUser, &pMaterial->mMaterialUser);
		schema->Allocate(memory_pool, allocation_params);

		if (pMaterial) {
			new (pMaterial) Material(name, name_uid, num_params);
		}

		if (pMaterialUser) {
			new (pMaterialUser) MaterialUser();
		}

		return pMaterial;
	}

	Model* Factory::NewModel(const char* name, u32 name_uid, u32 num_meshes, MemImageSchema* schema, UFG::qMemoryPool* memory_pool, u64 allocation_params)
	{
		if (!schema) {
			schema = GetSchema();
		}

		Model* pModel = nullptr;
		ModelPlat* pModelPlat = nullptr;
		ModelUserPlat* pModelUserPlat = nullptr;
		ModelUser* pModelUser = nullptr;

		UFG::qOffset<Mesh*>* pMeshOffset = nullptr;

		schema->Init();
		schema->AddAlign("Illusion.Model", &pModel);
		schema->AddAlign("Illusion.ModelPlat", &pModelPlat);
		schema->AddAlign("Illusion.ModelUserPlat", &pModelUserPlat, &pModelPlat->mModelUserPlat);
		schema->AddAlign("Illusion.ModelUser", &pModelUser, &pModel->mModelUser);

		for (u32 i = 0; num_meshes > i; ++i)
		{
			if (i) 
			{
				schema->Add<UFG::qOffset<Mesh*>>("UFG::qOffset<Mesh*>");
				continue;
			}

			schema->Add<UFG::qOffset<Mesh*>>("UFG::qOffset<Mesh*>", &pMeshOffset, &pModel->mMeshOffsetTable);
		}

		schema->Align16();

		for (u32 i = 0; num_meshes > i; ++i)
		{
			schema->AddAlign<Mesh>("Illusion.Mesh", 0, pMeshOffset++);
			schema->AddAlign<MeshPlat>("Illusion.MeshPlat");
		}

		schema->Allocate(memory_pool, allocation_params);

		if (pModel)
		{
			new (pModel) Model(name_uid, name);  
			
			pModel->mMemoryPool = memory_pool;
			pModel->mNumMeshes = num_meshes;
			pModel->mNumPrims = 0;
		}

		for (u32 i = 0; num_meshes > i; ++i)
		{
			auto pMesh = pModel->GetMesh(i);
			if (pMesh) {
				new (pMesh) Mesh();
			}
		}

		return pModel;
	}


	StateBlock* Factory::NewStateBlock(const char* name, u32 name_uid, u32 num_values, u32 byte_size, MemImageSchema* schema, UFG::qMemoryPool* memory_pool, u64 allocation_params)
	{
		if (!schema) {
			schema = GetSchema();
		}

		StateBlock* pStateBlock = nullptr;

		schema->Init();
		schema->AddAlign("Illusion.StateBlock", &pStateBlock);
		schema->Add("StateBlockData", UFG::qAlignUp<u32>(byte_size, 16));
		schema->AddAlignArray<StateBlock::StateBlockHeader>("StateBlockHeaders", num_values);

		schema->Allocate(memory_pool, allocation_params);

		if (pStateBlock)
		{
			new (pStateBlock) StateBlock(name_uid, name);

			pStateBlock->mNameUID = UFG::qStringHashUpper32(name);
			pStateBlock->mNumValues = num_values;
			pStateBlock->mDataByteSize = byte_size;
		}

		return pStateBlock;
	}

#endif
}