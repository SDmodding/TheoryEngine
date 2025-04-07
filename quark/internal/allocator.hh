//====================================================================================
// 
//	This file is incomplete, just barebone implementation for qMemoryPool!
// 
//====================================================================================
#pragma once

namespace UFG
{
	class allocator
	{
	public:
		enum allocation_strategy
		{
			strategy_bestfit,
			strategy_firstfit,
			strategy_lastfit
		};

		enum system_mode
		{
			MODE_FIXED,
			MODE_DYNAMIC
		};

		class block_header
		{
		public:
			enum block_flags
			{
				BL_USED = 1,
				BL_MARK,
				BL_BITS
			};

			block_header* mPrev;
			const char* mName;
			u64 mSizeAndFlags;
			u32 pad0;
			//s8 _padding[];
		};

		class free_link
		{
		public:
			free_link* mNext;
		};

		class page : public qNode<page>
		{
		public:
			free_link* mFreeList;
			u16 mBucketIndex;
			u16 mUseCount;
		};

		class bucket
		{
		public:
			qMutex mLock;
			qList<page, page, 0> mPageList;
			u64 mAllocNumBytes;
			u64 mFreeNumBytes;
			s8 _padding[48];
		};

		class small_free_node : public qNode<small_free_node>
		{
		public:
		};

		class system_page_header : public qNode<system_page_header>
		{
		public:
			u64 mByteSize;
			u64 pad0;
		};

		class stats
		{
		public:
			u64 mSystemPageNumBytes;
			u64 mSystemPageNumPages;
			u64 mSmallSystemAllocNumBytes;
			u64 mSmallSystemAllocNumBlocks;
			u64 mLargePoolNumBytes;
			u64 mLargeAllocNumBytes;
			u64 mLargeAllocNumBlocks;
			u64 mLargeAllocHighWaterNumBytes;
			u64 mSmallPoolNumBytes;
			u64 mSmallPoolNumPages;
			u64 mSmallAllocNumBytes;
			u64 mSmallAllocNumPages;
			u64 mSmallAllocHighWaterNumBytes;
			u64 mSmallAllocHighWaterNumPages;
			u64 mLinearAllocNumBytes;
		};

		// NOTE: This is commented, because we don't need it at the moment and 'intrusive_multi_rbtree' need to be implemented first.

		const char* mName;
		/*int mLargeBlockMemoryPlatformAllocated;
		int mCanSmallBlockOverflowIntoLargeBlock;
		volatile int mAllocationNumber;*/
		system_mode mSystemMode;
		/*qList<system_page_header, system_page_header, 0> mSystemPages;
		char* mFixedSystemPageStart;
		char* mFixedSystemPageEnd;*/
		stats mStats;
		/*qList<system_page_header, system_page_header, 0> mSmallBlockSystemPages;
		char* mSmallBlockStart;
		char* mSmallBlockEnd;
		bucket mBuckets[32];
		qList<page, page, 0> mBucketFreePages;
		qMutex mBucketFreePagesMutex;
		block_header* mMRFreeBlock;
		intrusive_multi_rbtree<free_node> mFreeTree;
		qList<small_free_node, small_free_node, 1> mSmallFreeList;
		qMutex mTreeMutex;
		int mTreeMutexLockCount;
		int mReportBucketIndex;
		int mReportSmallBucketIndex;
		page* mReportSmallPage;
		int mReportSmallSlot;
		system_page_header* mReportLargeSystemPage;
		block_header* mReportLargeBlock;*/

		allocator()
		{
			qMemSet(&mStats, 0, sizeof(mStats));
		}

		void init(const char* name, u8* large_block_memory, u64 large_block_byte_size, u64 small_block_byte_size, int can_small_block_overflow_into_large_block);
	};

#ifdef THEORY_IMPL

	void allocator::init(const char* name, u8* large_block_memory, u64 large_block_byte_size, u64 small_block_byte_size, int can_small_block_overflow_into_large_block)
	{
		if (mStats.mSystemPageNumBytes) {
			return;
		}

		mName = (name ? name : "<Unnamed MemoryPool>");

		if (large_block_memory)
		{
			mSystemMode = MODE_FIXED;

			mStats.mLargePoolNumBytes = large_block_byte_size - sizeof(system_page_header);
			mStats.mSystemPageNumBytes = large_block_byte_size + mStats.mLinearAllocNumBytes;
			mStats.mLargeAllocNumBytes = 0;
			mStats.mLargeAllocNumBlocks = 0;
			mStats.mLargeAllocHighWaterNumBytes = 0;
			mStats.mSystemPageNumPages = 1;
		}
		else {
			mSystemMode = MODE_DYNAMIC;
		}

		// Here should be allocation of bucket, but could be merged above inside the if check.
	}

#endif
}