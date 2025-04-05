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
			RemoveFromList();
		}

		THEORY_INLINE void LinkBeforeNode(qNode<T, U>* node)
		{
			node->mPrev = mPrev;
			node->mNext = this;
			mPrev->mNext = node;
			mPrev = node;
		}

		THEORY_INLINE void RemoveFromList()
		{
			auto prev = mPrev;
			auto next = mNext;
			prev->mNext = next;
			next->mPrev = prev;
			mNext = mPrev = this;
		}

		THEORY_INLINE T* prev() { return static_cast<T*>(mPrev); }
		THEORY_INLINE T* next() { return static_cast<T*>(mNext); }
		THEORY_INLINE T* type() { return static_cast<T*>(this); }
		
	private:
	};

	template <typename T, typename U = T, bool FREE = 1>
	class qList
	{
	public:
		qNode<T, U> mNode;

		~qList() 
		{ 
			DeleteNodes();
		}

		void DeleteNodes()
		{
			for (auto i = begin(); i != end(); i = begin())
			{
				Remove(i);

				i->~T();

				if (FREE) {
					qFree(i);
				}
			}
		}

		void Clear()
		{
			for (auto i = begin(); i != end(); i = begin()) {
				Remove(i);
			}
		}

		template <typename T, typename U>
		THEORY_INLINE void Insert(qNode<T, U>* node) { mNode.LinkBeforeNode(node); }

		template <typename T, typename U>
		THEORY_INLINE void Remove(qNode<T, U>* node) { node->RemoveFromList();	}

		THEORY_INLINE bool IsEmpty() { return mNode.mNext == &mNode; }
		THEORY_INLINE T* back() { return mNode.prev(); }
		THEORY_INLINE T* begin() { return mNode.next(); }
		THEORY_INLINE T* end() { return static_cast<T*>(&mNode); }
	};

#ifdef THEORY_IMPL

#endif
}