#pragma once

#define SimpleXML_Header	"<?xml version=\"1.0\"?>\n"

namespace SimpleXML
{
	//----------------------------
	//	XML Node
	//----------------------------

	const char* XMLNode::GetAttribute(const char* name)
	{
		auto attribute = GetRoot().attribute(name);
		return attribute.value();
	}

	const char* XMLNode::GetAttribute(const char* name, const char* default_value)
	{
		const char* attribute = GetAttribute(name);
		if (*attribute) {
			return attribute;
		}

		return default_value;
	}

	bool XMLNode::GetAttribute(const char* name, bool default_value)
	{
		return UFG::qToBool(GetAttribute(name), default_value);
	}

	f32 XMLNode::GetAttribute(const char* name, f32 default_value)
	{
		return ConvertToFloat(GetAttribute(name), default_value);
	}

	u32 XMLNode::GetAttribute(const char* name, u32 default_value)
	{
		return UFG::qToUInt32(GetAttribute(name), default_value);
	}

	int XMLNode::GetAttribute(const char* name, int default_value)
	{
		return UFG::qToInt32(GetAttribute(name), default_value);
	}

	const char* XMLNode::GetValue()
	{
		auto child = GetRoot().first_child();
		return child.value();
	}

	f32 XMLNode::GetValue(f32 default_value)
	{
		return ConvertToFloat(GetValue(), default_value);
	}

	u32 XMLNode::GetValue(u32 default_value)
	{
		return UFG::qToUInt32(GetValue(), default_value);
	}

	int XMLNode::GetValue(int default_value)
	{
		return UFG::qToInt32(GetValue(), default_value);
	}

	bool XMLNode::GetBool(bool default_value)
	{
		return UFG::qToBool(GetValue(), default_value);
	}

	const char* XMLNode::GetName()
	{
		auto child = GetRoot().first_child();
		if (child.empty()) {
			return 0;
		}

		if (auto name = child.name()) {
			return name;
		}
		return "";
	}

	int XMLNode::GetAttributeCount()
	{
		auto root = GetRoot();
		int count = 0;
		for (auto i = root.first_attribute(); !i.empty(); i = i.next_attribute()) {
			++count;
		}

		return count;
	}

	int XMLNode::GetChildCount()
	{
		auto root = GetRoot();
		int count = 0;
		for (auto i = root.first_child(); !i.empty(); i = i.next_sibling()) {
			++count;
		}

		return count;
	}

	//----------------------------
	//	XML Document
	//----------------------------

	XMLDocument* XMLDocument::Open(const char* filename, u64 alloc_params, UFG::qMemoryPool* pool)
	{
		auto xmlDoc = new ("XMLDocument.xml_doc", alloc_params) XMLDocument(alloc_params, pool);
		if (!xmlDoc)
		{
			auto dir = UFG::qGetDirectory();
			UFG::qPrintf("ERROR: SimpleXML::Open() - Insufficient memory to open file!\n       file = '%s'\n       dir  = '%s'\n", filename, dir.mData);
			return 0;
		}

		xmlDoc->SetFilename(filename);

		auto res = xmlDoc->mData->mDoc.load_file(filename);
		if (res == pugi::xml_document::LOAD_OK) {
			return xmlDoc;
		}

		auto dir = UFG::qGetDirectory();

		if (res == pugi::xml_document::LOAD_FILE_DOES_NOT_EXIST) {
			UFG::qPrintf("ERROR: SimpleXML::Open() - File does not exist!\n       file = '%s'\n       dir  = '%s'\n", filename, dir.mData);
		}
		else if (res == pugi::xml_document::LOAD_FILE_ERROR) {
			UFG::qPrintf("ERROR: SimpleXML::Open() - File could not be read!\n       file = '%s'\n       dir  = '%s'\n", filename, dir.mData);
		}
		else if (res == pugi::xml_document::LOAD_OUT_OF_MEMORY) {
			UFG::qPrintf("ERROR: SimpleXML::Open() - No memory to load file!\n       file = '%s'\n       dir  = '%s'\n", filename, dir.mData);
		}

		UFG::qDelete(xmlDoc);
		return 0;
	}

	XMLNode* XMLDocument::GetChildNode(const char* name, XMLNode* prev_node)
	{
		pugi::xml_node root = (prev_node ? prev_node->GetRoot() : mData->mDoc);
		auto childNode = root.child(name);
		if (childNode.empty()) {
			return 0;
		}

		return reinterpret_cast<XMLNode*>(childNode._root);
	}

	XMLNode* XMLDocument::GetNode(const char* name, XMLNode* prev_node)
	{
		auto childNode = (prev_node ? prev_node->GetRoot().next_sibling(name) : mData->mDoc.child(name));
		if (childNode.empty()) {
			return 0;
		}

		return reinterpret_cast<XMLNode*>(childNode._root);
	}

	//----------------------------
	//	XML Writer Data
	//----------------------------

