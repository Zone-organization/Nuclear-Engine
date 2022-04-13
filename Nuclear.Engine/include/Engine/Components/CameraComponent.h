#pragma once
#include <Core\NE_Common.h>
#include <Engine\Graphics\Camera.h>

namespace Nuclear
{
	namespace Components
	{
		class NEAPI CameraComponent
		{
		public:
			CameraComponent();
			CameraComponent(Graphics::Camera* camera);
			~CameraComponent();


			Graphics::Camera* GetCameraPtr();
		private:
			Graphics::Camera* mCamera;
		};
	}
}