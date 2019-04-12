#pragma once
#include <Base\NE_Common.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Diligent/Graphics/GraphicsEngine/interface/InputLayout.h>
#include <vector>
#include <string>

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

		//This is very useful class/factory, it automates the shader creation easily, control all their Input & output, may even store them in the future.
		class NEAPI ShaderManager
		{
		public:
			ShaderManager();
			~ShaderManager();

			IShader* CreateShader(const std::string& source, SHADER_TYPE type);

			IShader* CreateAutoVertexShader(const AutoVertexShaderDesc& desc, std::vector<LayoutElement>* Layout);
			IShader* CreateAutoPixelShader(const AutoPixelShaderDesc& desc);

			//Not Done
			IPipelineState* CreatePipelineState(IShader* VertexShader, IShader* PixelShader);

			std::vector<ShaderResourceVariableDesc> ReflectShaderVariables(IShader* VShader, IShader* PShader);

			SHADER_RESOURCE_VARIABLE_TYPE ParseNameToGetType(const std::string& name);

		protected:

		};
	}
}