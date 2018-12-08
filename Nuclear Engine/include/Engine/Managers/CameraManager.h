#pragma once
#include <Base\NE_Common.h>

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



			//LLGL::Buffer** GetCameraCB();

		};
	}
}