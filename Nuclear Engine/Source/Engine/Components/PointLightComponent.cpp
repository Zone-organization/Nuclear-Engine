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
		Math::Vector3 PointLightComponent::GetPosition()
		{
			return Math::Vector3(data.Position);
		}
		void PointLightComponent::SetPosition(Math::Vector3 pos)
		{
			data.Position = Math::Vector4(pos, 1.0f);
		}
		void PointLightComponent::SetIntensity(float intensity)
		{
			data.Intensity_Attenuation.x = intensity;
		}
		Graphics::Color PointLightComponent::GetColor()
		{
			return Graphics::Color(data.Color.r, data.Color.g, data.Color.b, data.Color.a);
		}
		float PointLightComponent::GetIntensity()
		{
			return data.Intensity_Attenuation.x;
		}
		Math::Vector3 PointLightComponent::GetAttenuation()
		{
			return Math::Vector3(data.Intensity_Attenuation.y, data.Intensity_Attenuation.z, data.Intensity_Attenuation.w);
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