#pragma once
#include <Core\NE_Common.h>
#include <Engine/Graphics/Texture.h>
#include <Engine/Graphics/BakeStatus.h>
#include <Engine/Graphics/ShaderPipelineSwitch.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include "Engine/Rendering/GBuffer.h"
#include <Engine/Rendering/RenderPasses/ShadowPass.h>
#include <set>

namespace Nuclear
{
	namespace Assets {
		class Shader;
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

			std::vector<std::string> mDefines = std::vector<std::string>();
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
			Uint32 mHashKey = -1;
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

			Assets::Shader* GetShaderAsset();
			Uint32 GetShaderAssetID();

			IPipelineState* GetShadersPipeline();
			IShaderResourceBinding* GetShadersPipelineSRB();

			IPipelineState* GetGBufferPipeline();
			IShaderResourceBinding* GetGBufferPipelineSRB();

			Uint32 GetRenderQueue();

			virtual Graphics::Texture GetDefaultTextureFromType(Uint8 Type);

			virtual Graphics::BakeStatus GetStatus();

			virtual std::vector<Graphics::RenderTargetDesc> GetGBufferDesc();

			std::string GetName();

			bool mAutoBake = true;

			bool isValid();

			bool isSkinned();

			bool isDeffered();

			//Move to main shader? its only RTs.
			Rendering::GBuffer mGBuffer;
			virtual void BakeGBufferRTs(Uint32 Width, Uint32 Height);

		protected:
			friend class ShaderPipeline;
			RefCntAutoPtr<IPipelineState> mPipeline;
			RefCntAutoPtr<IShaderResourceBinding> mPipelineSRB;

			RefCntAutoPtr<IPipelineState> mGBufferPipeline;
			RefCntAutoPtr<IShaderResourceBinding> mGBufferSRB;

			ShaderPipelineVariantDesc mDesc;

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