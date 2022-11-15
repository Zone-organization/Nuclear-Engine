#pragma once
#include <Core\NE_Common.h>
#include <Engine/Assets/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Diligent/Graphics/GraphicsEngine/interface/InputLayout.h>
#include <vector>
#include <string>

namespace Nuclear 
{
	namespace Managers
	{
		//This is very useful class/factory, it automates the shader creation easily, control all their Input & output, may even store them in the future.
		class NEAPI ShaderManager
		{
		public:
			ShaderManager();
			~ShaderManager();

			Assets::ShaderBuildDesc ParseShaderAsset(const std::string& source);

			void CreateShader(const std::string& source, IShader** shader, SHADER_TYPE type);

			std::vector<LayoutElement> GetBasicVSLayout(bool isDeffered);

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