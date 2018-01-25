#pragma once
#include <NE_Common.h>
#include <API\Color.h>
#include <Math\Math.h>

namespace NuclearEngine {

	namespace Components {

		namespace Internal 
		{

			struct NEAPI Shader_DirLight_Struct
			{
				Math::Vector4 Direction;
				Math::Vector4 Color;
			};
			struct NEAPI Shader_PointLight_Struct
			{
				Math::Vector4 Position;
				Math::Vector4 Intensity_Attenuation;
				Math::Vector4 Color;
			};
			struct NEAPI Shader_SpotLight_Struct
			{
				Math::Vector4 Position;
				Math::Vector4 Direction;
				Math::Vector4 Intensity_Attenuation;
				Math::Vector4 InnerCutOf_OuterCutoff;
				Math::Vector4 Color;
			};
		}

		struct NEAPI DirectionalLight
		{
		public:
			DirectionalLight();
			~DirectionalLight();

			void SetColor(API::Color color);

			void SetDirection(Math::Vector3 dir);

			Internal::Shader_DirLight_Struct GetInternalData();

		protected:
			Internal::Shader_DirLight_Struct data;
		};

		struct NEAPI PointLight
		{
		public:
			PointLight();
			~PointLight();

			void SetColor(API::Color color);

			void SetPosition(Math::Vector3 pos);

			void SetIntensity(float intensity);

			void SetAttenuation(Math::Vector3 att);

			Internal::Shader_PointLight_Struct GetInternalData();

		protected:
			Internal::Shader_PointLight_Struct data;
		};

		struct NEAPI SpotLight
		{
		public:
			SpotLight();
			~SpotLight();

			void SetColor(API::Color color);

			void SetPosition(Math::Vector3 pos);

			void SetDirection(Math::Vector3 dir);

			void SetIntensity(float intensity);

			void SetAttenuation(Math::Vector3 att);

			void SetSpotlightCone(Math::Vector2 cutoff_outercutoff);

			Internal::Shader_SpotLight_Struct GetInternalData();
		protected:
			Internal::Shader_SpotLight_Struct data;
		};
	}
}