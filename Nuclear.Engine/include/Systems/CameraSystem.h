#pragma once
#include <Graphics/Camera.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Assets\DefaultMeshes.h>
#include <ECS\System.h>

namespace Nuclear
{
	namespace Systems
	{
		//Control all created camera and the main camera constant buffer
		//Main Camera 'MUST BE' Active
		class NEAPI CameraSystem : public ECS::System<CameraSystem>
		{
		public:
			CameraSystem(Graphics::Camera* Camera);
			~CameraSystem();

			void Update(ECS::TimeDelta dt) override;

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

			Assets::Mesh CameraScreenQuad;

			//Main camera constant buffer
			RefCntAutoPtr<IBuffer> mCameraCB;
		};
	}
}