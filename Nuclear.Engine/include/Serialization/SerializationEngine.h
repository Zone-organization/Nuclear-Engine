#pragma once
#include <NE_Common.h>
#include <Interfaces/Singleton.h>

namespace Nuclear
{
	namespace Assets
	{
		class Scene;
	}
	namespace Serialization
	{
		class NEAPI SerializationEngine : public Interfaces::Singleton<SerializationEngine>
		{
		public:
		
			bool SaveScene(Assets::Scene* scene);
		private:

		};

	}
}