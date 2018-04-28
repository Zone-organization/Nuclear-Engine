#include <Managers\CameraManager.h>

namespace NuclearEngine
{
	namespace Managers 
	{
		Graphics::API::ConstantBuffer NECamera;

		Graphics::API::ConstantBuffer * CameraManager::GetCameraCB()
		{
			return &NECamera;
		}
	}
}