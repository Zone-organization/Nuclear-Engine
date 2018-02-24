#pragma once
#include <NE_Common.h>
#include <Core/Entity.h>
#include "Core/Event.h"
#include "Core/System.h"

namespace NuclearEngine
{
	namespace Core
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