	void XMLWriterData::CloseNode(const char* name)
	{
		if (mState == STATE_HEADER_OPEN)
		{
			Write("/>");
			mState = STATE_HEADER_CLOSED;
			return;
		}

		if (mState == STATE_NONE) {
			Write("\t", mIndent);
		}      
		
		Write("</");
		Write(name);
		Write(">");
	}

	void XMLWriterData::Write(const char* text, int count)
	{
		int len = UFG::qStringLength(text);

		if (mTextBuffer)
		{
			u32 requiredSize = mTextBufferSize;
			u32 bufferSize = count * len + mTextBufferPos + 1;
			while (bufferSize > requiredSize) {
				requiredSize *= 2;
			}

			if (requiredSize > mTextBufferSize)
			{
				auto buffer = static_cast<char*>(UFG::qMalloc(requiredSize, "XMLWriter.Write"));
				UFG::qMemCopy(buffer, mTextBuffer, mTextBufferPos);
				buffer[mTextBufferPos] = 0;
				UFG::qFree(mTextBuffer);

				mTextBuffer = buffer;
				mTextBufferSize = requiredSize;
			}

			for (int i = 0; count > i; ++i)
			{
				UFG::qMemCopy(&mTextBuffer[mTextBufferPos], text, len);
				mTextBufferPos += len;
				mTextBuffer[mTextBufferPos] = 0;
			}

			return;
		}

		if (!mFile) {
			return;
		}

		for (int i = 0; count > i; ++i)
		{
			const char* buffer = text;
			u32 size = len;
			if (!size) {
				continue;
			}

			char* writeBuffer;
			while (1)
			{
				u32 writeSize = mWriteBufferSizeBytes - mWriteBufferPos;
				writeBuffer = &mWriteBufferPtr[mWriteBufferPos];
				if (writeSize >= size) {
					break;
				}

				UFG::qMemCopy(writeBuffer, buffer, writeSize);
				mWriteBufferPos += writeSize;

				size -= writeSize;
				buffer += writeSize;

				if (mWriteBufferPtr)
				{
					if (mWriteBufferPos) {
						UFG::qWrite(mFile, mWriteBufferPtr, mWriteBufferPos);
					}

					mWriteBufferPos = 0;
				}

				if (!size) {
					break;
				}
			}

			if (size)
			{
				UFG::qMemCopy(writeBuffer, buffer, size);
				mWriteBufferPos += size;
			}
		}
	}

	//----------------------------
	//	XML Writer
	//----------------------------

	XMLWriter* XMLWriter::Create(const char* filename, bool includeHeader, u32 bufferSize)
	{
		auto file = UFG::qOpen(filename, UFG::QACCESS_WRITE_SEQUENTIAL, 1);
		if (!file) {
			return 0;
		}

		auto xmlWriter = new ("XMLWriter.Create") XMLWriter;
		auto data = xmlWriter->mData;

		data->mWriteBufferPtr = static_cast<char*>(UFG::qMalloc(bufferSize, "XMLWriter.mWriteBuffer"));
		data->mWriteBufferSizeBytes = bufferSize;
		data->mWriteBufferPos = 0;
		data->mFile = file;

		if (includeHeader)
		{
			data->Write(SimpleXML_Header);
			data->Write("\n");
		}

		return xmlWriter;
	}

	XMLWriter* XMLWriter::CreateBuffer(u32 text_buffer_size, bool includeHeader)
	{
		auto xmlWriter = new ("XMLWriter.CreateBuffer") XMLWriter;
		auto data = xmlWriter->mData;

		data->mTextBuffer = static_cast<char*>(UFG::qMalloc(text_buffer_size, "XMLWriter.mTextBuffer"));
		data->mTextBufferSize = text_buffer_size;

		if (includeHeader)
		{
			data->Write(SimpleXML_Header);
			data->Write("\n");
		}

		return xmlWriter;
	}

	int XMLWriter::Close(XMLWriter* writer)
	{
		auto data = writer->mData;

		auto& openNodes = data->mOpenNodes;
		for (auto node = openNodes.back(); node != openNodes.end(); node = openNodes.back())
		{
			--data->mIndent;

			data->CloseNode(*node);
			data->Write("\n");

			// NOTE: In original code this is before '--data->mIndent;'.
			// This isn't correct as it will destruct the "UFG::qString" and it's data and then it will attempt to write...
			UFG::qDelete(node);
		}

		if (data->mWriteBufferPtr)
		{
			// This is in original code twice for some reason...
			/*if (data->mWriteBufferPtr)
			{*/
				if (data->mWriteBufferPos) {
					UFG::qWrite(data->mFile, data->mWriteBufferPtr, data->mWriteBufferPos);
				}

				data->mWriteBufferPos = 0;
			//}

			UFG::qFree(data->mWriteBufferPtr);
		}

		UFG::qClose(data->mFile);

		UFG::qDelete(writer);

		return 1;
	}

