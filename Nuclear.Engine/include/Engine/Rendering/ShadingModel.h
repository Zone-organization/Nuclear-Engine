#pragma once
#include <Core\NE_Common.h>
#include <Engine/Assets/MaterialTypes.h>
#include <Engine/Graphics/BakeStatus.h>
#include <Engine/Rendering/ShaderEffect.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <vector>
#include <unordered_map>
#include "Engine/Rendering/GBuffer.h"

namespace Nuclear
{
	namespace Rendering
	{
		struct ShadingModelBakingDesc
		{
			Uint32 DirLights = 0;
			Uint32 SpotLights = 0;
			Uint32 PointLights = 0;

			Uint32 Max_DirLight_Caster = 0;
			Uint32 Max_SpotLight_Caster = 0;

			IBuffer* CameraBufferPtr = nullptr;
			IBuffer* AnimationBufferPtr = nullptr;
			IBuffer* LightsBufferPtr = nullptr;
			IBuffer* ShadowCastersBufferPtr = nullptr;

			std::vector<ShaderEffect> mRequiredEffects;
		};

		struct ShadingModelInitInfo {
			bool mDefferedPipeline = false;
			bool ShadowingEnabled = false;
		};

		//Used for both Deffered and Forward pipelines
		//Should provide GBuffer Pipeline Implementation.
		class NEAPI ShadingModel
		{
		public:
			virtual bool Bake(const ShadingModelBakingDesc& desc) = 0;

			virtual void Initialize(const ShadingModelInitInfo& info);

			IPipelineState* GetActivePipeline();
			IShaderResourceBinding* GetActiveSRB();

			IPipelineState* GetShadersPipeline();
			IShaderResourceBinding* GetShadersPipelineSRB();
			IPipelineState* GetGBufferPipeline();
			IShaderResourceBinding* GetGBufferPipelineSRB();

			virtual void ReflectPixelShaderData();

			Uint32 GetID();

			virtual Graphics::Texture GetDefaultTextureFromType(Uint8 Type);
			
			virtual Graphics::BakeStatus GetStatus();

			std::unordered_map<Uint32, ShaderEffect>& GetRenderingEffects();

			void SetEffect(const Uint32& effectId, bool value);

			bool isDeffered();

			virtual std::vector<Graphics::RenderTargetDesc> GetGBufferDesc();

			//This can be filled automatically by ReflectPixelShaderData(), Or fill it manually
			//Note: It is very important to fill it in order for material creation work with the pipeline.
			std::vector<Assets::ShaderTexture> mMaterialTexturesInfo;
			std::vector<Assets::ShaderTexture> mIBLTexturesInfo;

			Assets::ShaderTexture mSpotLight_ShadowmapInfo;         //Texture2DArray
			Assets::ShaderTexture mPointLight_ShadowmapInfo;        //TextureCubeArray
			Assets::ShaderTexture mDirLight_ShadowmapInfo;

			std::string GetName();
		protected:
			std::unordered_map<Uint32, ShaderEffect> mRenderingEffects;

			RefCntAutoPtr<IPipelineState> mPipeline;
			RefCntAutoPtr<IShaderResourceBinding> mPipelineSRB;
			RefCntAutoPtr<IPipelineState> mGBufferPipeline;
			RefCntAutoPtr<IShaderResourceBinding> mGBufferSRB;

			ShadingModelInitInfo mInitInfo;
			Graphics::BakeStatus mStatus = Graphics::BakeStatus::NotInitalized;
			Uint32 mID = 0;
			std::string mName;


		};

	}
}