#pragma once

namespace Illusion
{
	class MemImageSchema
	{
	public:
		struct MemStructure
		{
			const char* mName;
			usize mBaseOffset;
			u32 mSize;
			void** mPointer;
			usize mFixupOffset;
			usize mFixupOffsetPointer;
		};

		u32 mNumMemStructures;
		MemStructure mMemStructure[1000];
		usize mCurrSize;
		void* mAllocatedMemory;
		u32 mCurrSerializeIndex;
		u64 mBaseFilePosition;

		virtual ~MemImageSchema() {}

		THEORY_INLINE void Align16() { mCurrSize = UFG::qAlignUp<usize>(mCurrSize, 16); }

		void Allocate(UFG::qMemoryPool* memory_pool = 0, u64 allocation_params = 0);

		void Init();

		THEORY_INLINE void Add(const char* name, u32 size, void** pointer = 0, void* offset_ptr = 0)
		{
			auto memStructure = &mMemStructure[mNumMemStructures++];
			{
				memStructure->mName = name;
				memStructure->mBaseOffset = mCurrSize;
				memStructure->mSize = size;
				memStructure->mPointer = pointer;
				memStructure->mFixupOffset = 0;
				memStructure->mFixupOffsetPointer = 0;

				if (pointer) {
					*pointer = reinterpret_cast<void*>(mCurrSize);
				}

				if (offset_ptr)
				{
					memStructure->mFixupOffset = (mCurrSize - reinterpret_cast<usize>(offset_ptr));
					memStructure->mFixupOffsetPointer = reinterpret_cast<usize>(offset_ptr);
				}
			}
			mCurrSize += static_cast<usize>(size);
		}

		template <typename T>
		THEORY_INLINE void Add(const char* name, T** pointer = 0, void* offset_ptr = 0)
		{
			Add(name, sizeof(T), reinterpret_cast<void**>(pointer), offset_ptr);
		}

		template <typename T>
		THEORY_INLINE void AddAlign(const char* name, T** pointer = 0, void* offset_ptr = 0)
		{
			Add(name, UFG::qAlignUp<u32>(sizeof(T), 16), reinterpret_cast<void**>(pointer), offset_ptr);
		}

		void InitValidation(UFG::qChunkFileBuilder* chunk_builder);

		void BeginValidation(UFG::qChunkFileBuilder* chunk_builder, const char* name);

		void EndValidation(UFG::qChunkFileBuilder* chunk_builder, usize size);
	};

	extern MemImageSchema gMemImageSchema;

	THEORY_INLINE MemImageSchema* GetSchema() { return &gMemImageSchema; }

#ifdef THEORY_IMPL

	MemImageSchema gMemImageSchema;

	void MemImageSchema::Allocate(UFG::qMemoryPool* memory_pool, u64 allocation_params)
	{
		if (!memory_pool) {
			memory_pool = UFG::GetMainMemoryPool();
		}

		Align16();

		void* mem = memory_pool->Allocate(mCurrSize, mMemStructure[0].mName, allocation_params, true);
		UFG::qMemSet(mem, 0, mCurrSize);

		for (u32 i = 0; mNumMemStructures > i; ++i)
		{
			auto memStructure = &mMemStructure[i];
			if (void** pointer = memStructure->mPointer) {
				*pointer = reinterpret_cast<void*>(reinterpret_cast<uptr>(mem) + memStructure->mBaseOffset);
			}

			if (usize fixupOffset = memStructure->mFixupOffset) {
				*reinterpret_cast<usize*>(reinterpret_cast<uptr>(mem) + memStructure->mFixupOffsetPointer) = fixupOffset;
			}
		}

		mAllocatedMemory = mem;
	}

	void MemImageSchema::Init()
	{
		mNumMemStructures = 0;
		mCurrSize = 0;
		mCurrSerializeIndex = 0;
		mBaseFilePosition = 0;
	}

	void MemImageSchema::InitValidation(UFG::qChunkFileBuilder* chunk_builder)
	{
		mCurrSerializeIndex = 0;
		mBaseFilePosition = chunk_builder->GetFilePos();
	}

	void MemImageSchema::BeginValidation(UFG::qChunkFileBuilder* chunk_builder, const char* name)
	{
		auto& serializeStruct = mMemStructure[mCurrSerializeIndex];

		bool nameMatches = (UFG::qStringCompareInsensitive(name, serializeStruct.mName) == 0);
		bool baseMatches = ((chunk_builder->GetFilePos() - mBaseFilePosition) == serializeStruct.mBaseOffset);

		if (!nameMatches)
		{
			UFG::qPrintf("ERROR: MemImageSchema::BeginValidation: Expected a structure '%s' but got a '%s'\n", serializeStruct.mName, name);
			qAssert(nameMatches);
		}

		if (!baseMatches)
		{
			UFG::qPrintf("ERROR: MemImageSchema::BeginValidation: Base Offset mismatch (%d != %d)\n", 
				static_cast<int>(chunk_builder->GetFilePos() - mBaseFilePosition), serializeStruct.mBaseOffset);

			qAssert(baseMatches);
		}

		chunk_builder->LogComment(UFG::qString("Class: %s", name));
	}

	void MemImageSchema::EndValidation(UFG::qChunkFileBuilder* chunk_builder, usize size)
	{
		int serializedPos = static_cast<int>(mMemStructure[mCurrSerializeIndex].mBaseOffset + size);
		int schemaPos = static_cast<int>(chunk_builder->GetFilePos() - mBaseFilePosition);

		if (serializedPos != schemaPos)
		{
			UFG::qPrintf("ERROR: MemImageSchema::EndValidation: Size mismatch (%d != %d)\n", schemaPos, serializedPos);
			qAssert(serializedPos == schemaPos);
		}

		++mCurrSerializeIndex;
	}

#endif
}