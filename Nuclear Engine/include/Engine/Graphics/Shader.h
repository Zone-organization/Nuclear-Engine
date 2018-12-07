#pragma once
#include <LLGL/Shader.h>
#include <Engine\Graphics\ShaderTypes.h>
#include <Base/NE_PrivateCommon.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		class NEAPI Shader : LLGL::Shader
		{		
		public:
			ShaderReflection Reflection;
		};
	}
}
