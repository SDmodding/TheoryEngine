#pragma once

namespace UFG
{
	class qPrintChannel : public qNode<qPrintChannel>
	{
	public:
		enum OutputLevel
		{
			OUTPUT_LEVEL_NONE,
			OUTPUT_LEVEL_ERROR,
			OUTPUT_LEVEL_WARNING,
			OUTPUT_LEVEL_NORMAL,
			OUTPUT_LEVEL_DEBUG
		};

		const char* mName;
		u32 mUID;
		OutputLevel mOutputLevel;
		qString mLogFilename;

		qPrintChannel() {}
		qPrintChannel(const char* name, OutputLevel output_level);

		void VSPrintf(OutputLevel output_level, const char* format, char* arg_list);
		void Print(OutputLevel output_level, const char* format, ...);
	};

#ifdef THEORY_IMPL

	qPrintChannel::qPrintChannel(const char* name, OutputLevel output_level) : mName(name), mOutputLevel(output_level)
	{
		mUID = qStringHash32(name);

#ifdef THEORY_PRINTCHANNEL_DISABLE
		mOutputLevel = OUTPUT_LEVEL_NONE;
#endif

#ifdef THEORY_PRINTCHANNEL_FILE
		qString fileName = R"(Data\printchannel\)";
		fileName += name;
		fileName += "_PrintChannel_Log.txt";

		mLogFilename = fileName;
#endif

	}

	void qPrintChannel::VSPrintf(OutputLevel output_level, const char* format, char* arg_list)
	{
		if (output_level > mOutputLevel) {
			return;
		}

		char buf[0x4000];
		int len = qVSPrintf(buf, sizeof(buf), format, arg_list);

		printf(buf);

		if (!mLogFilename.IsEmpty())
		{
			// This should be 'qAppendPrint'.
			qWriteAppend(mLogFilename, buf, len);
		}
	}

	void qPrintChannel::Print(OutputLevel output_level, const char* format, ...)
	{
		va_list va;
		va_start(va, format);

		VSPrintf(output_level, format, va);

		va_end(format);
	}

#endif
}