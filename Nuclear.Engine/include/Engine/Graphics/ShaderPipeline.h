#pragma once
#include <Core\NE_Common.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Diligent/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <unordered_map>
#include <Core\Path.h>
#include <Engine/Graphics/ShaderPipelineVariant.h>

namespace Nuclear
{
	namespace Graphics
	{
		struct ShaderObjectCreationDesc
		{
			std::string mName;
			SHADER_TYPE mType;
			std::string mEntrypoint;

			std::string mSource;
			Core::Path mPath;

			std::vector<std::string> mDefines;
		};

		struct ShaderPipelineSwitch
		{
			ShaderPipelineSwitch(std::string KeyName_)
			{

			}
			std::string KeyName;

			//AUTO FILLED ON CREATING ShaderPipeline
			Uint32 mHashedName = 0;
		};
		struct ShaderRenderingBakingDesc
		{
			Uint32 DirLights = 0;
			Uint32 SpotLights = 0;
			Uint32 PointLights = 0;

			Rendering::ShadowPass* pShadowPass = nullptr;

			IBuffer* CameraBufferPtr = nullptr;
			IBuffer* AnimationBufferPtr = nullptr;
			IBuffer* LightsBufferPtr = nullptr;
		};

		struct ShaderPipelineDesc
		{
			std::string mName;

			ShaderObjectCreationDesc mVertexShader;
			ShaderObjectCreationDesc mPixelShader;

			ShaderPSODesc mForwardPSOCreateInfo;
			ShaderPSODesc mDefferedPSOCreateInfo;
			ShaderPSODesc mGBufferPSOCreateInfo;

			ShaderPipelineVariantDesc mInstanceDesc;

			std::vector<ShaderPipelineSwitch> Switches;

			std::vector<std::string> mDefines;

			bool isRenderingPipeline = true;
			ShaderRenderingBakingDesc mBakingDesc;

			//Increases memory cosumptions as it stores KeyChain information
			bool SaveKeyChainsInfo = DEBUG_TRUE_BOOL;
		};



		class NEAPI ShaderPipeline
		{
		public:
			ShaderPipeline(Assets::Shader* parent = nullptr);
			~ShaderPipeline();

			void Create(const ShaderPipelineDesc& Desc);

			bool Bake(ShaderRenderingBakingDesc* bakingdesc);

			Uint32 GetHashedKey(const std::string& Key);
			ShaderPipelineVariant GetVariant(Uint32 Key);
		protected:
			Assets::Shader* mParentAsset;
			std::vector<ShaderPipelineSwitch> mKeyChain;
			std::unordered_map<Uint32, ShaderPipelineVariant> mVariants;
			std::vector<ShaderPipelineVariantDesc> mVariantsInfo;
			ShaderPipelineDesc mDesc;

			ShaderPipelineVariant CreateForwardVariant(const ShaderPipelineVariantDesc& variantdesc, const ShaderPipelineDesc& pipelinedesc);
			ShaderPipelineVariant CreateDefferedVariant(const ShaderPipelineVariantDesc& variantdesc, const ShaderPipelineDesc& pipelinedesc);

		private:
			void ReflectPixelShaderData(ShaderPipelineVariant& pipeline);
		};
	}
}