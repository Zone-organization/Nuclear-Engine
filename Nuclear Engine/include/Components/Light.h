#pragma once
#include <NE_Common.h>
#include <API\Color.h>
#include <Math\Math.h>

namespace NuclearEngine {

	namespace Components {

		namespace Internal {

			struct NEAPI Shader_Light_Struct
			{
				Math::Vector4 Position;
				Math::Vector4 Direction;
				Math::Vector4 Intensity_Attenuation;
				Math::Vector4 InnerCutOf_OuterCutoff;
				Math::Vector4 Color;
			};
		}

		struct NEAPI Light
		{
		public:
			enum Type
			{
				DirectionalLight = 0,
				PointLight = 1,
				SpotLight = 2
			};

			Light(Type type);
			~Light();

			void SetType(Type type);

			void SetColor(API::Color color);

			void SetPosition(Math::Vector3 pos);

			void SetDirection(Math::Vector3 dir);

			void SetIntensity(float intensity);

			void SetAttenuation(Math::Vector3 att);

			void SetSpotlightCone(Math::Vector2 cutoff_outercutoff);

			Internal::Shader_Light_Struct GetInternalData();

		protected:
			Internal::Shader_Light_Struct data;
		};
	}
}