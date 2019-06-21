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
			else if (name.find("Specular") == 0)
				return Assets::TextureUsageType::Specular;
			else if (name.find("Normal") == 0)
				return Assets::TextureUsageType::Normal;

			return Assets::TextureUsageType::Unknown;
		}

		void RenderingPipeline::ReflectPixelShaderData()
		{
			RefCntAutoPtr<IShaderResourceBinding> TempSRB;

			mPipeline->CreateShaderResourceBinding(&TempSRB, true);

			for (Uint32 i = 0; i < TempSRB->GetVariableCount(SHADER_TYPE_PIXEL); i++)
			{
				auto variable = TempSRB->GetVariableByIndex(SHADER_TYPE_PIXEL, i);
				std::string VarName(variable->GetResourceDesc().Name);
				auto VarType = variable->GetResourceDesc().Type;
				if(VarType == SHADER_RESOURCE_TYPE_TEXTURE_SRV && VarName.find("NEMat_") == 0)
				{
					VarName.erase(0, 6);
					ShaderTextureInfo ReflectedTex;
					ReflectedTex.mTexture = Assets::DefaultTextures::DefaultBlackTex;
					ReflectedTex.mTexture.SetName(VarName);
					ReflectedTex.mSlot = i;
					ReflectedTex.mTexture.SetUsageType(ParseTexUsageFromName(VarName));
					mPixelShaderTextureInfo.push_back(ReflectedTex);
				}
			}
		}
	}
}