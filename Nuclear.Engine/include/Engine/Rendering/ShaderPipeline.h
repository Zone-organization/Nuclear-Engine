#pragma once
#include <Core\NE_Common.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Graphics/BakeStatus.h>
#include <Engine/Rendering/ShaderEffect.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include "Engine/Rendering/GBuffer.h"
#include <Engine/Rendering/RenderPasses/ShadowPass.h>

namespace Nuclear
{
	namespace Assets {
		class Shader;
	}
	namespace Rendering
	{
		struct ShaderPipelineBakingDesc
		{
			Uint32 DirLights = 0;
			Uint32 SpotLights = 0;
			Uint32 PointLights = 0;

			ShadowPass* pShadowPass = nullptr;

			IBuffer* CameraBufferPtr = nullptr;
			IBuffer* AnimationBufferPtr = nullptr;
			IBuffer* LightsBufferPtr = nullptr;

			std::vector<ShaderEffect> mRequiredEffects;
		};

		enum class ShaderPipelineType {
			ForwardPipeline,
			DefferedPipeline,
			GBufferPipeline,
			Unknown
		};
		struct ShaderPSODesc 
		{
			ShaderPipelineType mType;

			GraphicsPipelineDesc GraphicsPipeline;

			RefCntAutoPtr<IShader> pVS;

			RefCntAutoPtr<IShader> pPS;

		};		

		struct ShaderPipelineDesc
		{    
			ShaderPSODesc mMainPSOCreateInfo; 
			ShaderPSODesc mGBufferPSOCreateInfo;

			bool _isDeffered = false;

			bool _isSkinned = false;

			bool _SupportShadows = false;
		};

		//Used for both Deffered and Forward pipelines
		//Should provide GBuffer Pipeline Implementation.
		class NEAPI ShaderPipeline
		{
		public:
			ShaderPipeline();

			virtual bool Bake(const ShaderPipelineBakingDesc& desc);

			void Create(Assets::Shader* shader, const ShaderPipelineDesc& desc);


			//Returns the main pipeline used for rendering
			IPipelineState* GetRenderingPipeline();
			IShaderResourceBinding* GetRenderingSRB();

			Assets::Shader* GetShaderAsset();
			Uint32 GetShaderAssetID();

			IPipelineState* GetShadersPipeline();
			IShaderResourceBinding* GetShadersPipelineSRB();

			IPipelineState* GetGBufferPipeline();
			IShaderResourceBinding* GetGBufferPipelineSRB();

			Uint32 GetRenderQueue();

			virtual Graphics::Texture GetDefaultTextureFromType(Uint8 Type);

			virtual Graphics::BakeStatus GetStatus();

			std::unordered_map<Uint32, ShaderEffect>& GetRenderingEffects();

			void SetEffect(const Uint32& effectId, bool value);

			virtual std::vector<Graphics::RenderTargetDesc> GetGBufferDesc();

			std::string GetName();

			bool mAutoBake = true;

			bool isValid();

			bool isSkinned();

			bool isDeffered();


			GBuffer mGBuffer;
			virtual void BakeGBufferRTs(Uint32 Width, Uint32 Height);

		protected:
			std::unordered_map<Uint32, ShaderEffect> mRenderingEffects;

			RefCntAutoPtr<IPipelineState> mPipeline;
			RefCntAutoPtr<IShaderResourceBinding> mPipelineSRB;

			RefCntAutoPtr<IPipelineState> mGBufferPipeline;
			RefCntAutoPtr<IShaderResourceBinding> mGBufferSRB;

			ShaderPipelineDesc mDesc;

			Graphics::BakeStatus mStatus = Graphics::BakeStatus::NotInitalized;

			Uint32 mRenderQueue = -1;

			std::string mName;

			Assets::Shader* pShader;
			Uint32 mShaderID;

			bool _isValid = false;

			//helper function
			void AddToDefinesIfNotZero(std::vector<std::string>& defines, const std::string& name, Uint32 value);
		};

	}
}