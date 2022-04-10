#pragma once
#include <Core\NE_Common.h>
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
			const char* Name = "AutoVS";
			bool Use_Camera = true;
			bool OutFragPos = true;
		};
		struct AutoPixelShaderDesc
		{
			const char* Name = "AutoPS";

			bool OutputTexture = true;
		};

		//This is very useful class/factory, it automates the shader creation easily, control all their Input & output, may even store them in the future.
		class NEAPI ShaderManager
		{
		public:
			ShaderManager();
			~ShaderManager();

			void CreateShader(const std::string& source, IShader** shader, SHADER_TYPE type);

			void CreateAutoVertexShader(const AutoVertexShaderDesc& desc, IShader** shader, std::vector<LayoutElement>* Layout);
			void CreateAutoPixelShader(const AutoPixelShaderDesc& desc, IShader** shader);

			bool ProcessAndCreatePipeline(
				IPipelineState** PipelineState,
				GraphicsPipelineStateCreateInfo& Desc,
				const std::vector<ShaderResourceVariableDesc>& Resources,
				bool AutoCreateSamplersDesc = true,
				const std::vector<ImmutableSamplerDesc>& StaticSamplers = std::vector<ImmutableSamplerDesc>());

			std::vector<ShaderResourceVariableDesc> ReflectShaderVariables(IShader* VShader, IShader* PShader);

			SHADER_RESOURCE_VARIABLE_TYPE ParseNameToGetType(const std::string& name);

		protected:

		};
	}
}