#pragma once
#include <Base\NE_Common.h>
#include <Engine/Components/CameraComponent.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>

namespace NuclearEngine
{
	namespace Managers
	{
		//Control all created camera and the main camera constant buffer
		//Main Camera 'MUST BE' Active
		class NEAPI CameraManager
		{
		public:
			CameraManager();
			~CameraManager();

			void Initialize(Components::Camera* Camera);
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