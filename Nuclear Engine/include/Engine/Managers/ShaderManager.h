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

			bool OutFragPos = false;
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

			bool ProcessAndCreatePipeline(
				IPipelineState** PipelineState,
				PipelineStateDesc& Desc,
				const std::vector<ShaderResourceVariableDesc>& Resources,
				bool AutoCreateSamplersDesc = true,
				const std::vector<StaticSamplerDesc>& StaticSamplers = std::vector<StaticSamplerDesc>());

			std::vector<ShaderResourceVariableDesc> ReflectShaderVariables(IShader* VShader, IShader* PShader);

			SHADER_RESOURCE_VARIABLE_TYPE ParseNameToGetType(const std::string& name);

		protected:

		};
	}
}