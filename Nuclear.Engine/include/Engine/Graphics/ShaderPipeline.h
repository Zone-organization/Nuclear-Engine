#pragma once
#include <Core\NE_Common.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Diligent/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <unordered_map>
#include <Core\Path.h>
#include <Engine/Graphics/ShaderPipelineVariant.h>
#include <Engine/Graphics/ShaderPipelineSwitch.h>

namespace Nuclear
{
	namespace Graphics
	{
		struct ShaderObjectCreationDesc
		{
			std::string mName;
			SHADER_TYPE mType;
			std::string mEntrypoint = "main";

			std::string mSource;
			Core::Path mPath;

			std::vector<std::string> mDefines;
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
			std::string mName= "";

			ShaderObjectCreationDesc mVertexShader = ShaderObjectCreationDesc();
			ShaderObjectCreationDesc mPixelShader = ShaderObjectCreationDesc();

			ShaderPSODesc mForwardPSOCreateInfo = ShaderPSODesc();
			ShaderPSODesc mDefferedPSOCreateInfo = ShaderPSODesc();
			ShaderPSODesc mGBufferPSOCreateInfo = ShaderPSODesc();

			ShaderPipelineVariantDesc mInstanceDesc= ShaderPipelineVariantDesc();

			std::vector<ShaderPipelineSwitch> Switches = std::vector<ShaderPipelineSwitch>();

			std::vector<std::string> mDefines = std::vector<std::string>();

			bool isRenderingPipeline = true;
			ShaderRenderingBakingDesc mBakingDesc;
		};

		class NEAPI ShaderPipeline
		{
		public:
			ShaderPipeline(Assets::Shader* parent = nullptr);
			~ShaderPipeline();

			void Create(const ShaderPipelineDesc& Desc);

			bool Bake(ShaderRenderingBakingDesc* bakingdesc);

			Uint32 GetHashedKey(const std::string& Key);
			ShaderPipelineVariant* GetVariant(Uint32 Key);

			const std::vector<ShaderPipelineSwitch>& GetSwitches() const;

		protected:
			Assets::Shader* mParentAsset;
			std::unordered_map<Uint32, ShaderPipelineVariant> mVariants;
			std::vector<ShaderPipelineVariantDesc> mVariantsInfo;

			std::vector<ShaderPipelineSwitch> mSwitches;

			ShaderPipelineDesc mDesc;

			ShaderPipelineVariant CreateForwardVariant(const ShaderPipelineVariantDesc& variantdesc, const ShaderPipelineDesc& pipelinedesc);
			ShaderPipelineVariant CreateDefferedVariant(const ShaderPipelineVariantDesc& variantdesc, const ShaderPipelineDesc& pipelinedesc);

		private:
			void ReflectPixelShaderData(ShaderPipelineVariant& pipeline);
		};
	}
}