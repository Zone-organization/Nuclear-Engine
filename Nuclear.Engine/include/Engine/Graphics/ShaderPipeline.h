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

			ShaderPSODesc mForwardPSOCreateInfo = ShaderPSODesc();
			ShaderPSODesc mDefferedPSOCreateInfo = ShaderPSODesc();
			ShaderPSODesc mGBufferPSOCreateInfo = ShaderPSODesc();

			ShaderPipelineVariantDesc mInstanceDesc= ShaderPipelineVariantDesc();

			std::vector<ShaderPipelineSwitch> Switches = std::vector<ShaderPipelineSwitch>();

			std::vector<std::string> mDefines = std::vector<std::string>();

			bool isRenderingPipeline = true;
			ShaderRenderingBakingDesc* pBakingDesc;
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
			const Graphics::ShaderReflection& GetReflection() const;

			Assets::Shader* GetShaderAsset();
		protected:
			Assets::Shader* mParentAsset;
			std::unordered_map<Uint32, ShaderPipelineVariant> mVariants;
			std::vector<ShaderPipelineVariantDesc> mVariantsInfo;

			std::vector<ShaderPipelineSwitch> mSwitches;

			ShaderPipelineDesc mDesc;
			Graphics::ShaderReflection mReflection;

			ShaderPipelineVariant CreateForwardVariant(ShaderPipelineVariantDesc& variantdesc, ShaderPipelineDesc& pipelinedesc);
			ShaderPipelineVariant CreateDefferedVariant(ShaderPipelineVariantDesc& variantdesc, ShaderPipelineDesc& pipelinedesc);

		private:
			void ReflectShaderPipelineVariant(ShaderPipelineVariant& pipeline);
			bool mFirstReflection = true;
		};
	}
}