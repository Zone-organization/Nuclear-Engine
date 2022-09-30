#pragma once
#include <Core/Math/Math.h>
#include <Core/NE_Common.h>

namespace Nuclear
{
	namespace Components
	{

		enum class LightShadowType
		{
			No_Shadows,                        //Disables baking of shadow map for a light
			Simple_Shadows                     //Dir light no CSM
		};


		//Shader Structs
		namespace Internal
		{
			struct NEAPI Shader_SpotLight_Struct
			{
				Math::Vector4 Position;
				Math::Vector4 Direction;
				Math::Vector4 Intensity_Attenuation;
				Math::Vector4 InnerCutOf_OuterCutoff;
				Math::Vector4 Color;
			};

			struct NEAPI Shader_PointLight_Struct
			{
				Math::Vector4 Position;
				Math::Vector4 Intensity_Attenuation;
				Math::Vector4 Color_FarPlane;
			};

			struct NEAPI Shader_DirLight_Struct
			{
				Math::Vector4 Direction;
				Math::Vector4 Color_Intensity;

			};
		}
	}
}