#pragma once
#include <Base\NE_Common.h>
#include <Engine/Graphics/Color.h>
#include <Base\Math\Math.h>
#include <Engine\ECS\Entity.h>

namespace NuclearEngine
{
	namespace Components
	{
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
		}

		struct NEAPI SpotLightComponent : public ECS::Component<SpotLightComponent>
		{
		public:
			SpotLightComponent();
			~SpotLightComponent();

			Graphics::Color GetColor();
			void SetColor(Graphics::Color color);

			Math::Vector3 GetPosition();
			void SetPosition(Math::Vector3 pos);

			Math::Vector3 GetDirection();
			void SetDirection(Math::Vector3 dir);

			float GetIntensity();
			void SetIntensity(float intensity);

			Math::Vector3 GetAttenuation();
			void SetAttenuation(Math::Vector3 att);

			Math::Vector2 GetSpotlightCone();
			void SetSpotlightCone(Math::Vector2 cutoff_outercutoff);

			Internal::Shader_SpotLight_Struct GetInternalData();
		protected:
			Internal::Shader_SpotLight_Struct data;
		};
	}
}