#pragma once
#include <Base\NE_Common.h>
#include <Engine/Graphics/API/VertexShader.h>
#include <Engine/Graphics/API/PixelShader.h>
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

			static Graphics::API::VertexShader CreateAutoVertexShader(const AutoVertexShaderDesc& desc);
			static Graphics::API::PixelShader CreateAutoPixelShader(const AutoPixelShaderDesc& desc);

	

		};
	}
}