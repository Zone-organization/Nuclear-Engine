#pragma once
#include <NE_Common.h>
#include <Graphics\Camera.h>

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