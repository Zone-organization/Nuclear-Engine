#include "Engine\Components\SpotLightComponent.h"

namespace NuclearEngine
{
	namespace Components
	{
		SpotLightComponent::SpotLightComponent()
		{
			data.Position = Math::Vector4(0.5f, 0.5f, 0.5f, 1.0f);
			data.Direction = Math::Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
			data.Intensity_Attenuation = Math::Vector4(1.0f, 1.0f, 0.09f, 0.032f);
			data.InnerCutOf_OuterCutoff = Math::Vector4(cos(Math::radians(12.5f)), cos(Math::radians(15.0f)), 1.0f, 1.0f);
			data.Color = Math::Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		}
		SpotLightComponent::~SpotLightComponent()
		{
		}
		void SpotLightComponent::SetColor(Graphics::Color color)
		{
			data.Color = Math::Vector4(color.r, color.g, color.b, 1.0f);
		}
		void SpotLightComponent::SetPosition(Math::Vector3 pos)
		{
			data.Position = Math::Vector4(pos, 1.0f);
		}
		void SpotLightComponent::SetDirection(Math::Vector3 dir)
		{
			data.Direction = Math::Vector4(dir, 1.0f);
		}
		void SpotLightComponent::SetIntensity(float intensity)
		{
			data.Intensity_Attenuation.x = intensity;
		}
		void SpotLightComponent::SetAttenuation(Math::Vector3 att)
		{
			data.Intensity_Attenuation = Math::Vector4(data.Intensity_Attenuation.x, att.x, att.y, att.z);
		}
		void SpotLightComponent::SetSpotlightCone(Math::Vector2 cutoff_outercutoff)
		{
			data.InnerCutOf_OuterCutoff = Math::Vector4(cutoff_outercutoff.x, cutoff_outercutoff.y, 0.0f, 0.0f);

		}
		Graphics::Color SpotLightComponent::GetColor()
		{
			return Graphics::Color(data.Color.r, data.Color.g, data.Color.b, data.Color.a);
		}
		Math::Vector3 SpotLightComponent::GetPosition()
		{
			return Math::Vector3(data.Position);
		}
		Math::Vector3 SpotLightComponent::GetDirection()
		{
			return Math::Vector3(data.Direction);
		}
		float SpotLightComponent::GetIntensity()
		{
			return data.Intensity_Attenuation.x;
		}
		Math::Vector3 SpotLightComponent::GetAttenuation()
		{
			return Math::Vector3(data.Intensity_Attenuation.y, data.Intensity_Attenuation.z, data.Intensity_Attenuation.w);
		}
		Math::Vector2 SpotLightComponent::GetSpotlightCone()
		{
			return Math::Vector2(data.InnerCutOf_OuterCutoff);
		}
		Internal::Shader_SpotLight_Struct SpotLightComponent::GetInternalData()
		{
			return data;
		}
	}
}