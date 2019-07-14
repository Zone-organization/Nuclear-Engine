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

			void SetColor(Graphics::Color color);

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