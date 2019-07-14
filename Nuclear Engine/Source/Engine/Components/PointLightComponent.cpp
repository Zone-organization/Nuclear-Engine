#include "Engine\Components\PointLightComponent.h"

namespace NuclearEngine
{
	namespace Components
	{
		PointLightComponent::PointLightComponent()
		{
			data.Position = Math::Vector4(0.5f, 0.5f, 0.5f, 1.0f);
			data.Intensity_Attenuation = Math::Vector4(1.0f, 1.0f, 0.09f, 0.032f);
			data.Color = Math::Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		}
		PointLightComponent::~PointLightComponent()
		{
		}
		void PointLightComponent::SetColor(Graphics::Color color)
		{
			data.Color = Math::Vector4(color.r, color.g, color.b, 1.0f);
		}
		void PointLightComponent::SetPosition(Math::Vector3 pos)
		{
			data.Position = Math::Vector4(pos, 1.0f);
		}
		void PointLightComponent::SetIntensity(float intensity)
		{
			data.Intensity_Attenuation.x = intensity;
		}
		void PointLightComponent::SetAttenuation(Math::Vector3 att)
		{
			data.Intensity_Attenuation = Math::Vector4(data.Intensity_Attenuation.x, att.x, att.y, att.z);
		}
		Internal::Shader_PointLight_Struct PointLightComponent::GetInternalData()
		{
			return data;
		}
	}
}