#pragma once
#include <NE_Common.h>
#include <Graphics/API/VertexShader.h>
#include <Graphics/API/PixelShader.h>
#include <Graphics/API/ConstantBuffer.h>

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