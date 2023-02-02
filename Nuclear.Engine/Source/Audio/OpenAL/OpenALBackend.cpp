#include "OpenALBackend.h"
#include <Utilities/Logger.h>
#include <OpenAL/include/AL/al.h>
#include <OpenAL/include/AL/alc.h>
#include <OpenAL/include/AL/alext.h>

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


		bool OpenALBackend::Initialize()
		{
            const ALCchar* name;
            ALCdevice* device;
            ALCcontext* ctx;

            /* Open and initialize a device */
            device = NULL;

            device = alcOpenDevice(NULL);

            if (!device)
            {
                NUCLEAR_ERROR("[OpenALBackend] Could not open a device!");
                return false;
            }

            ctx = alcCreateContext(device, NULL);
            if (ctx == NULL || alcMakeContextCurrent(ctx) == ALC_FALSE)
            {
                if (ctx != NULL)
                    alcDestroyContext(ctx);
                alcCloseDevice(device);

                NUCLEAR_ERROR("[OpenALBackend] Could not set a context!");
                return false;
            }

            name = NULL;
            if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
                name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
            if (!name || alcGetError(device) != AL_NO_ERROR)
                name = alcGetString(device, ALC_DEVICE_SPECIFIER);
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
	}
}