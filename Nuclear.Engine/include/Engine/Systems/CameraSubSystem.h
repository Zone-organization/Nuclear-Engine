#pragma once
#include <Engine/Components/CameraComponent.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>

namespace Nuclear
{
	namespace Systems
	{
		//Control all created camera and the main camera constant buffer
		//Main Camera 'MUST BE' Active
		class NEAPI CameraSubSystem
		{
		public:
			CameraSubSystem(Components::Camera* Camera);
			~CameraSubSystem();

			void UpdateBuffer();

			void SetMainCamera(Components::Camera* Camera);
			void SetActiveCamera(Components::Camera* Camera);
			void RegisterCamera(Components::Camera* Camera);

			bool isRegisteredCamera(Components::Camera* Camera);
			bool isActiveCamera(Components::Camera* Camera);

			Components::Camera* GetMainCamera();

			IBuffer* GetCameraCB();

			std::vector<Components::Camera*> RegisteredCameras;
			std::vector<Components::Camera*> ActiveCameras;
		private:
			Components::Camera* MainCamera = nullptr;

			//Main camera constant buffer
			RefCntAutoPtr<IBuffer> mCameraCB;
		};
	}
}