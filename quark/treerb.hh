#pragma once

namespace UFG
{
	class qBaseNodeRB
	{
	public:
		qBaseNodeRB* mParent;
		qBaseNodeRB* mChild[2];
		u32 mUID;

		inline qBaseNodeRB() : mParent(0), mChild{0, 0}, mUID(0) {}
		inline qBaseNodeRB(u32 uid) : mParent(0), mChild{ 0, 0 }, mUID(uid) {}

		inline void SetUID(u32 uid) { mUID = uid; }
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
		for (auto i = this->mRoot.mChild[0]; i != &this->mNULL; i = i->mChild[i->mUID <= z->mUID]) {
			pParent = i;
		}

		z->mChild[0] = &this->mNULL;
		z->mChild[1] = &this->mNULL;
		z->mParent = pParent;

		if (reinterpret_cast<qBaseTreeRB*>(pParent) == this || pParent->mUID > z->mUID) {
			pParent->mChild[0] = z;
		}
		else {
			pParent->mChild[1] = z;
		}
	}

#endif
}