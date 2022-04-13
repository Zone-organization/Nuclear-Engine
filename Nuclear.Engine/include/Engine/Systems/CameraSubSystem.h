#pragma once
#include <Engine/Graphics/Camera.h>
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
			CameraSubSystem(Graphics::Camera* Camera);
			~CameraSubSystem();

			void UpdateBuffer();

			void SetMainCamera(Graphics::Camera* Camera);
			//void SetActiveCamera(Components::Camera* Camera);
			//void RegisterCamera(Components::Camera* Camera);

			//bool isRegisteredCamera(Components::Camera* Camera);
			//bool isActiveCamera(Components::Camera* Camera);

			Graphics::Camera* GetMainCamera();

			IBuffer* GetCameraCB();

			//std::vector<Components::Camera*> RegisteredCameras;
			//std::vector<Components::Camera*> ActiveCameras;
		private:
			Graphics::Camera* MainCamera = nullptr;

			//Main camera constant buffer
			RefCntAutoPtr<IBuffer> mCameraCB;
		};
	}
}