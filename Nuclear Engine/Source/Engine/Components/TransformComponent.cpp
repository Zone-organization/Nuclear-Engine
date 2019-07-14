#include "Engine\Components/TransformComponent.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <Base/Math/gtx/quaternion.hpp>
namespace NuclearEngine
{
	namespace Components
	{
		TransformComponent::TransformComponent()
		{

		}

		TransformComponent::TransformComponent(Math::Matrix4 Transform)
			: mTransform(Transform)
		{
		}

		TransformComponent::TransformComponent(Math::Vector3 position, Math::Quaternion rotation)
			: mTransform(position, rotation)
		{
		}

		TransformComponent::~TransformComponent()
		{
		}
	}
}