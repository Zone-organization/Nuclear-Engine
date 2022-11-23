#include <Engine\Graphics\ShaderPipelineVariant.h>
#include <Engine\Assets\Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <Engine/Managers/AssetManager.h>

namespace Nuclear
{
	namespace Graphics
	{
		static Uint32 gRenderQueue = 1;
		ShaderPipelineVariant::ShaderPipelineVariant()
		{
			pParent = nullptr;
			mRenderQueue = gRenderQueue;
			gRenderQueue++;
		}

		IPipelineState* ShaderPipelineVariant::GetRenderingPipeline()
		{
			if (mDesc._isDeffered)
			{
				return GetGBufferPipeline();
			}
			return GetMainPipeline();
		}
		IShaderResourceBinding* ShaderPipelineVariant::GetRenderingSRB()
		{
			if (mDesc._isDeffered)
			{
				return GetGBufferPipelineSRB();
			}
			return GetMainPipelineSRB();
		}
		Graphics::ShaderPipeline* ShaderPipelineVariant::GetParentPipeline()
		{
			return pParent;
		}

		IPipelineState* ShaderPipelineVariant::GetMainPipeline()
		{
			return mPipeline.RawPtr();
		}

		IShaderResourceBinding* ShaderPipelineVariant::GetMainPipelineSRB()
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
		Uint32 ShaderPipelineVariant::GetShaderID()
		{
			return mShaderAssetID;
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

		const std::string& ShaderPipelineVariant::GetName() const
		{
			return mName;
		}

		Graphics::ShaderVariantReflection& ShaderPipelineVariant::GetReflection()
		{
			return mReflection;
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
	}
}