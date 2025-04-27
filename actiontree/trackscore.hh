#pragma once

class SequenceTrack : public NodeRefTrack<class SequenceTask>
{
public:
	IMEMBERMAP_CLASSNAME("SequenceTrack");
};

class SpawnTrack : public NodeRefTrack<class SpawnTask>
{
public:
	IMEMBERMAP_CLASSNAME("SpawnTrack");

	bool mKeepAlive;
	qEnum<UFG::eActionTreeType, s8> mSpawnedNodeActionTreeType;
};