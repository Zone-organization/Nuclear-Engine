#pragma once
#include <NE_Common.h>
#include <vector>
namespace Nuclear
{
	namespace Audio
	{
		struct AudioClipInfo
		{
			//see SF_INFO (libsndfile)
			Uint32 mFormat;
			Uint32 mSections;
			Uint32 mSeekable;
			Uint32 mBitDepth;
			Uint32 mSamples; //Frames
			Uint32 mChannels;
			Uint32 mSampleRate;
			Uint32 mBitsPerSample;
		};

		struct AudioFile
		{
			std::vector<Byte> mData; //Unlike OpenAL, XAudio expects us to keep the data alive

			AudioClipInfo mInfo;
		};
	}
}