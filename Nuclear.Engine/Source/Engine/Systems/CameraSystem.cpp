#include <Engine\Systems\CameraSystem.h>
#include <Engine\Graphics\Context.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Engine\Assets\DefaultMeshes.h>

namespace Nuclear
{
	namespace Systems 
	{
		CameraSystem::CameraSystem(Graphics::Camera* Camera)
		{
			BufferDesc CBDesc;
			CBDesc.Name = "CameraCB";
			CBDesc.Size = sizeof(Graphics::CameraBuffer);
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			Graphics::Context::GetInstance().GetDevice()->CreateBuffer(CBDesc, nullptr, &mCameraCB);

			SetMainCamera(Camera);
			//ActiveCameras.push_back(Camera);
		}
		CameraSystem::~CameraSystem()
		{
		}
		void CameraSystem::Update(ECS::TimeDelta dt)
		{
			UpdateBuffer();
		}
		void CameraSystem::UpdateBuffer()
		{
			Diligent::MapHelper<Graphics::CameraBuffer> CBConstants(Graphics::Context::GetInstance().GetContext(), mCameraCB, MAP_WRITE, MAP_FLAG_DISCARD);
			*CBConstants = MainCamera->mCameraData;
		}
		void CameraSystem::SetMainCamera(Graphics::Camera* Camera)
		{
			MainCamera = Camera;
		}
		//void CameraSystem::SetActiveCamera(Components::Camera* Camera)
		//{
		//	ActiveCameras.push_back(Camera);
		//}
		//void CameraSystem::RegisterCamera(Components::Camera* Camera)
		//{
		//	RegisteredCameras.push_back(Camera);

		//}
		//bool CameraSystem::isRegisteredCamera(Components::Camera* Camera)
		//{
		//	for (auto it : RegisteredCameras)
		//	{
		//		if (it == Camera)
		//			return true;
		//	}
		//	return false;
		//}
		//bool CameraSystem::isActiveCamera(Components::Camera* Camera)
		//{
		//	for (auto it : ActiveCameras)
		//	{
		//		if (it == Camera)
		//			return true;
		//	}
		//	return false;
		//}
		Graphics::Camera* CameraSystem::GetMainCamera()
		{
			return MainCamera;
		}
		IBuffer* CameraSystem::GetCameraCB()
		{
			return mCameraCB;
		}
	}
}