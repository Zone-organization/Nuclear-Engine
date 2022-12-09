#pragma once
#include <Assets/MaterialTypes.h>

namespace Nuclear
{
	namespace Graphics
	{

		struct ShaderShadowMapsInfo
		{
			Assets::ShaderTexture mDirPos_SMInfo = Assets::ShaderTexture();         //Texture2DArray
			Assets::ShaderTexture mSpot_SMInfo = Assets::ShaderTexture();        //Texture2DArray
			Assets::ShaderTexture mOmniDir_SMInfo = Assets::ShaderTexture();        //TextureCubeArray
		};

		//Should be constant for all variants
		struct ShaderReflection
		{
			bool mHasDefferedPipelines = false;
			//TODO
			bool mAllPipelinesAreDeffered = false;
			std::vector<Assets::ShaderTexture> mMaterialTexturesInfo = std::vector<Assets::ShaderTexture>();
		};

		//can change from a variant to the other
		struct ShaderVariantReflection
		{
			ShaderShadowMapsInfo mShadowMapsInfo = ShaderShadowMapsInfo();
			std::vector<Assets::ShaderTexture> mIBLTexturesInfo = std::vector<Assets::ShaderTexture>();
		};
	}
}