	char* XMLWriter::CloseBuffer(XMLWriter*& writer, u32* buffer_size)
	{
		auto data = writer->mData;

		auto& openNodes = data->mOpenNodes;
		for (auto node = openNodes.back(); node != openNodes.end(); node = openNodes.back())
		{
			--data->mIndent;

			data->CloseNode(*node);
			data->Write("\n");

			// NOTE: In original code this is before '--data->mIndent;'.
			// This isn't correct as it will destruct the "UFG::qString" and it's data and then it will attempt to write...
			UFG::qDelete(node);
		}

		if (buffer_size) {
			*buffer_size = data->mTextBufferPos;
		}

		UFG::qDelete(writer);
		writer = 0;

		return data->mTextBuffer;
	}

	bool XMLWriter::CloseBufferAndWriteToFile(const char* filename)
	{
		bool written = false;

		auto writter = this;
		u32 buffer_size = 0;
		if (auto buffer = CloseBuffer(writter, &buffer_size))
		{
			if (filename && *filename && buffer_size)
			{
				if (auto file = UFG::qOpen(filename, UFG::QACCESS_WRITE, 1))
				{
					UFG::qWrite(file, buffer, buffer_size);
					UFG::qClose(file);

					written = true;
				}
			}

			UFG::qFree(buffer);
		}

		return written;
	}

	void XMLWriter::AddAttribute(const char* name, const char* val, bool replaceSpecialCharacters)
	{
		mData->Write(" ");
		mData->Write(name);
		mData->Write("=\"");

		if (replaceSpecialCharacters)
		{
			UFG::qString specialVal = { val };

			for (auto& replaceMap : mReplacementMap) {
				specialVal.ReplaceString(replaceMap.mNaturalString, replaceMap.mReplacementString, 0);
			}

			mData->Write(specialVal);
		}
		else {
			mData->Write(val);
		}

		mData->Write("\"");
	}

	void XMLWriter::AddAttribute(const char* name, f32 val)
	{
		UFG::qString str;
		str.Format("%.7f#%08X", val, val);
		AddAttribute(name, str);
	}

	void XMLWriter::AddAttribute(const char* name, u32 val)
	{
		UFG::qString str;
		str.Format("%u", val);
		AddAttribute(name, str);
	}

	void XMLWriter::AddAttribute(const char* name, u64 val)
	{
		// TODO: Replace this with call to `UFG::qToHex` after it's implemented.

		UFG::qString str;
		str.Format("0x%llX", val, val);
		AddAttribute(name, str);
	}

	void XMLWriter::AddComment(const char* text)
	{
		if (mData->mState == XMLWriterData::STATE_HEADER_OPEN)
		{
			mData->Write(">");

			mData->mState = XMLWriterData::STATE_HEADER_CLOSED;

			mData->Write("\n");
		}  
		
		mData->Write("\t", mData->mIndent);
		mData->Write("<!--");
		mData->Write(text);
		mData->Write("-->");
		mData->Write("\n");

		mData->mState = XMLWriterData::STATE_NONE;
	}


	void XMLWriter::AddValue(const char* val, bool replaceSpecialCharacters)
	{
		if (mData->mState == XMLWriterData::STATE_HEADER_OPEN)
		{
			mData->Write(">");

			mData->mState = XMLWriterData::STATE_HEADER_CLOSED;
		}

		if (replaceSpecialCharacters)
		{
			UFG::qString specialVal = { val };

			for (auto& replaceMap : mReplacementMap) {
				specialVal.ReplaceString(replaceMap.mNaturalString, replaceMap.mReplacementString, 0);
			}

			mData->Write(specialVal);
		}
		else {
			mData->Write(val);
		}
	}

	void XMLWriter::AddValue(f32 val)
	{
		UFG::qString str;
		str.Format("%.7f#%08X", val, val);
		AddValue(str);
	}

	void XMLWriter::AddValue(int val)
	{
		UFG::qString str;
		str.Format("%d", val);
		AddValue(str);
	}

	void XMLWriter::BeginNode(const char* name)
	{
		if (mData->mState == XMLWriterData::STATE_HEADER_OPEN)
		{
			mData->Write(">");

			mData->mState = XMLWriterData::STATE_HEADER_CLOSED;

			mData->Write("\n");
		}

		mData->Write("\t",mData->mIndent);
		mData->Write("<");
		mData->Write(name);

		mData->mOpenNodes.Insert(new ("XMLWriter.BeginNode") UFG::qString(name));

		++mData->mIndent;
		mData->mState = XMLWriterData::STATE_HEADER_OPEN;
	}

	void XMLWriter::EndNode(const char* name)
	{
		UFG::qDelete(mData->mOpenNodes.back());

		--mData->mIndent;

		mData->CloseNode(name);
		mData->Write("\n");

		mData->mState = XMLWriterData::STATE_NONE;
	}
}