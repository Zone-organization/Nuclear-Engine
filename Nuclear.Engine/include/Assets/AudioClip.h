#pragma once
#include <Assets/IAsset.h>

namespace Nuclear
{
	namespace Audio { class Channel; }

	namespace Assets
	{
		struct AudioFile
		{
			short* mData;
			int mNum_Bytes;

			//see SF_INFO (libsndfile)
			int64_t	frames;		/* Used to be called samples.  Changed to avoid confusion. */
			int			samplerate;
			int			channels;
			int			format;
			int			sections;
			int			seekable;

		};

		class NEAPI AudioClip : public IAsset
		{
		public:
			AudioClip();
			~AudioClip();

		//private:
			Uint32 mBufferID;
		};
	}
}