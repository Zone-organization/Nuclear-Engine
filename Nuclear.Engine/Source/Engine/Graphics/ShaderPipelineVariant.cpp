#include <Engine\Graphics\ShaderPipelineVariant.h>
#include <Engine\Assets\Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <Engine/Managers/AssetManager.h>

namespace Nuclear
{
	namespace Graphics
	{
		static Uint32 gRenderQueue = 0;
		ShaderPipelineVariant::ShaderPipelineVariant()
		{
			mRenderQueue = gRenderQueue;
			gRenderQueue++;
		}

		IPipelineState* ShaderPipelineVariant::GetRenderingPipeline()
		{
			if (mDesc._isDeffered)
			{
				return GetGBufferPipeline();
			}
			return GetShadersPipeline();
		}
		IShaderResourceBinding* ShaderPipelineVariant::GetRenderingSRB()
		{
			if (mDesc._isDeffered)
			{
				return GetGBufferPipelineSRB();
			}
			return GetShadersPipelineSRB();
		}
		Assets::Shader* ShaderPipelineVariant::GetShaderAsset()
		{
			return pShader;
		}
		Uint32 ShaderPipelineVariant::GetShaderAssetID()
		{
			return mShaderID;
		}
		IPipelineState* ShaderPipelineVariant::GetShadersPipeline()
		{
			return mPipeline.RawPtr();
		}

		IShaderResourceBinding* ShaderPipelineVariant::GetShadersPipelineSRB()
		{
			return mPipelineSRB.RawPtr();
		}

		IPipelineState* ShaderPipelineVariant::GetGBufferPipeline()
		{
			return mGBufferPipeline.RawPtr();
		}

		IShaderResourceBinding* ShaderPipelineVariant::GetGBufferPipelineSRB()
		{
			return mGBufferSRB.RawPtr();
		}

		Uint32 ShaderPipelineVariant::GetRenderQueue()
		{
			return mRenderQueue;
		}
		Graphics::Texture ShaderPipelineVariant::GetDefaultTextureFromType(Uint8 Type)
		{
			//TODO: Improve

			switch (Type)
			{
			case Graphics::TextureUsageType::Diffuse:
				Managers::AssetManager::DefaultDiffuseTex;
			case Graphics::TextureUsageType::Specular:
				Managers::AssetManager::DefaultSpecularTex;
			case Graphics::TextureUsageType::Normal:
				Managers::AssetManager::DefaultNormalTex;
			default:
				break;
			}
			return Managers::AssetManager::DefaultBlackTex;
		}
		Graphics::BakeStatus ShaderPipelineVariant::GetStatus()
		{
			return mStatus;
		}

		bool ShaderPipelineVariant::isValid()
		{
			return _isValid;
		}
		bool ShaderPipelineVariant::isSkinned()
		{
			return mDesc._isSkinned;
		}
		bool ShaderPipelineVariant::isDeffered()
		{
			return mDesc._isDeffered;
		}

		std::vector<Graphics::RenderTargetDesc> ShaderPipelineVariant::GetGBufferDesc()
		{
			return std::vector<Graphics::RenderTargetDesc>();
		}
		std::string ShaderPipelineVariant::GetName()
		{
			return mName;
		}

		void ShaderPipelineVariant::BakeGBufferRTs(Uint32 Width, Uint32 Height)
		{
			mGBuffer.Bake(Width, Height);
		}

		void ShaderPipelineVariant::AddToDefinesIfNotZero(std::vector<std::string>& defines, const std::string& name, Uint32 value)
		{
			if (value > 0) { defines.push_back(name + std::to_string(value)); }
		}
	}
}