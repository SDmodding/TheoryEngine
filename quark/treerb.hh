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

		void SetParent(qBaseNodeRB* parent) 
		{
			mParent = reinterpret_cast<qBaseNodeRB*>(reinterpret_cast<uptr>(parent) | (reinterpret_cast<uptr>(mParent) & 1));
		}

		inline qBaseNodeRB* GetParent()
		{
			return reinterpret_cast<qBaseNodeRB*>(reinterpret_cast<uptr>(mParent) & ~1);
		}

		inline bool IsRed() 
		{ 
			return (reinterpret_cast<uptr>(mParent) & 1);
		}

		inline void SetRed() 
		{ 
			mParent = reinterpret_cast<qBaseNodeRB*>(reinterpret_cast<uptr>(mParent) | 1);
		}

		inline void SetBlack() 
		{ 
			mParent = reinterpret_cast<qBaseNodeRB*>(reinterpret_cast<uptr>(mParent) & ~1); 
		}
	};

	template <typename T>
	class qNodeRB
	{
	public:
		qBaseNodeRB mNode;
	};

	class qBaseTreeRB
	{
	public:
		qBaseNodeRB mRoot;
		qBaseNodeRB mNULL;
		int mCount;

		qBaseTreeRB();

		void AddHelper(qBaseNodeRB* z);

		void Add(qBaseNodeRB* x);
	};

	template <typename T>
	class qTreeRB
	{
	public:
		qBaseTreeRB mTree;
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

#endif
}