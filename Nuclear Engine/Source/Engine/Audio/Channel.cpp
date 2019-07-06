#include <Engine\Audio\Channel.h>
#include <FMOD\inc\fmod.hpp>
#include <FMOD\inc\fmod_errors.h>
namespace NuclearEngine
{
	namespace Audio
	{
		FMOD::Channel* Channel::GetChannel()
		{
			return channel;
		}
		FMOD::Channel ** Channel::GetChannelPtr()
		{
			return &channel;
		}
	}
}