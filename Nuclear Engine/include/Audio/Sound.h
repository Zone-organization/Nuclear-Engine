#pragma once
#include <NE_Common.h>

namespace FMOD { class Sound; }


#define SOUND_MODE_DEFAULT                   0x00000000  /* Default for all modes listed below. SOUND_MODE_LOOP_OFF, SOUND_MODE_2D, SOUND_MODE_3D_WORLDRELATIVE, SOUND_MODE_3D_INVERSEROLLOFF */
#define SOUND_MODE_LOOP_OFF                  0x00000001  /* For non looping sounds. (DEFAULT).  Overrides SOUND_MODE_LOOP_NORMAL / SOUND_MODE_LOOP_BIDI. */
#define SOUND_MODE_LOOP_NORMAL               0x00000002  /* For forward looping sounds. */
#define SOUND_MODE_LOOP_BIDI                 0x00000004  /* For bidirectional looping sounds. (only works on software mixed static sounds). */
#define SOUND_MODE_2D                        0x00000008  /* Ignores any 3d processing. (DEFAULT). */
#define SOUND_MODE_3D                        0x00000010  /* Makes the sound positionable in 3D.  Overrides SOUND_MODE_2D. */
#define SOUND_MODE_CREATESTREAM              0x00000080  /* Decompress at runtime, streaming from the source provided (ie from disk).  Overrides SOUND_MODE_CREATESAMPLE and SOUND_MODE_CREATECOMPRESSEDSAMPLE.  Note a stream can only be played once at a time due to a stream only having 1 stream buffer and file handle.  Open multiple streams to have them play concurrently. */
#define SOUND_MODE_CREATESAMPLE              0x00000100  /* Decompress at loadtime, decompressing or decoding whole file into memory as the target sample format (ie PCM).  Fastest for playback and most flexible.  */
#define SOUND_MODE_CREATECOMPRESSEDSAMPLE    0x00000200  /* Load MP2/MP3/FADPCM/IMAADPCM/Vorbis/AT9 or XMA into memory and leave it compressed.  Vorbis/AT9/FADPCM encoding only supported in the .FSB container format.  During playback the SOUND_MODE software mixer will decode it in realtime as a 'compressed sample'.  Overrides SOUND_MODE_CREATESAMPLE.  If the sound data is not one of the supported formats, it will behave as if it was created with SOUND_MODE_CREATESAMPLE and decode the sound into PCM. */
#define SOUND_MODE_OPENUSER                  0x00000400  /* Opens a user created static sample or stream. Use SOUND_MODE_CREATESOUNDEXINFO to specify format and/or read callbacks.  If a user created 'sample' is created with no read callback, the sample will be empty.  Use Sound::lock and Sound::unlock to place sound data into the sound if this is the case. */
#define SOUND_MODE_OPENMEMORY                0x00000800  /* "name_or_data" will be interpreted as a pointer to memory instead of filename for creating sounds.  Use SOUND_MODE_CREATESOUNDEXINFO to specify length.  If used with SOUND_MODE_CREATESAMPLE or SOUND_MODE_CREATECOMPRESSEDSAMPLE, SOUND_MODE duplicates the memory into its own buffers.  Your own buffer can be freed after open.  If used with SOUND_MODE_CREATESTREAM, SOUND_MODE will stream out of the buffer whose pointer you passed in.  In this case, your own buffer should not be freed until you have finished with and released the stream.*/
#define SOUND_MODE_OPENMEMORY_POINT          0x10000000  /* "name_or_data" will be interpreted as a pointer to memory instead of filename for creating sounds.  Use SOUND_MODE_CREATESOUNDEXINFO to specify length.  This differs to SOUND_MODE_OPENMEMORY in that it uses the memory as is, without duplicating the memory into its own buffers.  Cannot be freed after open, only after Sound::release.   Will not work if the data is compressed and SOUND_MODE_CREATECOMPRESSEDSAMPLE is not used. */
#define SOUND_MODE_OPENRAW                   0x00001000  /* Will ignore file format and treat as raw pcm.  Use SOUND_MODE_CREATESOUNDEXINFO to specify format.  Requires at least defaultfrequency, numchannels and format to be specified before it will open.  Must be little endian data. */
#define SOUND_MODE_OPENONLY                  0x00002000  /* Just open the file, dont prebuffer or read.  Good for fast opens for info, or when sound::readData is to be used. */
#define SOUND_MODE_ACCURATETIME              0x00004000  /* For System::createSound - for accurate Sound::getLength/Channel::setPosition on VBR MP3, and MOD/S3M/XM/IT/MIDI files.  Scans file first, so takes longer to open. SOUND_MODE_OPENONLY does not affect this. */
#define SOUND_MODE_MPEGSEARCH                0x00008000  /* For corrupted / bad MP3 files.  This will search all the way through the file until it hits a valid MPEG header.  Normally only searches for 4k. */
#define SOUND_MODE_NONBLOCKING               0x00010000  /* For opening sounds and getting streamed subsounds (seeking) asyncronously.  Use Sound::getOpenState to poll the state of the sound as it opens or retrieves the subsound in the background. */
#define SOUND_MODE_UNIQUE                    0x00020000  /* Unique sound, can only be played one at a time */
#define SOUND_MODE_3D_HEADRELATIVE           0x00040000  /* Make the sound's position, velocity and orientation relative to the listener. */
#define SOUND_MODE_3D_WORLDRELATIVE          0x00080000  /* Make the sound's position, velocity and orientation absolute (relative to the world). (DEFAULT) */
#define SOUND_MODE_3D_INVERSEROLLOFF         0x00100000  /* This sound will follow the inverse rolloff model where mindistance = full volume, maxdistance = where sound stops attenuating, and rolloff is fixed according to the global rolloff factor.  (DEFAULT) */
#define SOUND_MODE_3D_LINEARROLLOFF          0x00200000  /* This sound will follow a linear rolloff model where mindistance = full volume, maxdistance = silence. */
#define SOUND_MODE_3D_LINEARSQUAREROLLOFF    0x00400000  /* This sound will follow a linear-square rolloff model where mindistance = full volume, maxdistance = silence. */
#define SOUND_MODE_3D_INVERSETAPEREDROLLOFF  0x00800000  /* This sound will follow the inverse rolloff model at distances close to mindistance and a linear-square rolloff close to maxdistance. */
#define SOUND_MODE_3D_CUSTOMROLLOFF          0x04000000  /* This sound will follow a rolloff model defined by Sound::set3DCustomRolloff / Channel::set3DCustomRolloff.  */
#define SOUND_MODE_3D_IGNOREGEOMETRY         0x40000000  /* Is not affect by geometry occlusion.  If not specified in Sound::setMode, or Channel::setMode, the flag is cleared and it is affected by geometry again. */


namespace NuclearEngine
{
	namespace Audio
	{
		class Channel;

		class NEAPI Sound
		{
		public:

			bool Create(const std::string& path, int mode = SOUND_MODE_DEFAULT);
			void Play(Channel* channel = nullptr, bool paused = false);
		private:
			FMOD::Sound *sound;
		};
	}
}