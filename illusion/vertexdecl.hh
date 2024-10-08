#pragma once

namespace Illusion
{
	class VertexDeclUser
	{
	public:
		int mExampleOfVertexDeclUserData1;
		f32 mExampleOfVertexDeclUserData2;
	};


	class IVertexDeclPlat
	{
	public:
	};

	class VertexDecl : public IVertexDeclPlat, public UFG::qResourceData
	{
	public:
		UFG::qOffset64<VertexStreamElement*> mElements;
		int mNumElements;
		u32 mStreamStrides[4];
		u32 mStreamRuntimeFlags;
		UFG::qOffset64<VertexDeclUser*> mVertexDeclUser;
	};

}