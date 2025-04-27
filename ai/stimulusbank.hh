#pragma once

struct StimulusRecord
{
	int m_ChildIndex;
	f32 m_MaxSecondsSincePerceived;
};

class StimulusBank : public ActionNodeBank
{
public:
	IMEMBERMAP_CLASSNAME("StimulusBank");

	int m_NumChildren;
	int m_NumStimulusTypes;
	UFG::eStimulusType* m_pStimulusType;
	int* m_pNumRecords;
	StimulusRecord* m_pStimulusRecordBlock;
	StimulusRecord** m_ppFirstStimulusRecord;
	char* m_pNeedToCallMatchOnChild;
	char* m_pNeedToCallMatchOnChildInitialValues;
	f32 m_MaxSecondsSincePerceivedForAnyStimulus;
};