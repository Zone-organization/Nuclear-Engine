#include <Components\Light.h>
#include <Math\Math.h>
namespace NuclearEngine {
	namespace Components {
		DirectionalLight::DirectionalLight()
		{
			data.Direction = Math::Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
			data.Color = Math::Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		}
		DirectionalLight::~DirectionalLight()
		{
		}
		void DirectionalLight::SetColor(Graphics::Color color)
		{
			data.Color = Math::Vector4(color.r, color.g, color.b, 1.0f);
		}
		void DirectionalLight::SetDirection(Math::Vector3 dir)
		{
			data.Direction = Math::Vector4(dir,1.0f);
		}
		Internal::Shader_DirLight_Struct DirectionalLight::GetInternalData()
		{
			return data;
		}
		PointLight::PointLight()
		{
			data.Position = Math::Vector4(0.5f, 0.5f, 0.5f, 1.0f);
			data.Intensity_Attenuation = Math::Vector4(1.0f, 1.0f, 0.09f, 0.032f);
			data.Color = Math::Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		}
		PointLight::~PointLight()
		{
		}
		void PointLight::SetColor(Graphics::Color color)
		{
			data.Color = Math::Vector4(color.r, color.g, color.b, 1.0f);
		}
		void PointLight::SetPosition(Math::Vector3 pos)
		{
			data.Position = Math::Vector4(pos, 1.0f);
		}
		void PointLight::SetIntensity(float intensity)
		{
			data.Intensity_Attenuation.x = intensity;
		}
		void PointLight::SetAttenuation(Math::Vector3 att)
		{
			data.Intensity_Attenuation = Math::Vector4(data.Intensity_Attenuation.x, att.x, att.y, att.z);
		}
		Internal::Shader_PointLight_Struct PointLight::GetInternalData()
		{
			return data;
		}
		SpotLight::SpotLight()
		{
			data.Position = Math::Vector4(0.5f, 0.5f, 0.5f, 1.0f);
			data.Direction = Math::Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
			data.Intensity_Attenuation = Math::Vector4(1.0f, 1.0f, 0.09f, 0.032f);
			data.InnerCutOf_OuterCutoff = Math::Vector4(cos(Math::radians(12.5f)), cos(Math::radians(15.0f)), 1.0f, 1.0f);
			data.Color = Math::Vector4(0.5f, 0.5f, 0.5f, 1.0f);
		}
		SpotLight::~SpotLight()
		{
		}
		void SpotLight::SetColor(Graphics::Color color)
		{
			data.Color = Math::Vector4(color.r, color.g, color.b, 1.0f);
		}
		void SpotLight::SetPosition(Math::Vector3 pos)
		{
			data.Position = Math::Vector4(pos, 1.0f);
		}
		void SpotLight::SetDirection(Math::Vector3 dir)
		{
			data.Direction = Math::Vector4(dir, 1.0f);
		}
		void SpotLight::SetIntensity(float intensity)
		{
			data.Intensity_Attenuation.x = intensity;
		}
		void SpotLight::SetAttenuation(Math::Vector3 att)
		{
			data.Intensity_Attenuation = Math::Vector4(data.Intensity_Attenuation.x, att.x, att.y, att.z);
		}
		void SpotLight::SetSpotlightCone(Math::Vector2 cutoff_outercutoff)
		{
			data.InnerCutOf_OuterCutoff = Math::Vector4(cutoff_outercutoff.x, cutoff_outercutoff.y,0.0f,0.0f);

		}
		Internal::Shader_SpotLight_Struct SpotLight::GetInternalData()
		{
			return data;
		}
	}
}