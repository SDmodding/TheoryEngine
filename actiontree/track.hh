#pragma once

class ITrack : public Expression::IMemberMap
{
public:
	u32 m_TrackClassNameUID;
	bool mBreakPoint;
	bool mDisable;
	bool mMaster;
	ExpressionParameterFloat mMasterRate;
	f32 mTimeBegin;
	f32 mTimeEnd;
};

template <typename T>
class Track : public ITrack
{
public:
};

class TrackGroup : public Expression::IMemberMap
{
public:
	BinPtrArray<ITrack, 1, 0> mTracks;
};