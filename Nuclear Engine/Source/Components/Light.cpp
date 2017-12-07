#include <Components\Light.h>
#include <Math\Math.h>
namespace NuclearEngine {
	namespace Components {
		struct NEAPI Shader_Light_Struct
		{
			Math::Vector4 Position;
			Math::Vector4 Direction;
			Math::Vector4 Attenuation_Intensity;
			Math::Vector4 InnerCutOf_OuterCutoff;
			Math::Vector4 Color;
		};

		Light::Light(Type type)
		{
			data.Color = Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			data.Position = Math::Vector4(0.0f,0.0f,0.0f, type);
			data.Direction = Math::Vector4();
			data.Attenuation_Intensity = Math::Vector4(1.0f, 1.0f, 0.09f, 0.032f);
			data.InnerCutOf_OuterCutoff = Math::Vector4(cos(Math::ToRadians(12.5f)), cos(Math::ToRadians(15.0f)), 0.0f,0.0f);
		}
		Light::~Light()
		{
		}

		void Light::SetType(Type type)
		{
			data.Position = Math::Vector4(data.Position.xyz, type);
		}

		void Light::SetColor(API::Color color)
		{
			data.Color = Math::Vector4(color.r, color.g, color.b, 1.0f);
		}
		void Light::SetPosition(Math::Vector3 pos)
		{
			data.Position = Math::Vector4(pos, data.Position.w);
		}
		void Light::SetDirection(Math::Vector3 dir)
		{
			data.Direction = Math::Vector4(dir, 1.0f);
		}
		void Light::SetIntensity(float intensity)
		{
			data.Attenuation_Intensity.w = intensity;
		}
		void Light::SetAttenuation(Math::Vector3 att)
		{
			data.Attenuation_Intensity = Math::Vector4(data.Attenuation_Intensity.z, att.x, att.y, att.z);
		}
		void Light::SetSpotlightCone(Math::Vector2 cutoff_outercutoff)
		{
			data.InnerCutOf_OuterCutoff = Math::Vector4(cutoff_outercutoff.x, cutoff_outercutoff.y, 0.0f, 0.0f);
		}

	}
}