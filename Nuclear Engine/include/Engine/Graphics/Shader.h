#pragma once
#include <LLGL/Shader.h>
#include <Engine\Graphics\ShaderTypes.h>
#include <Engine\Graphics\Context.h>
#include <Base/NE_PrivateCommon.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		class NEAPI Shader 
		{		
		public:
			LLGL::Shader* mShader;
			ShaderReflection Reflection;
			LLGL::VertexFormat mVSFormat;
		};
	}
}
