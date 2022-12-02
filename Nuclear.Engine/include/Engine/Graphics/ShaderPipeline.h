#pragma once
#include <Core\NE_Common.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Diligent/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <unordered_map>
#include <Core\Path.h>
#include <Engine/Graphics/ShaderPipelineVariant.h>
#include <Engine/Graphics/ShaderPipelineSwitch.h>
#include "Engine/Rendering/GBuffer.h"

namespace Nuclear
{
	namespace Rendering
	{
		class ImageBasedLighting;
	}
	namespace Graphics
	{				
		struct ShaderRenderingBakingDesc
		{
			Uint32 mRTWidth = 800;
			Uint32 mRTHeight = 600;

			Uint32 DirLights = 0;
			Uint32 SpotLights = 0;
			Uint32 PointLights = 0;

			Rendering::ShadowPass* pShadowPass = nullptr;
			Rendering::ImageBasedLighting* pIBLContext = nullptr;
			IBuffer* CameraBufferPtr = nullptr;
			IBuffer* AnimationBufferPtr = nullptr;
			IBuffer* LightsBufferPtr = nullptr;

			bool mRenderSystemHasDefferedPass = false;
			bool mRenderSystemHasShadowPass = false;
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

			bool isDeffered = false;
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
			Rendering::GBuffer* GetGBuffer();

			//bool GetAlwaysRequestDeffered();
		protected:
			Assets::Shader* mParentAsset;
			std::unordered_map<Uint32, ShaderPipelineVariant> mVariants;
			std::vector<ShaderPipelineVariantDesc> mVariantsInfo;

			ShaderPipelineDesc mDesc;
			Graphics::ShaderReflection mReflection;
			Rendering::GBuffer mGBuffer;

			ShaderPipelineVariant CreateForwardVariant(ShaderPipelineVariantDesc& variantdesc, ShaderPipelineDesc& pipelinedesc);
			ShaderPipelineVariant CreateDefferedVariant(ShaderPipelineVariantDesc& variantdesc, ShaderPipelineDesc& pipelinedesc);

			//bool mAlwaysRequestDeffered = true;
		private:
			void ReflectShaderPipelineVariant(ShaderPipelineVariant& pipeline, ShaderRenderingBakingDesc* pBakingDesc);
			bool mFirstReflection = true;
		};
	}
}