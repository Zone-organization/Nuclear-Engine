#pragma once
#include <NE_Common.h>
#include <Assets/MaterialTypes.h>
#include <Graphics/BakeStatus.h>
#include <Graphics/ShaderPipelineSwitch.h>
#include <Graphics/ShaderReflection.h>
#include <Graphics/ShaderTypes.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Rendering/RenderPasses/ShadowPass.h>
#include <set>

namespace Nuclear
{
	namespace Assets {
		class Shader;
	}

	namespace Graphics
	{

		//Used for both Deffered and Forward pipelines
		//Should provide GBuffer Pipeline Implementation.
		struct ShaderPipelineVariant
		{
			Diligent::RefCntAutoPtr<Diligent::IPipelineState> mPipeline;
			Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> mPipelineSRB;

			Diligent::RefCntAutoPtr<Diligent::IPipelineState> mGBufferPipeline;
			Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> mGBufferSRB;

			ShaderPipelineVariantDesc mDesc = ShaderPipelineVariantDesc();
			ShaderVariantReflection mReflection = ShaderVariantReflection();

			Graphics::BakeStatus mStatus = Graphics::BakeStatus::NotInitalized;

			Uint32 mRenderQueue = 0;
			Uint32 mShaderAssetID = 0;
			std::string mName = "";

			Graphics::ShaderPipeline* pParent = nullptr;

			bool isValid = false;

			FORCE_INLINE Diligent::IPipelineState* GetRenderingPipeline()
			{
				if (mDesc.isDeffered)
				{
					return mGBufferPipeline.RawPtr();
				}
				return mPipeline.RawPtr();
			}
			FORCE_INLINE Diligent::IShaderResourceBinding* GetRenderingSRB()
			{
				if (mDesc.isDeffered)
				{
					return mGBufferSRB.RawPtr();
				}
				return mPipelineSRB.RawPtr();
			}
		};

	}
}