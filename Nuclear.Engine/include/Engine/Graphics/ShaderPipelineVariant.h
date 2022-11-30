#pragma once
#include <Core\NE_Common.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Graphics/BakeStatus.h>
#include <Engine/Graphics/ShaderPipelineSwitch.h>
#include <Engine/Graphics/ShaderReflection.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <Engine/Rendering/RenderPasses/ShadowPass.h>
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
		enum class ShaderPSOType {
			ForwardPipeline,
			DefferedPipeline,
			GBufferPipeline,
			Unknown
		};

		struct ShaderObjectCreationDesc
		{
			std::string mName ="";
			SHADER_TYPE mType = SHADER_TYPE_UNKNOWN;
			std::string mEntrypoint = "main";

			std::string mSource = "";
			Core::Path mPath = "";

			std::set<std::string> mDefines = std::set<std::string>();
		};

		struct ShaderPSODesc 
		{
			ShaderObjectCreationDesc mVertexShader = ShaderObjectCreationDesc();
			ShaderObjectCreationDesc mPixelShader = ShaderObjectCreationDesc();
			ShaderPSOType mType = ShaderPSOType::Unknown;
			GraphicsPipelineDesc GraphicsPipeline = GraphicsPipelineDesc();
		};		

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
			IPipelineState* GetRenderingPipeline();
			IShaderResourceBinding* GetRenderingSRB();

			Graphics::ShaderPipeline* GetParentPipeline();

			IPipelineState* GetMainPipeline();
			IShaderResourceBinding* GetMainPipelineSRB();

			IPipelineState* GetGBufferPipeline();
			IShaderResourceBinding* GetGBufferPipelineSRB();

			Uint32 GetRenderQueue();

			Uint32 GetShaderID();
			//TODO ??
			Graphics::Texture GetDefaultTextureFromType(Uint8 Type);

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
			RefCntAutoPtr<IPipelineState> mPipeline;
			RefCntAutoPtr<IShaderResourceBinding> mPipelineSRB;

			RefCntAutoPtr<IPipelineState> mGBufferPipeline;
			RefCntAutoPtr<IShaderResourceBinding> mGBufferSRB;

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