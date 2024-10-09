#pragma once

namespace UFG
{
	template <typename T, typename U = T>
	class qNode
	{
	public:
		qNode<T, U>* mPrev;
		qNode<T, U>* mNext;

		qNode() : mPrev(reinterpret_cast<qNode<T, U>*>(&mPrev)), mNext(reinterpret_cast<qNode<T, U>*>(&mNext)) {}
		~qNode()
		{
			RemoveNode();
		}

		template <typename T, typename U>
		THEORY_INLINE void InsertNode(qNode<T, U>* node)
		{
			node->mPrev = mPrev;
			node->mNext = this;
			mPrev->mNext = node;
			mPrev = node;
		}

		THEORY_INLINE void RemoveNode()
		{
			mPrev->mNext = mNext;
			mNext->mPrev = mPrev;
			mPrev = mNext = this;
		}
	};

	template <typename T, typename U = T>
	class qList
	{
	public:
		qNode<T, U> mNode;

		~qList();

		void Clear();

		void DeleteNodes();

		template <typename T, typename U>
		THEORY_INLINE void Insert(qNode<T, U>* node)
		{
			mNode.InsertNode(node);
		}

		template <typename T, typename U>
		THEORY_INLINE void Remove(qNode<T, U>* node)
		{
			node->RemoveNode();
		}
	};

#ifdef THEORY_IMPL

	template <typename T, typename U>
	qList<T, U>::~qList()
	{
		DeleteNodes();
	}

	template <typename T, typename U>
	void qList<T, U>::DeleteNodes()
	{
		for (auto i = mNode.mNext; i != &mNode; i = mNode.mNext)
		{
			Remove(i);
			/* Need call destructor and delete? */
		}
	}

#endif
}