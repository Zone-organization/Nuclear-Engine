#include <Components\Light.h>
#include <Math\Math.h>
namespace NuclearEngine {
	namespace Components {
		DirectionalLight::DirectionalLight()
		{
		}
		DirectionalLight::~DirectionalLight()
		{
		}
		void DirectionalLight::SetColor(API::Color color)
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
		}
		PointLight::~PointLight()
		{
		}
		void PointLight::SetColor(API::Color color)
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
		}
		SpotLight::~SpotLight()
		{
		}
		void SpotLight::SetColor(API::Color color)
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