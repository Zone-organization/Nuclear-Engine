#pragma once
#include <NE_Common.h>
#include <ECS/Entity.h>
#include "ECS/Event.h"
#include "ECS/System.h"

namespace NuclearEngine
{
	namespace ECS
	{
		class NEAPI Scene {
		public:
			Scene();
			~Scene();

			//ECS impl.
			EventManager Events;
			EntityManager Entities;
			SystemManager Systems;
		};
	}
}