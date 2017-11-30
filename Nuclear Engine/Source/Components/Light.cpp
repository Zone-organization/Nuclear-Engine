#include <Components\Light.h>
#include <Math\Math.h>
namespace NuclearEngine {
	namespace Components {

		SpotLight::SpotLight()
		{
			data.m_color = Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			data.m_pos = Math::Vector4();
			data.m_dir = Math::Vector4();
			data.m_intensity_att = Math::Vector4(1.0f, 1.0f, 0.09f, 0.032f);
			data.m_cutoff_outercutoff = Math::Vector4(cos(Math::ToRadians(12.5f)), cos(Math::ToRadians(15.0f)), 0.0f,0.0f);
		}
		SpotLight::~SpotLight()
		{
		}

		void SpotLight::SetColor(API::Color color)
		{
			data.m_color = Math::Vector4(color.r, color.g, color.b, 1.0f);
		}
		void SpotLight::SetPosition(Math::Vector3 pos)
		{
			data.m_pos = Math::Vector4(pos, 1.0f);
		}
		void SpotLight::SetDirection(Math::Vector3 dir)
		{
			data.m_dir = Math::Vector4(dir, 1.0f);
		}
		void SpotLight::SetIntensity(float intensity)
		{
			data.m_intensity_att.x = intensity;
		}
		void SpotLight::SetAttenuation(Math::Vector3 att)
		{
			data.m_intensity_att = Math::Vector4(data.m_intensity_att.x, att.x, att.y, att.z);
		}
		void SpotLight::SetSpotlightCone(Math::Vector2 cutoff_outercutoff)
		{
			data.m_cutoff_outercutoff = Math::Vector4(cutoff_outercutoff.x, cutoff_outercutoff.y, 0.0f, 0.0f);
		}
		DirectionalLight::DirectionalLight()
		{
			data.m_color = Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			data.m_dir = Math::Vector4();
		}
		DirectionalLight::~DirectionalLight()
		{
		}
		void DirectionalLight::SetColor(API::Color color)
		{
			data.m_color = Math::Vector4(color.r, color.g, color.b, 1.0f);

		}
		void DirectionalLight::SetDirection(Math::Vector3 dir)
		{
			data.m_dir = Math::Vector4(dir, 1.0f);
		}
		PointLight::PointLight()
		{
			data.m_color = Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			data.m_pos = Math::Vector4();
			data.m_intensity_att = Math::Vector4(1.0f, 1.0f, 0.09f, 0.032f);
		}
		PointLight::~PointLight()
		{
		}
		void PointLight::SetColor(API::Color color)
		{
			data.m_color = Math::Vector4(color.r, color.g, color.b, 1.0f);
		}
		void PointLight::SetPosition(Math::Vector3 pos)
		{
			data.m_pos = Math::Vector4(pos, 1.0f);
		}
		void PointLight::SetIntensity(float intensity)
		{
			data.m_intensity_att.x = intensity;
		}
		void PointLight::SetAttenuation(Math::Vector3 att)
		{
			data.m_intensity_att = Math::Vector4(data.m_intensity_att.x, att.x, att.y, att.z);
		}
	}
}