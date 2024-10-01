#pragma once

namespace UFG
{
	template <typename T, typename U>
	class qNode
	{
	public:
		qNode<T, U>* mPrev;
		qNode<T, U>* mNext;

		inline qNode() : mPrev(reinterpret_cast<qNode<T, U>*>(&mPrev)), mNext(reinterpret_cast<qNode<T, U>*>(&mNext)) {}
		inline ~qNode()
		{
			mPrev->mNext = mNext;
			mNext->mPrev = mPrev;
			mPrev = mNext = this;
		}
	};

	template <typename T, typename U>
	class qList
	{
	public:
		qNode<T, U> mNode;
	};

}