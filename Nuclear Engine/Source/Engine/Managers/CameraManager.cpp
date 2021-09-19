#include <Engine\Managers\CameraManager.h>
#include <Engine\Graphics\Context.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>

namespace NuclearEngine
{
	namespace Managers 
	{
		CameraManager::CameraManager()
		{
		}
		CameraManager::~CameraManager()
		{
		}
		void CameraManager::Initialize(Components::Camera* Camera)
		{
			BufferDesc CBDesc;
			CBDesc.Name = "CameraCB";
			CBDesc.Size = sizeof(Components::CameraBuffer);
			CBDesc.Usage = USAGE_DYNAMIC;
			CBDesc.BindFlags = BIND_UNIFORM_BUFFER;
			CBDesc.CPUAccessFlags = CPU_ACCESS_WRITE;
			Graphics::Context::GetDevice()->CreateBuffer(CBDesc, &BufferData(), &mCameraCB);

			SetMainCamera(Camera);
			ActiveCameras.push_back(Camera);
		}
		void CameraManager::UpdateBuffer()
		{
			Diligent::MapHelper<Components::CameraBuffer> CBConstants(Graphics::Context::GetContext(), mCameraCB, MAP_WRITE, MAP_FLAG_DISCARD);
			*CBConstants = MainCamera->mCameraData;
		}
		void CameraManager::SetMainCamera(Components::Camera* Camera)
		{
			MainCamera = Camera;
		}
		void CameraManager::SetActiveCamera(Components::Camera* Camera)
		{
			ActiveCameras.push_back(Camera);
		}
		void CameraManager::RegisterCamera(Components::Camera* Camera)
		{
			RegisteredCameras.push_back(Camera);

		}
		bool CameraManager::isRegisteredCamera(Components::Camera* Camera)
		{
			for (auto it : RegisteredCameras)
			{
				if (it == Camera)
					return true;
			}
			return false;
		}
		bool CameraManager::isActiveCamera(Components::Camera* Camera)
		{
			for (auto it : ActiveCameras)
			{
				if (it == Camera)
					return true;
			}
			return false;
		}
		Components::Camera* CameraManager::GetMainCamera()
		{
			return MainCamera;
		}
		IBuffer* CameraManager::GetCameraCB()
		{
			return mCameraCB;
		}
	}
}