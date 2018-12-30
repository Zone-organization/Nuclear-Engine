#pragma once
#include <Base\NE_Common.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <vector>

namespace NuclearEngine 
{
	namespace Managers
	{
		struct AutoVertexShaderDesc
		{
			bool Use_Camera = true;

			bool InTexCoords = true;
			bool InNormals = true;
			bool InTangents = false;

			bool OutFragPos = true;
		};
		struct AutoPixelShaderDesc
		{
			bool OutputTexture = true;
		};

		//This is very useful class, it automates the shader creation easily, control all their Input & output, may even store them in the future.
		class NEAPI ShaderManager
		{
		public:
			ShaderManager();
			~ShaderManager();


			static IShader* CreateShader(std::string source, SHADER_TYPE type);

			static IShader* CreateAutoVertexShader(const AutoVertexShaderDesc& desc);
			static IShader* CreateAutoPixelShader(const AutoPixelShaderDesc& desc);
		};
	}
}