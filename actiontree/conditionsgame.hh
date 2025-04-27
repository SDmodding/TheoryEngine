#pragma once

class PropertyConditionBase : public Condition
{
public:
	enum ePropertyConditionInitState
	{
		eINIT_STATE_NEEDSRESOLVE,
		eINIT_STATE_NEEDSINIT,
		eINIT_STATE_DONE
	};

	UFG::qOffset64<ActionNodeRoot*> m_MyRoot;
	u32 m_PropertyValueUID;
	u32 m_PropertyValueHasBeenReadUID;
	u32 m_NextTimeToCachePropertyValueUID;
	f32 m_MinTimeBeforeRecachePropertyValue;
	f32 m_MaxTimeBeforeRecachePropertyValue;
	UFG::qSymbol m_PropertySymbol;
	u32 m_Pad0;
	s8 m_Pad1;
	s8 m_CachePropertyValue;
	s8 m_PeriodicallyRecachePropertyValue;
	qEnum<ePropertyConditionInitState, u8> m_InitState;
};

class PropertyFloatCondition : public PropertyConditionBase
{
public:
	IMEMBERMAP_CLASSNAME("PropertyFloatCondition");

	f32 m_Value;
	qEnum<UFG::InEqualityOperator, u8> m_InEqualityOperator;
};


class PropertyBooleanCondition : public PropertyConditionBase
{
public:
	IMEMBERMAP_CLASSNAME("PropertyBooleanCondition");
};

class PropertyIntegerCondition : public PropertyConditionBase
{
public:
	IMEMBERMAP_CLASSNAME("PropertyIntegerCondition");

	s16 m_Value;
	qEnum<UFG::InEqualityOperator, u8> m_InEqualityOperator;
};

class PropertySymbolCondition : public PropertyConditionBase
{
public:
	IMEMBERMAP_CLASSNAME("PropertySymbolCondition");

	UFG::qSymbol m_Value;
};