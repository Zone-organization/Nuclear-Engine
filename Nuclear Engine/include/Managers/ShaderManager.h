#pragma once
#include <NE_Common.h>
#include <API\VertexShader.h>
#include <API\PixelShader.h>

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

		};

		//This is very useful class, it automates the shader creation easily, control all their Input & output, may even store them in the future.
		class NEAPI ShaderManager
		{
		public:
			ShaderManager();
			~ShaderManager();

			static API::VertexShader CreateAutoVertexShader(const AutoVertexShaderDesc& desc);
			static API::PixelShader CreateAutoPixelShader(const AutoPixelShaderDesc& desc);
		};
	}
}