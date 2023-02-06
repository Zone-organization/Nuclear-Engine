#include "OpenALBackend.h"
#include <Components/AudioSourceComponent.h>
#include <Utilities/Logger.h>
#include <Assets/AudioClip.h>

#include <OpenAL/include/AL/al.h>
#include <OpenAL/include/AL/alc.h>
#include <OpenAL/include/AL/alext.h>
#pragma comment(lib,"OpenAL32.lib")

namespace Nuclear
{
	namespace Audio
	{
        static void printDeviceList(const char* list)
        {
            if (!list || *list == '\0')
                printf("    !!! none !!!\n");
            else do
            {
                printf("    %s\n", list);
                list += strlen(list) + 1;
            } while (*list != '\0');
        }


        OpenALBackend::OpenALBackend()
        {
            pDevice = NULL;
            pMasterContext = NULL;
        }

        OpenALBackend::~OpenALBackend()
        {
            pDevice = NULL;
            pMasterContext = NULL;
        }

        bool OpenALBackend::Initialize()
		{
            const ALCchar* name;

            /* Open and initialize a device */
            pDevice = alcOpenDevice(NULL);

            if (!pDevice)
            {
                NUCLEAR_ERROR("[OpenALBackend] Could not open a device!");
                return false;
            }

            pMasterContext = alcCreateContext(pDevice, NULL);
            if (pMasterContext == NULL || alcMakeContextCurrent(pMasterContext) == ALC_FALSE)
            {
                if (pMasterContext != NULL)
                    alcDestroyContext(pMasterContext);
                alcCloseDevice(pDevice);

                NUCLEAR_ERROR("[OpenALBackend] Could not set a context!");
                return false;
            }

            name = NULL;
            if (alcIsExtensionPresent(pDevice, "ALC_ENUMERATE_ALL_EXT"))
                name = alcGetString(pDevice, ALC_ALL_DEVICES_SPECIFIER);
            if (!name || alcGetError(pDevice) != AL_NO_ERROR)
                name = alcGetString(pDevice, ALC_DEVICE_SPECIFIER);


            printf("Opened \"%s\"\n", name);


            NUCLEAR_INFO("[OpenALBackend] Initialized OpenAL-Soft successfully.");
            return true;
		}
        void OpenALBackend::Shutdown()
        {
            ALCdevice* device;
            ALCcontext* ctx;

            ctx = alcGetCurrentContext();
            if (ctx == NULL)
                return;

            device = alcGetContextsDevice(ctx);

            alcMakeContextCurrent(NULL);
            alcDestroyContext(ctx);
            alcCloseDevice(device);
        }
        bool OpenALBackend::CreateAudioClip(Uint32& result, AudioFile& file)
        {
            result = 0;
            alGenBuffers(1, &result);


            ALenum format = AL_NONE;

            if (file.mInfo.mChannels == 1)
                format = AL_FORMAT_MONO16;
            else if (file.mInfo.mChannels == 2)
                format = AL_FORMAT_STEREO16;

            alBufferData(result, format, file.mData.data(), file.mData.size(), file.mInfo.mSampleRate);
            
            //free the data
            file.mData.clear();
            file.mData.shrink_to_fit();

            auto err = alGetError();
            if (err != AL_NO_ERROR)
            {
                fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
                if (result && alIsBuffer(result))
                    alDeleteBuffers(1, &result);

                result = 0;
                return false;
            }

            return true;
        }

        bool OpenALBackend::CreateAudioSource(Uint32& source)
        {
            alGenSources(1, &source);
            auto err = alGetError();
            if (err != AL_NO_ERROR)
            {
                NUCLEAR_ERROR("[OpenALBackend] Failed to create AudioSourceComponent!, Error Code: {0}", (int)err);
                return false;
            }
          //  alSourcei(source, AL_SOURCE_RELATIVE, false);
            alSourcef(source, AL_SEC_OFFSET, 0.0f);
            alSourcef(source, AL_REFERENCE_DISTANCE,1.0f);
            alSourcef(source, AL_ROLLOFF_FACTOR, 1.0f);
            return true;
        }

        void OpenALBackend::SetAudioSourceClip(const Uint32 source, const Uint32 clip)
        {
            alSourcei(source, AL_BUFFER, (ALint)clip);
            //TODO: Debug build error checking
        }

        void OpenALBackend::Play(const Uint32 src)
        {
            alSourcePlay(src);

        }
        void OpenALBackend::Pause(const Uint32 src)
        {
            alSourcePause(src);

        }
        void OpenALBackend::Stop(const Uint32 src)
        {
            alSourceStop(src);
        }

        void OpenALBackend::SetSource_Volume(const Uint32 audio_source, float vol)
        {
            alSourcef(audio_source, AL_GAIN, vol);
        }
        void OpenALBackend::SetSource_Pitch(const Uint32 audio_source, float pitch)
        {
            alSourcef(audio_source, AL_PITCH, pitch);
        }
        void OpenALBackend::SetSource_IsLooping(const Uint32 audio_source, bool val)
        {
            alSourcei(audio_source, AL_LOOPING, val);
        }
        void OpenALBackend::SetSource_Transform(const Uint32 audio_source, const Math::Vector3& pos, const Math::Quaternion& rot)
        {
            alSource3f(audio_source, AL_POSITION, pos.x, pos.y, -pos.z);
            auto err = alGetError();
            if (err != AL_NO_ERROR)
            {
                assert(0);
            }
        }
        void OpenALBackend::SetSource_Velocity(const Uint32 audio_source, const Math::Vector3& val)
        {
            alSource3f(audio_source, AL_VELOCITY, val.x, val.y, -val.z);
            auto err = alGetError();
            if (err != AL_NO_ERROR)
            {
                assert(0);
            }
        }
        void OpenALBackend::SetListener_Velocity(const Math::Vector3& val)
        {
            alListener3f( AL_VELOCITY, val.x, val.y, -val.z);
            auto err = alGetError();
            if (err != AL_NO_ERROR)
            {
                assert(0);
            }
        }
        void OpenALBackend::SetListener_Transform(const Math::Vector3& pos, const Math::Quaternion& quat)
        {
            //AL_ORIENTATION = float[6]
            Math::Vector3 orientation[2] =
            {                
                quat* Math::Vector3(0.0f, 0.0f, -1.0f), // Forward
               
                quat* Math::Vector3(0.0f, 1.0f, 0.0f)    // Up
            };
            alListener3f(AL_POSITION, pos.x, pos.y, -pos.z);
            alListenerfv(AL_ORIENTATION, (float*)orientation);
            auto err = alGetError();
            if (err != AL_NO_ERROR)
            {
                assert(0);
            }
        }
	}
}