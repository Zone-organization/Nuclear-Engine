#pragma once
#include <ECS/Transform.h>
#include <ECS/Entity.h>
#include <Serialization/Access.h>

namespace Nuclear
{
	namespace Components
	{
		class NEAPI EntityInfoComponent
		{
		public:
			EntityInfoComponent(const ECS::Transform& Transform = ECS::Transform(), const std::string& name = "Entity");
			~EntityInfoComponent();

			ECS::Transform mTransform;
			std::string mName;

		private:
			friend Serialization::Access;

			template<class S> void serialize(S& s)
			{
				s.text1b(mName, 256);
				s.object(mTransform);
			}
		};
	}
}