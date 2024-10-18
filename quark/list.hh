#pragma once

namespace UFG
{
	template <typename T, typename U = T>
	class qNode
	{
	public:
		qNode<T, U>* mPrev;
		qNode<T, U>* mNext;

		qNode() : mPrev(this), mNext(this) {}
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

		THEORY_INLINE T* prev() { return static_cast<T*>(mPrev); }
		THEORY_INLINE T* next() { return static_cast<T*>(mNext); }
		THEORY_INLINE T* type() { return static_cast<T*>(this); }
	};

	template <typename T, typename U = T>
	class qList
	{
	public:
		qNode<T, U> mNode;

		~qList() { DeleteNodes(); }

		void Clear();

		void DeleteNodes();

		template <typename T, typename U>
		THEORY_INLINE void Insert(qNode<T, U>* node) { mNode.InsertNode(node); }

		template <typename T, typename U>
		THEORY_INLINE void Remove(qNode<T, U>* node) { node->RemoveNode();	}

		THEORY_INLINE bool IsEmpty() { return mNode.mNext == &mNode; }
		THEORY_INLINE T* back() { return mNode.prev(); }
		THEORY_INLINE T* begin() { return mNode.next(); }
		THEORY_INLINE T* end() { return static_cast<T*>(&mNode); }
	};

#ifdef THEORY_IMPL

	template <typename T, typename U>
	void qList<T, U>::DeleteNodes()
	{
		for (auto i = begin(); i != end(); i = begin())
		{
			Remove(i);
			/* Need call destructor and delete? */
		}
	}

#endif
}