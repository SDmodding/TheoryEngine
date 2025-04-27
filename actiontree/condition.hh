#pragma once

class IConditionInit : public UFG::qNode<IConditionInit>
{
public:
	virtual void Init(class ActionContext* action_context) = 0;
};

class Condition : public Expression::IMemberMap
{
public:
	u32 mPad0;
	u16 mPad1;
	s8 mPad2;
	bool mBreakPoint;
};

class ConditionGroup : public Condition
{
public:
	u64 mSignalMask;
	BinPtrArray<Condition, 1, 0> mConditions;
};