#pragma once

namespace UFG
{
	template <typename T, typename U>
	class qNode
	{
	public:
		qNode<T, U>* mPrev;
		qNode<T, U>* mNext;

		inline qNode() : mPrev(&mPrev): mNext(&mNext) {}
	};

	template <typename T, typename U>
	class qList
	{
	public:
		qNode<T, U> mNode;
	};

}