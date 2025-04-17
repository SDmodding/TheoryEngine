#pragma once
//--------------------------------------------------------------------
// 
//	There is some changes related to XMLNode to prevent memory
//	allocation that is only deallocted on destructor.
// 
//--------------------------------------------------------------------

namespace SimpleXML
{
	/*class XMLNodeData
	{
	public:
		pugi::xml_node mNode;
	};

	class XMLNode : public UFG::qNode<XMLNode>
	{
	public:
		XMLNodeData* mData;
	};*/

	class XMLNode
	{
	public:
		pugi::xml_node GetRoot() { return reinterpret_cast<pugi::xml_node_struct*>(this); }

		const char* GetAttribute(const char* name);
		const char* GetAttribute(const char* name, const char* default_value);
		bool GetAttribute(const char* name, bool default_value);
		f32 GetAttribute(const char* name, f32 default_value);
		u32 GetAttribute(const char* name, u32 default_value);
		int GetAttribute(const char* name, int default_value);

		const char* GetValue();
		f32 GetValue(f32 default_value);
		u32 GetValue(u32 default_value);
		int GetValue(int default_value);
		bool GetBool(bool default_value);

		const char* GetName();

		int GetAttributeCount();
		int GetChildCount();

	private:
		f32 ConvertToFloat(const char* value_string, f32 default_value) 
		{
			if (!value_string) {
				return default_value;
			}

			if (auto s = UFG::qStringFind(value_string, "#"))
			{
				u32 value = UFG::qToUInt32(s, *reinterpret_cast<u32*>(&default_value));
				return *reinterpret_cast<f32*>(&value);
			}

			return UFG::qToFloat(value_string, default_value);
		}
	};

	class XMLDocumentData
	{
	public:
		UFG::qString mFilename;
		pugi::xml_document mDoc;
		//UFG::qList<XMLNode, XMLNode, 0> mNodes;
		UFG::qMemoryPool* mPool;

		XMLDocumentData(u64 alloc_params, UFG::qMemoryPool* pool) : mDoc(pool, alloc_params), mPool(pool) { }

		~XMLDocumentData()
		{
			/*for (auto i = mNodes.begin(); i != mNodes.end(); i = mNodes.begin())
			{
				mNodes.Remove(i);
				mPool->Free(i);
			}*/
		}
	};

	class XMLDocument
	{
	public:
		XMLDocumentData* mData;
		UFG::qMemoryPool* mPool;

		/* Constructor, Destructor */

		XMLDocument(u64 alloc_params = 0, UFG::qMemoryPool* pool = 0) : mPool(pool)
		{
			if (!mPool) {
				mPool = UFG::gMainMemoryPool;
			}

			mData = new (mPool->Allocate(sizeof(XMLDocumentData), "XMLDocument.mData", alloc_params)) XMLDocumentData(alloc_params, mPool);
		}

		~XMLDocument()
		{
			mData->~XMLDocumentData();
			mPool->Free(mData);
		}

		/* Static Functions */

		static XMLDocument* Open(const char* filename, u64 alloc_params = 0, UFG::qMemoryPool* pool = 0);

		/* Functions */

		void SetFilename(const char* filename) { mData->mFilename.Set(filename); }
		const char* GetFilename() const { return mData->mFilename; }

		XMLNode* GetChildNode(const char* name, XMLNode* prev_node = 0);
		XMLNode* GetNode(const char* name, XMLNode* prev_node = 0);
	};

	class XMLReplacementMap
	{
	public:
		char mNaturalString[8];
		char mReplacementString[8];
	};

	class XMLWriterData
	{
	public:
		enum State
		{
			STATE_NONE,
			STATE_HEADER_OPEN,
			STATE_HEADER_CLOSED
		};

		UFG::qFile* mFile = 0;
		char* mWriteBufferPtr = 0;
		u32 mWriteBufferSizeBytes = 0;
		u32 mWriteBufferPos = 0;
		char* mTextBuffer = 0;
		u32 mTextBufferSize = 0;
		u32 mTextBufferPos = 0;
		int mIndent = 0;
		State mState = STATE_NONE;
		UFG::qList<UFG::qString> mOpenNodes;

		/* Functions */

		void CloseNode(const char* name);
		void Write(const char* text, int count = 1);
	};

	class XMLWriter
	{
	public:
		XMLWriterData* mData;
		XMLReplacementMap mReplacementMap[5];

		XMLWriter()
		{
			mData = new ("XMLWriter.Data") XMLWriterData;

			UFG::qStringCopy(mReplacementMap[0].mNaturalString, "&");
			UFG::qStringCopy(mReplacementMap[1].mNaturalString, "<");
			UFG::qStringCopy(mReplacementMap[2].mNaturalString, ">");
			UFG::qStringCopy(mReplacementMap[3].mNaturalString, "\"");
			UFG::qStringCopy(mReplacementMap[4].mNaturalString, "'");
			UFG::qStringCopy(mReplacementMap[0].mReplacementString, "&amp;");
			UFG::qStringCopy(mReplacementMap[1].mReplacementString, "&lt;");
			UFG::qStringCopy(mReplacementMap[2].mReplacementString, "&gt;");
			UFG::qStringCopy(mReplacementMap[3].mReplacementString, "&quot;");
			UFG::qStringCopy(mReplacementMap[4].mReplacementString, "&apos;");
		}

		~XMLWriter()
		{
			UFG::qDelete(mData);
		}

		/* Static Functions */

		static XMLWriter* Create(const char* filename, bool includeHeader, u32 bufferSize);
		static XMLWriter* CreateBuffer(u32 text_buffer_size, bool includeHeader = 1);

		static int Close(XMLWriter* writer);
		static char* CloseBuffer(XMLWriter*& writer, u32* buffer_size = 0);

		/* Functions */

		bool CloseBufferAndWriteToFile(const char* filename);

		void AddAttribute(const char* name, const char* val, bool replaceSpecialCharacters = 1);
		void AddAttribute(const char* name, f32 val);
		void AddAttribute(const char* name, u32 val);
		void AddAttribute(const char* name, u64 val);

		void AddComment(const char* text);

		void AddValue(const char* val, bool replaceSpecialCharacters = 1);
		void AddValue(f32 val);
		void AddValue(int val);

		void BeginNode(const char* name);
		void EndNode(const char* name);
	};
}