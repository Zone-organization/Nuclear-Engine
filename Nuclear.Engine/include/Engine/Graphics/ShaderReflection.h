#pragma once
#include <Engine/Assets/MaterialTypes.h>

namespace Nuclear
{
	namespace Graphics
	{

		struct ShaderShadowMapsInfo
		{
			Assets::ShaderTexture mDirPos_SMInfo;         //Texture2DArray
			Assets::ShaderTexture mSpot_SMInfo;         //Texture2DArray
			Assets::ShaderTexture mOmniDir_SMInfo;        //TextureCubeArray
		};
		class NEAPI ShaderReflection
		{
		public:

			//This can be filled automatically by ReflectPixelShaderData(), Or fill it manually
			//Note: It is very important to fill it in order for material creation work with the pipeline.
			std::vector<Assets::ShaderTexture> mMaterialTexturesInfo;
			std::vector<Assets::ShaderTexture> mIBLTexturesInfo;
			ShaderShadowMapsInfo mShadowMapsInfo;

		};

		class NEAPI ShaderVariantReflection
		{
		public:
		};
	}
}