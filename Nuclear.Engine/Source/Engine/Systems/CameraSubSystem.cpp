#include <Engine\Systems\CameraSubSystem.h>
#include <Engine\Graphics\Context.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>

namespace Nuclear
{
	namespace Systems 
	{
		CameraSubSystem::CameraSubSystem(Graphics::Camera* Camera)
		{
			BufferDesc CBDesc;
			CBDesc.Name = "CameraCB";
			CBDesc.Size = sizeof(Graphics::CameraBuffer);
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			Graphics::Context::GetDevice()->CreateBuffer(CBDesc, nullptr, &mCameraCB);

			SetMainCamera(Camera);
			//ActiveCameras.push_back(Camera);
		}
		CameraSubSystem::~CameraSubSystem()
		{
		}
		void CameraSubSystem::UpdateBuffer()
		{
			Diligent::MapHelper<Graphics::CameraBuffer> CBConstants(Graphics::Context::GetContext(), mCameraCB, MAP_WRITE, MAP_FLAG_DISCARD);
			*CBConstants = MainCamera->mCameraData;
		}
		void CameraSubSystem::SetMainCamera(Graphics::Camera* Camera)
		{
			MainCamera = Camera;
		}
		//void CameraSubSystem::SetActiveCamera(Components::Camera* Camera)
		//{
		//	ActiveCameras.push_back(Camera);
		//}
		//void CameraSubSystem::RegisterCamera(Components::Camera* Camera)
		//{
		//	RegisteredCameras.push_back(Camera);

		//}
		//bool CameraSubSystem::isRegisteredCamera(Components::Camera* Camera)
		//{
		//	for (auto it : RegisteredCameras)
		//	{
		//		if (it == Camera)
		//			return true;
		//	}
		//	return false;
		//}
		//bool CameraSubSystem::isActiveCamera(Components::Camera* Camera)
		//{
		//	for (auto it : ActiveCameras)
		//	{
		//		if (it == Camera)
		//			return true;
		//	}
		//	return false;
		//}
		Graphics::Camera* CameraSubSystem::GetMainCamera()
		{
			return MainCamera;
		}
		IBuffer* CameraSubSystem::GetCameraCB()
		{
			return mCameraCB;
		}
	}
}