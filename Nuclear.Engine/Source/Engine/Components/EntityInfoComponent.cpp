#include "Engine\Components/EntityInfoComponent.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <Core/Math/gtx/quaternion.hpp>
namespace Nuclear
{
	namespace Components
	{
		EntityInfoComponent::EntityInfoComponent()
			: mName("Entity")
		{
		}

		EntityInfoComponent::EntityInfoComponent(Math::Matrix4 Transform)
			: mName("Entity"), mTransform(Transform)
		{
		}

		EntityInfoComponent::EntityInfoComponent(Math::Vector3 position, Math::Quaternion rotation)
			: mName("Entity"), mTransform(position, rotation)
		{
		}

		EntityInfoComponent::~EntityInfoComponent()
		{
		}
	}
}