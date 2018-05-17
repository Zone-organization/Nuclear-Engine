#pragma once
#include <Base\NE_Common.h>
#include <Engine/Graphics/API/VertexShader.h>
#include <Engine/Graphics/API/PixelShader.h>
#include <Engine/Graphics/API/ConstantBuffer.h>

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



			Graphics::API::ConstantBuffer* GetCameraCB();

		};
	}
}