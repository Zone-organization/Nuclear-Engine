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
		struct ShaderCreationDesc
		{
			std::string mName;
			SHADER_TYPE mType;
			std::string mEntrypoint;

			std::string mSource;
			Core::Path mPath;

			std::vector<std::string> mDefines;
		};

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

			bool BuildShaderAsset(Assets::Shader* shader, Assets::ShaderBuildDesc& desc);

			Assets::ShaderBuildDesc ParseShaderAsset(const std::string& source);

			ShadersReflection ReflectShaderAsset(Assets::ShaderBuildDesc& desc);


			bool CreateShaderAssetPipeline();

			void CreateShader(IShader** result, const ShaderCreationDesc& desc);


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