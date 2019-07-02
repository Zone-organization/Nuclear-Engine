#pragma once
#include <Base\NE_Common.h>
#include <Engine/Components/CameraComponent.h>

namespace NuclearEngine
{
	namespace Managers
	{
		
		//Control all created camera and their Constant Buffers
		class NEAPI CameraManager
		{
		public:
			CameraManager();
			~CameraManager();



		private:

			std::vector<Components::Camera*> RegisteredCameras;
			std::vector<Components::Camera*> ActiveCameras;
			Components::Camera* MainCamera;
		};
	}
}