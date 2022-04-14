#pragma once
#include <Core\NE_Common.h>
#include <Engine/Assets/MaterialTypes.h>
#include <Engine/Graphics/BakeStatus.h>
#include <Engine/Rendering/ShaderEffect.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <vector>

namespace Nuclear
{
	namespace Rendering
	{
		struct ShadingModelBakingDesc
		{
			Uint32 DirLights = 0;
			Uint32 SpotLights = 0;
			Uint32 PointLights = 0;

			IBuffer* CameraBufferPtr = nullptr;
			IBuffer* AnimationBufferPtr = nullptr;
			IBuffer* LightsBufferPtr = nullptr;

			std::vector<ShaderEffect> mRequiredEffects;
		};

		class NEAPI ShadingModel
		{
		public:
			virtual bool Bake(const ShadingModelBakingDesc& desc) = 0;

			IPipelineState* GetPipeline();
			virtual void ReflectPixelShaderData();

			Uint32 GetID();

			virtual Graphics::Texture GetDefaultTextureFromType(Uint8 Type);
			
			virtual Graphics::BakeStatus GetStatus();

			//This can be filled automatically by ReflectPixelShaderData(), Or fill it manually
			//Note: It is very important to fill it in order for material creation work with the pipeline.
			std::vector<Assets::ShaderTexture> mPixelShaderTextureInfo;

		protected:
			std::vector<ShaderEffect> mRenderingEffects;

			RefCntAutoPtr<IPipelineState> mPipeline;
			Graphics::BakeStatus mStatus = Graphics::BakeStatus::NotInitalized;
			Uint32 mID = 0;
		};

	}
}