#pragma once

class hkaSkeleton : public hkReferencedObject
{
public:
	struct LocalFrameOnBone
	{
		hkRefPtr<class hkLocalFrame> m_localFrame;

		/// The index of the bone that the frame is attached to.
		i16 m_boneIndex;
	};

	struct Partition
	{
		hkStringPtr m_name;
		i16 m_startBoneIndex;
		i16 m_numBones;
	};

	/// A user name to aid in identifying the skeleton
	hkStringPtr m_name;

	/// Parent relationship
	hkArray<i16> m_parentIndices;

	/// Bones for this skeleton
	hkArray<class hkaBone> m_bones;

	/// The reference pose for the bones of this skeleton. This pose is stored in local space.
	hkArray<hkQsTransformf> m_referencePose;

	/// The reference values for the float slots of this skeleton. This pose is stored in local space.
	hkArray<f32> m_referenceFloats;

	/// Floating point track slots. Often used for auxiliary float data or morph target parameters etc.
	/// This defines the target when binding animations to a particular rig.
	hkArray<hkStringPtr> m_floatSlots;

	/// The local frames that are attached to the bones of this skeleton. These must be ordered
	/// in order of increasing bone index.
	hkArray<struct LocalFrameOnBone> m_localFrames;

	/// A set of partitions of the skeleton.
	hkArray<struct Partition> m_partitions;
};