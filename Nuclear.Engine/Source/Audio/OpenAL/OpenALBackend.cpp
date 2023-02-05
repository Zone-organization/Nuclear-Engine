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
        bool OpenALBackend::CreateAudioClip(Assets::AudioClip* result, AudioFile& file)
        {
            result->mBufferID = 0;
            alGenBuffers(1, &result->mBufferID);


            ALenum format = AL_NONE;

            if (file.mInfo.mChannels == 1)
                format = AL_FORMAT_MONO16;
            else if (file.mInfo.mChannels == 2)
                format = AL_FORMAT_STEREO16;

            alBufferData(result->mBufferID, format, file.mData.data(), file.mData.size(), file.mInfo.mSampleRate);
            
            //free the data
            file.mData.clear();
            file.mData.shrink_to_fit();

            auto err = alGetError();
            if (err != AL_NO_ERROR)
            {
                fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
                if (result->mBufferID && alIsBuffer(result->mBufferID))
                    alDeleteBuffers(1, &result->mBufferID);

                result->mBufferID = 0;
                return false;
            }



            return true;
        }

        bool OpenALBackend::CreateAudioSource(Components::AudioSourceComponent* source)
        {
            alGenSources(1, &source->mSourceID);
            auto err = alGetError();
            if (err != AL_NO_ERROR)
            {
                NUCLEAR_ERROR("[OpenALBackend] Failed to create AudioSourceComponent!, Error Code: {0}", (int)err);
                return false;
            }
            return true;
        }

        void OpenALBackend::SetAudioSourceClip(Components::AudioSourceComponent* source, Assets::AudioClip* clip)
        {
            alSourcei(source->mSourceID, AL_BUFFER, (ALint)clip->mBufferID);
            //TODO: Debug build error checking
        }

        void OpenALBackend::Play(Components::AudioSourceComponent* src)
        {
            alSourcePlay(src->mSourceID);

        }
        void OpenALBackend::Pause(Components::AudioSourceComponent* src)
        {
            alSourcePause(src->mSourceID);

        }
        void OpenALBackend::Stop(Components::AudioSourceComponent* src)
        {
            alSourceStop(src->mSourceID);
        }

        void OpenALBackend::SetSource_Volume(Components::AudioSourceComponent* audio_source, float vol)
        {
            alSourcef(audio_source->GetSourceID(), AL_GAIN, vol);
        }
        void OpenALBackend::SetSource_Pitch(Components::AudioSourceComponent* audio_source, float pitch)
        {
            alSourcef(audio_source->GetSourceID(), AL_PITCH, pitch);
        }
        void OpenALBackend::SetSource_IsLooping(Components::AudioSourceComponent* audio_source, bool val)
        {
            alSourcei(audio_source->GetSourceID(), AL_LOOPING, val);
        }
	}
}