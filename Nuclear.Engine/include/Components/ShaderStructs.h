#pragma once
#include <Math\Math.h>

namespace Nuclear
{

	namespace Components
	{

		//TODO: Maybe rename the namespace?
		struct CameraBuffer
		{
			Math::Matrix4 Model = Math::Matrix4(1.0f);
			Math::Matrix4 ModelInvTranspose = Math::Matrix4(1.0f);
			Math::Matrix4 ModelViewProjection = Math::Matrix4(1.0f);

			Math::Matrix4 View = Math::Matrix4(1.0f);
			Math::Matrix4 Projection = Math::Matrix4(1.0f);
		};
	}
}