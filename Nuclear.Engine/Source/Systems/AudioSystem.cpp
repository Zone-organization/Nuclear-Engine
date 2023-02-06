#include <Systems/AudioSystem.h>
#include <Components/EntityInfoComponent.h>
#include <Components/AudioSourceComponent.h>
#include <Components/AudioListenerComponent.h>
#include <Audio/AudioEngine.h>
#include <Audio/AudioBackend.h>

#include <Core/Scene.h>

namespace Nuclear
{
	namespace Systems
	{
		AudioSystem::AudioSystem()
		{
		}
		AudioSystem::~AudioSystem()
		{
		}
		void AudioSystem::Update(ECS::TimeDelta dt)
		{

			auto listener_view = Core::Scene::GetInstance().GetRegistry().view<Components::AudioListenerComponent>();
			for (auto entity : listener_view)
			{
				auto& audio_listener = listener_view.get<Components::AudioListenerComponent>(entity);

				//if(audio_listener.mIsActive)
				auto Einfo = Core::Scene::GetInstance().GetRegistry().try_get<Components::EntityInfoComponent>(entity);

				const Math::Vector3 pos = Einfo->mTransform.GetLocalPosition();

				if (audio_listener.mPrevPos != pos)
				{
					const Math::Vector3 prevVelocity = audio_listener.mVelocity;
					audio_listener.mVelocity = (pos - audio_listener.mPrevPos) / dt;
					audio_listener.mPrevPos = pos;
					if (audio_listener.mVelocity != prevVelocity)
					{
						Audio::AudioEngine::GetInstance().GetBackend()->SetListener_Velocity(audio_listener.mVelocity);
					}
					Audio::AudioEngine::GetInstance().GetBackend()->SetListener_Transform(pos, Einfo->mTransform.GetLocalRotation());

				}
			}


			auto eView = Core::Scene::GetInstance().GetRegistry().view<Components::AudioSourceComponent>();
			for (auto entity : eView)
			{
				auto& audio_source = eView.get<Components::AudioSourceComponent>(entity);				
				auto Einfo = Core::Scene::GetInstance().GetRegistry().try_get<Components::EntityInfoComponent>(entity);

				const Math::Vector3 pos = Einfo->mTransform.GetLocalPosition();
				
				if (audio_source.mPrevPos != pos)
				{
					const Math::Vector3 prevVelocity = audio_source.mVelocity;
					audio_source.mVelocity = (pos - audio_source.mPrevPos) / dt;
					audio_source.mPrevPos = pos;
					if (audio_source.mVelocity != prevVelocity)
					{
						Audio::AudioEngine::GetInstance().GetBackend()->SetSource_Velocity(audio_source.GetSourceID(), audio_source.mVelocity);
					}
					Audio::AudioEngine::GetInstance().GetBackend()->SetSource_Transform(audio_source.GetSourceID(), pos, Einfo->mTransform.GetLocalRotation());
			
				}
			}
		}
	}
}