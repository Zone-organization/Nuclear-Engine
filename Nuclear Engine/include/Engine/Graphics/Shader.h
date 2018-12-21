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
			bool isValid = false;
			bool Converted = false;
			bool ConvertShaderRowMajorGLSL = true;
			bool KeepShaderSourceCode = false;



			std::string mSourceCode;
			Uint32 mHashedName;
					   
			LLGL::Shader* mShader;
			ShaderReflection mReflection;
			LLGL::VertexFormat mVSFormat;
		};
	}
}
