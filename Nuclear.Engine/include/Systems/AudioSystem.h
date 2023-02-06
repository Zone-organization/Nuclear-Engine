#pragma once
#include <NE_Common.h>
#include <ECS/ECS.h>

namespace Nuclear
{
	namespace Systems
	{
		class NEAPI AudioSystem : public ECS::System<AudioSystem>
		{
		public:
			AudioSystem();
			~AudioSystem();

	
			void Update(ECS::TimeDelta dt) override;

		private:

		};

	}
}