#pragma once
#include <Engine/ECS/Scene.h>
#include <Engine/ECS/Event.h>
#include <Engine/ECS/Entity.h>
#include <Engine/ECS/System.h>

namespace NuclearEngine
{
	namespace ECS
	{
		class NEAPI EntityFactory {
		public:
			EntityFactory();
			~EntityFactory();

			Entity CreateBox();

		};
	}
}