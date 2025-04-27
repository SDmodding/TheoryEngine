#pragma once
typedef void(__fastcall* FNInPlaceNew)(void*);

template <typename T>
class InPlaceInit
{
public:
	static void InPlaceNew(void* ptr) { new (ptr) T; }
};

class ConditionCallback
{
public:
	enum TYPE
	{
		ON_ENTER,
		ON_EXIT
	};

	ConditionCallback* mNextCallback;
	//fastdelegate::FastDelegate2<ActionNode*, ActionContext*, void> m_CallbackDelegate;
	TYPE mType;
};

class ActionNode : public Expression::IMemberMap
{
public:
	IMEMBERMAP_CLASSNAME("ActionNode");

	UFG::qOffset64<ActionNode*> mParent;
	ConditionCallback* mFirstCallback;
	u32 mMostUsedIndex;
	u32 mUniqueID;
	ActionID mID;
	bool mBreakPoint;
	bool mDisable;
	s8 mPad0;
	s8 mPad1;
	s8 mPad2;

	void GetResourcePath(char* path, int pathSize) override
	{
		auto fullpath = GetNameFullPath();
		UFG::qSPrintf(path, pathSize, fullpath.GetString_DBG());
	}

	/* Functions */

	void GetNameFullPath(ActionPath& pathToPopulate);

	ActionPath GetNameFullPath()
	{
		ActionPath path;
		GetNameFullPath(path);
		return path;
	}
};

class ActionNodeReference : public ActionNode
{
public:
	IMEMBERMAP_CLASSNAME("ActionNodeReference");

	ActionPath mAnyPathToNode;
	UFG::qOffset64<ActionNode*> mActionNode;
};

class ActionNodeFileReference : public ActionNodeReference
{
public:
	IMEMBERMAP_CLASSNAME("ActionNodeFileReference");

	BinString mRelativeFilePath;
	bool m_Writeable;
	bool m_IsClone;
};

class ActionNodeImplementation : public ActionNode
{
public:
	UFG::qOffset64<ConditionGroup*> mConditions;
	UFG::qOffset64<TrackGroup*> mTracks;
	BinPtrArray<ActionNode, 0, 0> mChildren;
};

class ActionNodePlayable : public ActionNodeImplementation
{
public:
	IMEMBERMAP_CLASSNAME("ActionNodePlayable");
};

class ActionNodeBank : public ActionNodeImplementation, public UFG::qNode<ActionNodeBank>
{
public:
	IMEMBERMAP_CLASSNAME("ActionNodeBank");

	u16 m_Level;
	bool m_NeedsBalancing;
	u64 mSignalMask;
};

class ActionNodeRoot : public ActionNodeBank, public UFG::qNode<ActionNodeRoot>
{
public:
	IMEMBERMAP_CLASSNAME("ActionNodeRoot");

	BinString mToolVersion;
	UFG::qList<IConditionInit, IConditionInit, 0> m_ConditionInitList;
	UFG::qList<ActionNodeRoot, ActionNodeRoot, 0> m_SubRoots;
	UFG::qList<ActionNodeBank, ActionNodeBank, 0> m_BanksToBalance;
	int m_NumNeededActionTreeComponentBaseValueUIDs[11];
	qEnum<UFG::eActionTreeType, s8> mActionTreeType;
};

template <typename T>
class NodeRefTrack : public Track<T>
{
public:
	UFG::qOffset64<ActionNodeReference*> mActionNodeReference;
};

#ifdef THEORY_IMPL

void ActionNode::GetNameFullPath(ActionPath& pathToPopulate)
{
	ActionID nodeIDs[40];
	int numIDs = 0;

	ActionNode* nodes[40];
	int numNodes = 0;

	for (ActionNode* node = this; node; node = node->mParent.Get()) {
		nodes[numNodes++] = node;
	}

	for (int i = numNodes - 1; i >= 0; --i) {
		nodeIDs[numIDs++] = nodes[i]->mID;
	}

	auto& path = pathToPopulate.mPath;
	if (path.mCount > numIDs) {
		path.mCount = 0;
	}
	else {
		path.Reallocate(numIDs, "ActionID.Set");
	}

	auto id = path.mData.Get();
	for (int i = 0; numIDs > i; ++i) {
		*id++ = nodeIDs[i];
	}
}

#endif