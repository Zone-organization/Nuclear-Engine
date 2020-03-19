#include <Engine\Graphics\RenderingPipelines\RenderingPipeline.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/ShaderResourceBinding.h>
#include <Engine/Assets/DefaultTextures.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		IPipelineState* RenderingPipeline::GetPipeline()
		{
			return mPipeline.RawPtr();
		}

		Assets::TextureUsageType ParseTexUsageFromName(std::string& name)
		{
			if (name.find("Diffuse") == 0)
				return Assets::TextureUsageType::Diffuse;
			else if (name.find("Albedo") == 0)
				return Assets::TextureUsageType::Albedo;
			else if (name.find("Specular") == 0)
				return Assets::TextureUsageType::Specular;
			else if (name.find("Metallic") == 0)
				return Assets::TextureUsageType::Metallic;
			else if (name.find("Normal") == 0)
				return Assets::TextureUsageType::Normal;
			else if (name.find("Roughness") == 0)
				return Assets::TextureUsageType::Roughness;
			else if (name.find("AO") == 0)
				return Assets::TextureUsageType::AO;
			return Assets::TextureUsageType::Unknown;
		}

		void RenderingPipeline::ReflectPixelShaderData()
		{
			RefCntAutoPtr<IShaderResourceBinding> TempSRB;

			mPipeline->CreateShaderResourceBinding(&TempSRB, true);

			for (Uint32 i = 0; i < TempSRB->GetVariableCount(SHADER_TYPE_PIXEL); i++)
			{
				auto variable = TempSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, i);
				ShaderResourceDesc VarDesc;
				variable->GetResourceDesc(VarDesc);
				std::string VarName(VarDesc.Name);
				auto VarType = VarDesc.Type;
				if(VarType == SHADER_RESOURCE_TYPE_TEXTURE_SRV && VarName.find("NEMat_") == 0)
				{
					VarName.erase(0, 6);
					Assets::ShaderTexture ReflectedTex;
					ReflectedTex.mTex = Assets::DefaultTextures::DefaultBlackTex;
					ReflectedTex.mTex.SetName(VarName);
					ReflectedTex.mSlot = i;
					ReflectedTex.mTex.SetUsageType(ParseTexUsageFromName(VarName));
					mPixelShaderTextureInfo.push_back(ReflectedTex);
				}
			}
		}
		Uint32 RenderingPipeline::GetID()
		{
			return mID;
		}
		Assets::Texture RenderingPipeline::GetDefaultTextureFromType(Uint8 Type)
		{
			//TODO: Improve

			switch (Type)
			{
			case Assets::TextureUsageType::Diffuse:
				Assets::DefaultTextures::DefaultDiffuseTex;
			case Assets::TextureUsageType::Specular:
				Assets::DefaultTextures::DefaultSpecularTex;
			case Assets::TextureUsageType::Normal:
				Assets::DefaultTextures::DefaultNormalTex;
			default:
				break;
			}
			return Assets::DefaultTextures::DefaultBlackTex;
		}
		BakeStatus RenderingPipeline::GetStatus()
		{
			return mStatus;
		}
	}
}