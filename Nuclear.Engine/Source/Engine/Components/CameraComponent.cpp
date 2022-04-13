#include "Engine/Components\CameraComponent.h"

namespace Nuclear
{
	using namespace Math;

	namespace Components
	{
		CameraComponent::CameraComponent()
		{
		}
		CameraComponent::CameraComponent(Graphics::Camera* camera)
		{
		}
	
		CameraComponent::~CameraComponent()
		{
		}

		Graphics::Camera* CameraComponent::GetCameraPtr()
		{
			return mCamera;
		}
	}
}