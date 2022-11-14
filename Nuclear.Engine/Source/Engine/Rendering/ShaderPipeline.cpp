#include <Engine\Rendering\ShaderPipeline.h>
#include <Engine\Assets\Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <Engine/Managers/AssetManager.h>

namespace Nuclear
{
	namespace Rendering
	{
		static Uint32 gRenderQueue = 0;
		ShaderPipeline::ShaderPipeline()
		{
			mRenderQueue = gRenderQueue;
			gRenderQueue++;
		}
		void ShaderPipeline::Create(Assets::Shader* shader, const ShaderPipelineDesc& desc)
		{
		}
		IPipelineState* ShaderPipeline::GetRenderingPipeline()
		{
			if (mDesc._isDeffered)
			{
				return GetGBufferPipeline();
			}
			return GetShadersPipeline();
		}
		IShaderResourceBinding* ShaderPipeline::GetRenderingSRB()
		{
			if (mDesc._isDeffered)
			{
				return GetGBufferPipelineSRB();
			}
			return GetShadersPipelineSRB();
		}
		Assets::Shader* ShaderPipeline::GetShaderAsset()
		{
			return pShader;
		}
		Uint32 ShaderPipeline::GetShaderAssetID()
		{
			return mShaderID;
		}
		IPipelineState* ShaderPipeline::GetShadersPipeline()
		{
			return mPipeline.RawPtr();
		}

		IShaderResourceBinding* ShaderPipeline::GetShadersPipelineSRB()
		{
			return mPipelineSRB.RawPtr();
		}

		IPipelineState* ShaderPipeline::GetGBufferPipeline()
		{
			return mGBufferPipeline.RawPtr();
		}

		IShaderResourceBinding* ShaderPipeline::GetGBufferPipelineSRB()
		{
			return mGBufferSRB.RawPtr();
		}

		Uint32 ShaderPipeline::GetRenderQueue()
		{
			return mRenderQueue;
		}
		Graphics::Texture ShaderPipeline::GetDefaultTextureFromType(Uint8 Type)
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
		Graphics::BakeStatus ShaderPipeline::GetStatus()
		{
			return mStatus;
		}
		std::unordered_map<Uint32, ShaderEffect>& ShaderPipeline::GetRenderingEffects()
		{
			return mRenderingEffects;
		}
		void ShaderPipeline::SetEffect(const Uint32& effectId, bool value)
		{
			auto it = mRenderingEffects.find(effectId);
			if (it != mRenderingEffects.end())
			{
				it->second.SetValue(value);
			}
			else
			{
				assert(false);
			}
		}
		bool ShaderPipeline::isValid()
		{
			return _isValid;
		}
		bool ShaderPipeline::isSkinned()
		{
			return mDesc._isSkinned;
		}
		bool ShaderPipeline::isDeffered()
		{
			return mDesc._isDeffered;
		}

		std::vector<Graphics::RenderTargetDesc> ShaderPipeline::GetGBufferDesc()
		{
			return std::vector<Graphics::RenderTargetDesc>();
		}
		std::string ShaderPipeline::GetName()
		{
			return mName;
		}

		void ShaderPipeline::BakeGBufferRTs(Uint32 Width, Uint32 Height)
		{
			mGBuffer.Bake(Width, Height);
		}

		void ShaderPipeline::AddToDefinesIfNotZero(std::vector<std::string>& defines, const std::string& name, Uint32 value)
		{
			if (value > 0) { defines.push_back(name + std::to_string(value)); }
		}
	}
}