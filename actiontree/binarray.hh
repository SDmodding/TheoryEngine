#pragma once

template <typename T, int WHAT>
class BinArray
{
public:
	int mCount = 0;
	UFG::qOffset64<T*> mData = 0;

	~BinArray()
	{
		if (mCount)
		{
			if (void* data = mData.Get()) {
				UFG::qFree(data);
			}
		}
	}

	void Reallocate(u32 newsize, const char* reason)
	{
		const int newCount = static_cast<int>(newsize);
		if (mCount == newCount) {
			return;
		}

		void* data = UFG::qMalloc(newsize * sizeof(T), reason);
		if (void* oldData = mData.Get())
		{
			UFG::qMemCopy(data, oldData, mCount * sizeof(T));
			UFG::qFree(oldData);
		}

		mData.Set(data);
		mCount = newCount;
	}
};