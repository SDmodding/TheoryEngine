#pragma once

namespace UFG
{
	class qBaseNodeRB
	{
	public:
		qBaseNodeRB* mParent;
		qBaseNodeRB* mChild[2]; /* 0: left, 1: right */
		u32 mUID;

		qBaseNodeRB() : mParent(0), mChild{0, 0} {}
		qBaseNodeRB(u32 uid) : mParent(0), mChild{ 0, 0 }, mUID(uid) {}

#ifdef THEORY_DUCKTAPE

		typedef std::map<u32, qBaseNodeRB*> tMap;

		~qBaseNodeRB()
		{
			auto map = reinterpret_cast<tMap*>(mParent);
			if (map)
			{
				const auto it = map->find(mUID);
				if (it != map->end()) {
					map->erase(it);
				}

				mParent = 0;
			}
		}


		void SetUID(u32 uid)
		{
			auto map = reinterpret_cast<tMap*>(mParent);
			if (map)
			{
				const auto it = map->find(mUID);
				if (it != map->end()) {
					map->erase(it);
				}
			}

			mUID = uid;

			if (map) {
				map->operator[](mUID) = this;
			}
		}

#else

		void SetUID(u32 uid) { mUID = uid; }

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
#endif
	};

	template <typename T>
	class qNodeRB
	{
	public:
		qBaseNodeRB mNode;

		qNodeRB() {}
		qNodeRB(u32 uid) : mNode(uid) {}

		THEORY_INLINE T* type() { return static_cast<T*>(this); }
	};

	class qBaseTreeRB
	{
	public:
		qBaseNodeRB mRoot;
		qBaseNodeRB mNULL;
		int mCount;


		qBaseTreeRB() { mRoot.mUID = -1; }

#ifdef THEORY_DUCKTAPE

		~qBaseTreeRB()
		{
			if (mRoot.mChild[0]) {
				delete& GetMap();
			}
		}

		qBaseNodeRB::tMap& GetMap()
		{
			auto mapPtr = reinterpret_cast<qBaseNodeRB::tMap**>(&mRoot.mChild[0]);
			if (auto map = *mapPtr) {
				return *map;
			}

			auto map = new qBaseNodeRB::tMap;
			*mapPtr = map;
			return *map;
		}

		bool IsEmpty() 
		{ 
			auto& map = GetMap();
			if (map.empty())
			{
				mCount = 0;
				return true;
			}
			return false;
		}

		void Add(qBaseNodeRB* z) 
		{
			GetMap()[z->mUID] = z;
			z->mParent = mRoot.mChild[0];

			++mCount;
		}

		void Remove(qBaseNodeRB* z)
		{
			auto& map = GetMap();
			const auto it = map.find(z->mUID);
			if (it != map.end())
			{
				(*it).second->mParent = 0;
				map.erase(it);
			}

			--mCount;
		}

		qBaseNodeRB* Get(u32 uid)
		{
			auto& map = GetMap();
			const auto it = map.find(uid);
			if (it != map.end()) {
				return (*it).second;
			}

			return nullptr;
		}

#else
		qBaseTreeRB();

		bool IsEmpty() { return mCount == 0; }

		void AddHelper(qBaseNodeRB* z);

		void Add(qBaseNodeRB* z);

		void RemoveHelper(qBaseNodeRB* z);

		void Remove(qBaseNodeRB* z);

		qBaseNodeRB* Get(u32 uid);
#endif
	};

	template <typename T, bool FREE = 1>
	class qTreeRB
	{
	public:
		qBaseTreeRB mTree;

		~qTreeRB() { DeleteAll(); }

		THEORY_INLINE bool IsEmpty() { return mTree.IsEmpty(); }

		THEORY_INLINE void Add(qNodeRB<T>* nodeRB) { return mTree.Add(&nodeRB->mNode); }

		THEORY_INLINE void Remove(qNodeRB<T>* nodeRB) { mTree.Remove(&nodeRB->mNode); }

		THEORY_INLINE T* Get(u32 uid) 
		{ 
			auto node = reinterpret_cast<qNodeRB<T>*>(mTree.Get(uid));
			return (node ? node->type() : 0);
		}

#ifdef THEORY_DUCKTAPE

		class qIterator
		{
		public:
			using iterator = qBaseNodeRB::tMap::iterator;

			qIterator(iterator it) : mIt(it) {}

			T* operator*() const { return reinterpret_cast<qNodeRB<T>*>(mIt->second)->type(); }
			T* operator->() const { return reinterpret_cast<qNodeRB<T>*>(mIt->second)->type(); }

			qIterator& operator++() { ++mIt; return *this; }

			bool operator==(const qIterator& other) const { return mIt == other.mIt; }
			bool operator!=(const qIterator& other) const { return mIt != other.mIt; }

		private:
			iterator mIt;
		};

		qIterator begin() { return mTree.GetMap().begin(); }
		qIterator end() { return mTree.GetMap().end(); }
#endif

		void DeleteAll()
		{
			while (!IsEmpty())
			{
				auto node = *begin();
				Remove(node);
				
				node->~T();

				if (FREE) {
					qFree(node);
				}
			}
		}
	};

#ifdef THEORY_IMPL

#ifndef THEORY_DUCKTAPE
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

#endif
}