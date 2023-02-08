#pragma once
#include <NE_Common.h>

namespace Nuclear
{
	namespace Audio
	{

        void ConvertToMono8(const Int8* audioIn, Uint8* result, Uint32 num_samples, Uint32 num_channels)
        {
            for (Uint32 i = 0; i < num_samples; i++)
            {
                Int16 sum = 0;
                for (Uint32 j = 0; j < num_channels; j++)
                {
                    sum += *audioIn;
                    ++audioIn;
                }

                *result = sum / num_channels;
                ++result;
            }
        }

        void ConvertToMono16(const Int16* audioIn, Int16* result, Uint32 num_samples, Uint32 num_channels)
        {
            for (Uint32 i = 0; i < num_samples; i++)
            {
                Int32 sum = 0;
                for (Uint32 j = 0; j < num_channels; j++)
                {
                    sum += *audioIn;
                    ++audioIn;
                }

                *result = sum / num_channels;
                ++result;
            }
        }


        void ConvertToMono(const Byte* audioIn, Byte* result, Uint32 bit_depth, Uint32 num_samples_per_channel, Uint32 num_channels)
        {
            switch (bit_depth)
            {
            case 8:
                ConvertToMono8((Int8*)audioIn, result, num_samples_per_channel, num_channels);
                break;
            case 16:
                ConvertToMono16((Int16*)audioIn, (Int16*)result, num_samples_per_channel, num_channels);
                break;
            default:
                break;
            }
        }

	}
}