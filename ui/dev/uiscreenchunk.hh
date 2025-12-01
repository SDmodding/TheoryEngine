#pragma once

namespace UFG
{
	class UIScreenChunk : public qResourceData
	{
	public:
		u32 mBufferSize;
		qOffset<void*> mBuffer;
	};
}