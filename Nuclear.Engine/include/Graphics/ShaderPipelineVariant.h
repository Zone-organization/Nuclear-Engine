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
	namespace Systems
	{
		class DebugSystem;
	}
	namespace Graphics
	{
		struct ShaderPipelineVariantDesc
		{    
			ShaderPSODesc mMainPSOCreateInfo = ShaderPSODesc();
			ShaderPSODesc mGBufferPSOCreateInfo = ShaderPSODesc();

			bool _isDeffered = false;
			bool _isSkinned = false;
			bool _isShadowed = false;

			std::set<std::string> mDefines = std::set<std::string>();
			Uint32 mHashKey = 0;
		};

		//Used for both Deffered and Forward pipelines
		//Should provide GBuffer Pipeline Implementation.
		class NEAPI ShaderPipelineVariant
		{
		public:
			ShaderPipelineVariant();

			//Returns the main pipeline used for rendering
			Diligent::IPipelineState* GetRenderingPipeline();
			Diligent::IShaderResourceBinding* GetRenderingSRB();

			Graphics::ShaderPipeline* GetParentPipeline();

			Diligent::IPipelineState* GetMainPipeline();
			Diligent::IShaderResourceBinding* GetMainPipelineSRB();

			Diligent::IPipelineState* GetGBufferPipeline();
			Diligent::IShaderResourceBinding* GetGBufferPipelineSRB();

			Uint32 GetRenderQueue();

			Uint32 GetShaderID();
			//TODO ??
			Assets::MaterialTexture GetDefaultTextureFromType(Uint8 Type);

			//Graphics::BakeStatus GetStatus();

			//std::vector<Graphics::RenderTargetDesc> GetGBufferDesc();

			const std::string& GetName() const;
			Graphics::ShaderVariantReflection& GetReflection();

			bool mAutoBake = true;

			bool isValid();

			bool isSkinned();

			bool isDeffered();

			bool isShadowed();

		protected:
			friend class ShaderPipeline;
			friend class ShaderManager;
			friend class Systems::DebugSystem;
			Diligent::RefCntAutoPtr<Diligent::IPipelineState> mPipeline;
			Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> mPipelineSRB;

			Diligent::RefCntAutoPtr<Diligent::IPipelineState> mGBufferPipeline;
			Diligent::RefCntAutoPtr<Diligent::IShaderResourceBinding> mGBufferSRB;

			ShaderPipelineVariantDesc mDesc;
			ShaderVariantReflection mReflection;

			Graphics::BakeStatus mStatus = Graphics::BakeStatus::NotInitalized;

			Uint32 mRenderQueue = 1;
			Uint32 mShaderAssetID = 0;
			std::string mName;

			Graphics::ShaderPipeline* pParent;

			bool _isValid = false;
		};

	}
}