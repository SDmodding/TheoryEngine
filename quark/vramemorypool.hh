#pragma once

namespace UFG
{
	extern qPrintChannel VRAMChannel;

	class qVRAMemoryHandle : public qNode<qVRAMemoryHandle>
	{
	public:
		char* mData;
		u16 mReadOnlyAndPoolID;
		u16 mBlockID;

		// TODO: Implement missing stuff.

		qVRAMemoryHandle() : mData(0), mReadOnlyAndPoolID(0), mBlockID(0)
		{
			VRAMChannel.Print(qPrintChannel::OUTPUT_LEVEL_DEBUG, "[VRAM Channel] - Creating new empty qVRAMemoryHandle(%p)\n", this);
		}

		~qVRAMemoryHandle() {}
	};

#ifdef THEORY_IMPL

	qPrintChannel VRAMChannel;

#endif
}