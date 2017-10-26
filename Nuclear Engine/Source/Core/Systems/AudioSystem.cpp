#include "Core\Systems\AudioSystem.h"
#include <OpenAL\include\al.h>
#include <OpenAL\include\alc.h>
#include <xaudio2.h>

#pragma comment (lib, "OpenAL32.lib")
#pragma comment (lib, "Xaudio2.lib")

namespace NuclearEngine {
	namespace Core {
		namespace Systems {
			AudioSystem::AudioSystem(AudioAPIs TheAPI)
			{
				SetAudioAPI(TheAPI);
			}
			AudioSystem::~AudioSystem()
			{
			}
			bool AudioSystem::Initialize()
			{
				if (GetAudioAPI() == AudioAPIs::OpenAL)
				{
					return InitOAL();
				}
				else if (GetAudioAPI() == AudioAPIs::XAudio)
				{
					return InitDXA();
				}
				else if (GetAudioAPI() == AudioAPIs::Auto)
				{

					return InitOAL();
					
				}
				else
				{
					Log->Info("[AudioSystem] No Audio API Selected!\n");
					return false;
				}
			}
			bool AudioSystem::InitOAL()
			{
				ALCdevice *device;

				device = alcOpenDevice(NULL);
				if (!device)
				{
					Log->Error("[AudioSystem] Failed To create OpenAL Device!\n");
					return false;
				}

				ALCcontext *context;

				context = alcCreateContext(device, NULL);
				if (!alcMakeContextCurrent(context))
				{
					Log->Error("[AudioSystem] Failed To create OpenAL Context!\n");
					return false;
				}

				Log->Info("----------------------------------------------------------------\n");
				Log->Info("--OpenAL Successfully Initialized\n");
				Log->Info("--OpenAL Info: \n");
				Log->Info("--    Version: ");
				Log->Info((char*)alGetString(AL_VERSION));
				Log->Info("\n--     Vendor: ");
				Log->Info((char*)alGetString(AL_VENDOR));
				Log->Info("\n--   Renderer: ");
				Log->Info((char*)alGetString(AL_RENDERER));
				Log->Info("\n----------------------------------------------------------------\n");

				return true;
			}
			bool AudioSystem::InitDXA()
			{
				HRESULT hr;

				IXAudio2* pXAudio2 = NULL;

				UINT32 flags;

				if (FAILED(hr = XAudio2Create(&pXAudio2, flags)))
				{
					Log->Error("[AudioSystem] Failed To create XAudio2 -Reason: " + hr);
					Log->Info("\n");
					return false;
				}

				IXAudio2MasteringVoice* pMasteringVoice = NULL;

				if (FAILED(hr = pXAudio2->CreateMasteringVoice(&pMasteringVoice)))
				{
					Log->Error("[AudioSystem] Failed To create XAudio2 Mastering Voice -Reason: " + hr);
					Log->Info("\n");
					return false;
				}
				return true;
			}
		}
	}
}