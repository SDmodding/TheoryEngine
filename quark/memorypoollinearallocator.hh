#pragma once

namespace UFG
{
	class qLinearAllocator
	{
	public:
		volatile u32 mCurrentPos;
		u32 pad0; // ?
		char* mBuffer0Start;
		char* mBuffer0End;
		char* mBuffer1Start;
		char* mBuffer1End;
		qLinearAllocator* mMainMemoryAddress;
		u32 mLastFrameNumBytes;
		u32 mLastFrameNumAllocBytes;
		u32 mLastFrameNumFreeBytes;
		u32 mLastFrameOverflowed;
		u32 mNumRetries;
		char pad[60]; // ?

		qLinearAllocator();

		void Init(char* buffer0, usize byte_size0, char* buffer1, usize byte_size1);

		bool InFrame(char* address);

		char* Malloc(u32 size, u32 alignment);

		void Reset();
	};

#ifdef THEORY_IMPL

	qLinearAllocator::qLinearAllocator()
	{
		mMainMemoryAddress = this;
		mCurrentPos = 0;

		mBuffer0Start = mBuffer0End = nullptr;
		mBuffer1Start = mBuffer1End = nullptr;

		mLastFrameNumBytes = 0;
		mLastFrameNumAllocBytes = 0;
		mLastFrameNumFreeBytes = 0;
		mLastFrameOverflowed = 0;
		mNumRetries = 0;

		qMemSet(pad, 0, sizeof(pad));
	}

	bool qLinearAllocator::InFrame(char* address)
	{
		return ((address >= mBuffer0Start && address < mBuffer0End) || (address >= mBuffer1Start && address < mBuffer1End));
	}

	char* qLinearAllocator::Malloc(u32 size, u32 alignment)
	{
		// TODO: Implementation needed.
		return 0;
	}

	void qLinearAllocator::Init(char* buffer0, usize byte_size0, char* buffer1, usize byte_size1)
	{
		u32 freeBytes = (mCurrentPos & 0x7FFFFFFF);
		if ((mCurrentPos & 0x80000000) == 0) {
			/* Might be incorrect (didn't test - sneakyevil) */
			freeBytes = static_cast<u32>(reinterpret_cast<usize>(&mBuffer1End[(mCurrentPos & 0x7FFFFFFF) - reinterpret_cast<usize>(mBuffer1Start)]));
		}

		const usize buffer0Bytes = (reinterpret_cast<usize>(mBuffer0End) - reinterpret_cast<usize>(mBuffer0Start));
		const usize buffer1Bytes = (reinterpret_cast<usize>(mBuffer1End) - reinterpret_cast<usize>(mBuffer1Start));

		mLastFrameNumBytes = static_cast<u32>(buffer0Bytes + buffer1Bytes);
		mLastFrameNumAllocBytes = (mLastFrameNumBytes - freeBytes);
		mLastFrameNumFreeBytes = freeBytes;
		mLastFrameOverflowed = (mCurrentPos & 0x80000000) != 0;

		mCurrentPos = static_cast<u32>(byte_size0);

		mBuffer0Start = buffer0;
		mBuffer0End = &buffer0[byte_size0];

		mBuffer1Start = buffer1;
		mBuffer1End = &buffer1[byte_size1];
	}
	
	void qLinearAllocator::Reset()
	{
		mCurrentPos = static_cast<u32>(reinterpret_cast<usize>(mBuffer0End) - reinterpret_cast<usize>(mBuffer0Start));
	}

#endif
}