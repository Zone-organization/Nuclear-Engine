#pragma once
#include <Base\NE_Common.h>
#include <Engine/Assets/Common.h>
#include <FMOD/inc/fmod.hpp>

namespace NuclearEngine
{
	namespace Audio { class Channel; }

	namespace Assets
	{
		class NEAPI AudioClip 
		{
		public:

			void Play(FMOD::Channel* channel = nullptr, bool paused = false);

			FMOD::Sound* mSound;
		};
	}
}