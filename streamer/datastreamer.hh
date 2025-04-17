#pragma once
typedef void(__fastcall* OnLoadCallback)(void*, void*);

namespace UFG
{
	class DataStreamer
	{
	public:
		enum DISK_POS_SORT_MODE
		{
			SORT_BY_PRIORITY,
			SORT_BY_PRIORITY_THEN_DISK_POSITION = 1,
			SORT_BY_DEFAULT = 1,
			SORT_BY_DISK_POSITION = 2
		};

		enum OPENFILES_RESULT
		{
			OPENFILES_OK,
			OPENFILES_FILE_ERROR,
			OPENFILES_NO_MEMORY
		};

		enum PRIORITY
		{
			VERY_LOW_PRIORITY = 0x0,
			LOW_PRIORITY = 0x1000,
			DEFAULT_PRIORITY = 0x2000,
			DEFAULT_ANIM_PRIORITY = 0x2000,
			HIGH_ANIM_PRIORITY = 0x2800,
			HIGH_PRIORITY = 0x3000,
			VERY_HIGH_PRIORITY = 0x4000
		};

		enum STREAM_FLAGS
		{
			MOVEABLE = 0,
			CACHEABLE = 0,
			FIXED = 1,
			UNCACHEABLE = 2,
			DEFAULT = 3,
			MAYNOTEXIST = 4,
			LOCKED = 8,
			HOT_RESOURCE = 16,
			RAW_RESOURCE = 32,
			HI_SFW_PRIORITY = 64,
			SPREAD_UNLOAD = 128,
			ALLOC_FROM_TOP = 256
		};

		enum STREAM_STATE
		{
			STATE_ERROR,
			STATE_EMPTY,
			STATE_LOAD_QUEUED,
			STATE_LOADING,
			STATE_FIXUP_DELAY,
			STATE_RESOURCE_FIXUP,
			STATE_ACTIVE,
			STATE_DEACTIVATING,
			STATE_COUNT
		};

		struct Handle : public qNode<Handle>
		{
			OnLoadCallback mCallback;
			void* mCallbackParam;
			void* mImpl;
			u16 mFlags;
			qEnum<PRIORITY, u16> mPriority;
			qEnum<STREAM_STATE, u16> _mTargetState;

			Handle() : mCallback(0), mCallbackParam(0), mImpl(0), mFlags(DEFAULT), mPriority(DEFAULT_PRIORITY), _mTargetState(STATE_EMPTY) {}

			~Handle()
			{

			}
		};

		struct StreamInfo : public qNodeRB<StreamInfo>, public qNode<StreamInfo, class StreamOrder>, public qNode<StreamInfo, class Priority>
		{
			STREAM_STATE mStateCurrent;
			STREAM_STATE mStateTarget;
			StreamingMemory::DATA_TYPE mDataType;
			f32 mTimeInState;
			u16 mFilesQueried : 1;
			u16 mDBGResLoaded : 1;
			u16 mbWasCPURAMRestoredFromCache : 1;
			u16 mDBG : 1;
			u16 mHotResource : 1;
			u16 mMainAllocFailed : 1;
			u16 mVRAMAllocFailed : 1;
			u16 mIsTagged : 1;
			u16 mAddedExternalChunks : 1;
			u16 mDeactivationTicks;
			BIGFileSize mFileSizePerm;
			u64 mhBufferPerm;
			void* mhFilePerm;
			BIGFileSize mFileSizeTemp;
			qVRAMemoryHandle mhBufferTemp;
			void* mhFileTemp;
			qString mFileName;
			u32 mTagSymbolUID;
			qList<Handle> mHandles;
			u32 mFlagsAND;
			u32 mFlagsOR;
			u32 mDataLoadedTo;
			u64 mDiskPosSortKey;
		};

		// TODO: Implement missing stuff.
	};
}