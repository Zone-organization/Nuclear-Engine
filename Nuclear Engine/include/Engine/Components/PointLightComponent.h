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
			struct NEAPI Shader_PointLight_Struct
			{
				Math::Vector4 Position;
				Math::Vector4 Intensity_Attenuation;
				Math::Vector4 Color;
			};
		}

		struct NEAPI PointLightComponent : public ECS::Component<PointLightComponent>
		{
		public:
			PointLightComponent();
			~PointLightComponent();

			Graphics::Color GetColor();
			void SetColor(Graphics::Color color);

			Math::Vector3 GetPosition();
			void SetPosition(Math::Vector3 pos);

			float GetIntensity();
			void SetIntensity(float intensity);

			Math::Vector3 GetAttenuation();
			void SetAttenuation(Math::Vector3 att);

			Internal::Shader_PointLight_Struct GetInternalData();

		protected:
			Internal::Shader_PointLight_Struct data;
		};

	}
}