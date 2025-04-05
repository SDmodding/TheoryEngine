#pragma once

namespace UFG
{
	class qPropertyList;
	class qPropertySetResource;

	class qProperty
	{
	public:
		u32 mTypeUIDOffsetChanged;
		u32 mNameUID;
	};

	class qPropertyCollection
	{
	public:
		u32 mFlags;
		u32 mPad0;
		qOffset64<qPropertyCollection*> mOwner;
	};

	class qPropertySetHandle : public qTypedResourceHandle<RTypeUID_qPropertySetResource, qPropertySetResource>
	{
	public:
	};

	class qPropertySet : public qPropertyCollection, public qNode<qPropertySet>
	{
	public:
		enum Type
		{
			Unknown,
			Archetype,
			SceneObject,
			LayerProperties,
			Asset
		};

		enum Flags
		{
			FLAG_RESOURCE_SET = 0x10000,
			FLAG_HAS_SCHEMA = 0x20000,
			FLAG_IS_SCHEMA = 0x40000,
			FLAG_INHERIT_SCHEMA = 0x80000,
			FLAG_COMPONENT_SCHEMA = 0x100000,
			FLAG_CS_SKIP_PARENT_CHECK = 0x200000,
			FLAG_REQUIRES_RECURSIVE_SETUP = 0x400000,
			FLAG_TYPE_START = 0x10000000,
		};

		qOffset64<qPropertySetHandle*> mParents;
		qOffset64<u8*> mValues;
		qOffset64<u32*> mDefaultBits;
		qOffset64<qProperty*> mProperties;
		qSymbol mName;
		u16 mRefCount;
		u16 mNumParents;
		u32 mParentMask;
		qSymbol mSchemaName;
		u32 mPropertyMask;
		u16 mNumDataBytes;
		u16 mNumProperties;
	};

	class qPropertySetResource : public qResourceData
	{
	public:
		enum Flags
		{
			FLAG_MEMIMAGE = 1,
			FLAG_IN_INVENTORY = 2
		};

		u32 mFlags;
		u32 mSourceCRC;
		qOffset64<const char*> mNameString;
		qPropertySet mData;

		void ClearFlags(int flags) { mFlags &= ~flags; }
		int GetFlags(int flags) { return mFlags & flags; }
		const char* GetNameString() { return mNameString.Get(); }
		qPropertySet* GetPropertySet() { return &mData; }
		bool IsMemImaged() { return mFlags & FLAG_MEMIMAGE; }

		//qPropertySet* LoadSetFromXMLNode(XMLDocument* xml_doc, XMLNode* parent_node, qPropertySet* existing_set, bool clearExistingSet);
	};
}