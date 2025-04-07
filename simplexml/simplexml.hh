#pragma once

namespace SimpleXML
{
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