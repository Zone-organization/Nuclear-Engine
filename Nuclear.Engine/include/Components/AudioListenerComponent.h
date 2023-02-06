#pragma once
#include <NE_Common.h>
#include <Assets\AudioClip.h>
#include <Math/Math.h>

namespace Nuclear
{
	namespace Systems { class AudioSystem; }

	namespace Components
	{

		//each scene should have only one (Active) audio listener since it emulates the player ears 
		//its unrealistic to have multiple audio listeners. (realistically you have only one pair of ears)

		class NEAPI AudioListenerComponent		//= OpenAL Context i guess
		{
		public:
			AudioListenerComponent();
			~AudioListenerComponent();


		protected:
			friend class Systems::AudioSystem;

			bool mIsActive;

			Math::Vector3 mVelocity;
			Math::Vector3 mPrevPos;
		};
	}
}