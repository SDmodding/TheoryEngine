#pragma once

namespace Illusion
{
	class VertexDeclUser
	{
	public:
		int mExampleOfVertexDeclUserData1;
		float mExampleOfVertexDeclUserData2;
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
		unsigned int mStreamStrides[4];
		unsigned int mStreamRuntimeFlags;
		UFG::qOffset64<VertexDeclUser*> mVertexDeclUser;
	};

}