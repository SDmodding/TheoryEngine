#pragma once

namespace UFG
{
	class qBaseNodeRB
	{
	public:
		qBaseNodeRB* mParent;
		qBaseNodeRB* mChild[2]; /* 0: left, 1: right */
		u32 mUID;

		inline qBaseNodeRB() : mParent(0), mChild{0, 0}, mUID(0) {}
		inline qBaseNodeRB(u32 uid) : mParent(0), mChild{ 0, 0 }, mUID(uid) {}

		inline void SetUID(u32 uid) { mUID = uid; }

		/* Helpers */

		THEORY_INLINE void SetParent(qBaseNodeRB* p)
		{
			mParent = reinterpret_cast<qBaseNodeRB*>(reinterpret_cast<uptr>(p) | (reinterpret_cast<uptr>(mParent) & 1));
		}

		THEORY_INLINE qBaseNodeRB* GetParent()
		{
			return reinterpret_cast<qBaseNodeRB*>(reinterpret_cast<uptr>(mParent) & ~1);
		}

		THEORY_INLINE bool IsRed()
		{ 
			return (reinterpret_cast<uptr>(mParent) & 1);
		}

		THEORY_INLINE bool IsBlack()
		{
			return !(reinterpret_cast<uptr>(mParent) & 1);
		}

		THEORY_INLINE void SetRed()
		{ 
			mParent = reinterpret_cast<qBaseNodeRB*>(reinterpret_cast<uptr>(mParent) | 1);
		}

		THEORY_INLINE void SetBlack()
		{ 
			mParent = reinterpret_cast<qBaseNodeRB*>(reinterpret_cast<uptr>(mParent) & ~1); 
		}
	};

	template <typename T>
	class qNodeRB
	{
	public:
		qBaseNodeRB mNode;

		/* Change this, if there is a better way to do this... */
		THEORY_INLINE T* GetBase() { return reinterpret_cast<T*>(reinterpret_cast<uptr>(this) - offsetof(T, mNode)); }
	};

	class qBaseTreeRB
	{
	public:
		qBaseNodeRB mRoot;
		qBaseNodeRB mNULL;
		int mCount;

		qBaseTreeRB();

		inline bool IsEmpty() { return mCount == 0; }

		void AddHelper(qBaseNodeRB* z);

		void Add(qBaseNodeRB* z);

		void RemoveHelper(qBaseNodeRB* z);

		void Remove(qBaseNodeRB* z);

		qBaseNodeRB* Get(u32 uid);
	};

	template <typename T>
	class qTreeRB
	{
	public:
		qBaseTreeRB mTree;

		THEORY_INLINE qBaseNodeRB* Get(u32 uid) { return mTree.Get(uid); }

		template <typename T>
		THEORY_INLINE qNodeRB<T>* Get(u32 uid) { return reinterpret_cast<qNodeRB<T>*>(Get(uid)); }
	};

#ifdef THEORY_IMPL

	qBaseTreeRB::qBaseTreeRB()
	{
		mRoot.mParent = mRoot.mChild[0] = mRoot.mChild[1] = &mNULL;
		mRoot.SetUID(-1);

		mNULL.mParent = mNULL.mChild[0] = mNULL.mChild[1] = &mNULL;

		mCount = 0;
	}

	void qBaseTreeRB::AddHelper(qBaseNodeRB* z)
	{
		auto pParent = reinterpret_cast<qBaseNodeRB*>(this);
		for (auto i = mRoot.mChild[0]; i != &mNULL; i = i->mChild[i->mUID <= z->mUID]) {
			pParent = i;
		}

		z->mChild[0] = &this->mNULL;
		z->mChild[1] = &this->mNULL;
		z->SetParent(pParent);

		if (reinterpret_cast<qBaseTreeRB*>(pParent) == this || pParent->mUID > z->mUID) {
			pParent->mChild[0] = z;
		}
		else {
			pParent->mChild[1] = z;
		}
	}

	void qBaseTreeRB::Add(qBaseNodeRB* z)
	{
		if (!z || z->GetParent()) {
			return;
		}

		AddHelper(z);
		z->SetRed();

		// TODO: Fix order...

		mRoot.mChild[0]->mParent->SetBlack();
		++mCount;
	}


	void qBaseTreeRB::RemoveHelper(qBaseNodeRB* z)
	{
		// TODO: Implement this...
	}

	void qBaseTreeRB::Remove(qBaseNodeRB* z)
	{
		if (!z || !z->GetParent()) {
			return;
		}

		// TODO: Implement this...
		// --mCount;
	}


	qBaseNodeRB* qBaseTreeRB::Get(u32 uid)
	{
		auto node = &mRoot;

		while (node != &mNULL)
		{
			if (node->mUID == uid && reinterpret_cast<qBaseTreeRB*>(node) != this) {
				break;
			}

			node = node->mChild[uid > node->mUID];
		}

		for (auto i = node->mChild[0]; i; i = i->mChild[0])
		{
			if (i->mUID != uid) {
				break;
			}

			node = i;
		}

		return node;
	}

#endif
}