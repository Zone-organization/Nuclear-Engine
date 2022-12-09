#pragma once
#include <NE_Common.h>
#include <Assets/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Diligent/Graphics/GraphicsEngine/interface/InputLayout.h>
#include <vector>
#include <string>

namespace Nuclear 
{
	namespace Managers
	{
		struct ShadersReflection
		{
			std::vector<ShaderResourceVariableDesc> mTextures;
		};

		//This is very useful class/factory, it automates the shader creation easily, control all their Input & output, may even store them in the future.
		class NEAPI ShaderManager
		{
		public:
			ShaderManager();
			~ShaderManager();

			void Initialize();

			bool ParseShaderAsset(const std::string& source, Assets::ShaderBuildDesc& desc);

			//ShadersReflection ReflectShaderVariant(Assets::ShaderBuildDesc& desc);

			//ShadersReflection ReflectShaderAsset(Assets::ShaderBuildDesc& desc);

			void CreateShader(IShader** result, const Graphics::ShaderObjectCreationDesc& desc);

			IShaderSourceInputStreamFactory* GetDefaultShaderSourceFactory();
			//////////////////////////////////////////////////////////////////////////////////////////////
			std::vector<LayoutElement> GetBasicVSLayout(bool isDeffered);

			bool ProcessAndCreatePipeline(
				IPipelineState** PipelineState,
				GraphicsPipelineStateCreateInfo& Desc,
				const std::vector<ShaderResourceVariableDesc>& Resources,
				bool AutoCreateSamplersDesc = true,
				const std::vector<ImmutableSamplerDesc>& StaticSamplers = std::vector<ImmutableSamplerDesc>());

			std::vector<ShaderResourceVariableDesc> ReflectShaderVariables(IShader* VShader, IShader* PShader);


			void CreateShader(const std::string& source, IShader** shader, SHADER_TYPE type);

			SHADER_RESOURCE_VARIABLE_TYPE ParseNameToGetType(const std::string& name);

		protected:
			RefCntAutoPtr<IShaderSourceInputStreamFactory> pShaderSourceFactory;
		};
	}
}