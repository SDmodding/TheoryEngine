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

#endif
}