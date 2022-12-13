#include <Graphics\ShaderPipelineVariant.h>
#include <Assets\Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <Importers/AssetsImporter.h>

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
				Importers::AssetsImporter::GetInstance().DefaultDiffuseTex;
			case Graphics::TextureUsageType::Specular:
				Importers::AssetsImporter::GetInstance().DefaultSpecularTex;
			case Graphics::TextureUsageType::Normal:
				Importers::AssetsImporter::GetInstance().DefaultNormalTex;
			default:
				break;
			}
			return Importers::AssetsImporter::GetInstance().DefaultBlackTex;
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