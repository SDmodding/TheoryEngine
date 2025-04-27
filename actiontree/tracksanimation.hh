#pragma once

class AnimationTrack : public Track<class AnimationTask>
{
public:
	IMEMBERMAP_CLASSNAME("AnimationTrack");

	SkeletalAnimation* mAnimation;
	f32 mStartFrame;
	f32 mEndFrame;
	f32 mBlendInTime;
	f32 mBlendOutTime;
	f32 mPlayRate;
	int mPlayPriority;
	UFG::qSymbolUC mWeightSetName;
	UFG::qSymbolUC mAnimationName;
	qEnum<AnimationPlayMode, u8> mAnimationPlayMode;
	qEnum<AnimationBlendMode, u8> mAnimationBlendMode;
	qEnum<FloatInputSignal, u8> mFloatInputSignal;
	bool mPlayAnimtionTracks;
	bool mPhaseIn;
	bool mPreserveMotion;
	bool mResetMotionIntentionToFacing;
};

class AnimationBankReferenceTrack : public Track<class AnimationBankReferenceTask>
{
public:
	IMEMBERMAP_CLASSNAME("AnimationBankReferenceTrack");

	BinString m_AnimationBankName;
	qEnum<UFG::eAnimationPriorityEnum, u8> m_eAnimationPriority;
};
