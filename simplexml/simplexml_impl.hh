#pragma once

#define SimpleXML_Header	"<?xml version=\"1.0\"?>\n"

namespace SimpleXML
{
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
					UFG::qWrite(filename, buffer, buffer_size);
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