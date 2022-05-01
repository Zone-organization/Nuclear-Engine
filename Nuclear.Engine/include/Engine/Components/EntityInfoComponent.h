#pragma once
#include <Engine/ECS/Transform.h>
#include <Engine/ECS/Entity.h>

namespace Nuclear
{
	namespace Components
	{
		class NEAPI EntityInfoComponent
		{
		public:
			EntityInfoComponent(const ECS::Transform& Transform = ECS::Transform(), const std::string& name = "Entity");
			~EntityInfoComponent();

			template<class Archive>
			void serialize(Archive& archive)
			{
				archive(CEREAL_NVP(mName));
				archive(CEREAL_NVP(mTransform));
			}

			ECS::Entity mOwnerEntity;
			ECS::Entity mParentEntity;
			ECS::Transform mTransform;
			std::string mName;
		};
	}
}