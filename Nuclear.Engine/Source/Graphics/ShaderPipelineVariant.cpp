#include <Graphics\ShaderPipelineVariant.h>
#include <Assets\Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <Fallbacks/FallbacksEngine.h>

namespace Nuclear
{
	namespace Graphics
	{
		static Uint32 gRenderQueue = 1;
		ShaderPipelineVariant::ShaderPipelineVariant()
		{
			mName = "UnNamed";
			pParent = nullptr;
			mRenderQueue = gRenderQueue;
			gRenderQueue++;
		}

		Diligent::IPipelineState* ShaderPipelineVariant::GetRenderingPipeline()
		{
			if (mDesc._isDeffered)
			{
				return GetGBufferPipeline();
			}
			return GetMainPipeline();
		}
		Diligent::IShaderResourceBinding* ShaderPipelineVariant::GetRenderingSRB()
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

		Diligent::IPipelineState* ShaderPipelineVariant::GetMainPipeline()
		{
			return mPipeline.RawPtr();
		}

		Diligent::IShaderResourceBinding* ShaderPipelineVariant::GetMainPipelineSRB()
		{
			return mPipelineSRB.RawPtr();
		}

		Diligent::IPipelineState* ShaderPipelineVariant::GetGBufferPipeline()
		{
			return mGBufferPipeline.RawPtr();
		}

		Diligent::IShaderResourceBinding* ShaderPipelineVariant::GetGBufferPipelineSRB()
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
		Assets::MaterialTexture ShaderPipelineVariant::GetDefaultTextureFromType(Uint8 Type)
		{
			//TODO: Improve

			switch (Type)
			{
			case Assets::TextureUsageType::Diffuse:
				return Fallbacks::FallbacksEngine::GetInstance().GetDefaultDiffuseTex();
			case Assets::TextureUsageType::Specular:
				return Fallbacks::FallbacksEngine::GetInstance().GetDefaultSpecularTex();
			case Assets::TextureUsageType::Normal:
				return Fallbacks::FallbacksEngine::GetInstance().GetDefaultNormalTex();
			default:
				break;
			}
			return Fallbacks::FallbacksEngine::GetInstance().GetDefaultDiffuseTex();
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
		bool ShaderPipelineVariant::isShadowed()
		{
			return mDesc._isShadowed;
		}
	}
}