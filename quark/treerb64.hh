#pragma once

namespace UFG
{
	template <typename T>
	class qBaseNodeVariableRB
	{
	public:
		qBaseNodeVariableRB<T>* mParent;
		qBaseNodeVariableRB<T>* mChild[2]; /* 0: left, 1: right */
		u64 mUID;

		/* Helpers */

		THEORY_INLINE void SetParent(qBaseNodeVariableRB<T>* p)
		{
			mParent = reinterpret_cast<qBaseNodeVariableRB<T>*>(reinterpret_cast<uptr>(p) | (reinterpret_cast<uptr>(mParent) & 1));
		}

		THEORY_INLINE qBaseNodeVariableRB<T>* GetParent()
		{
			return reinterpret_cast<qBaseNodeVariableRB<T>**>(reinterpret_cast<uptr>(mParent) & ~1);
		}
	};

	template <typename T>
	class qNodeRB64
	{
	public:
		qBaseNodeVariableRB<u64> mNode;
	};

	template <typename T>
	class qBaseTreeVariableRB
	{
	public:
		qBaseNodeVariableRB<T> mRoot;
		qBaseNodeVariableRB<T> mNULL;
		int mCount;

		qBaseTreeVariableRB()
		{
			mRoot.SetParent(&mNULL);
			mRoot.mChild[0] = mRoot.mChild[1] = &mNULL;
			mCount = 0;
		}
	};

	template <typename T, typename U = T>
	class qTreeRB64
	{
	public:
		qBaseTreeVariableRB<u64> mTree;
	};
}