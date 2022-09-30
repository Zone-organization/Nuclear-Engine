#include "Engine\Components\PointLightComponent.h"

namespace Nuclear
{
	namespace Components
	{
		PointLightComponent::PointLightComponent()
		{
			data.Position = Math::Vector4(0.5f, 0.5f, 0.5f, 1.0f);
			data.Intensity_Attenuation = Math::Vector4(1.0f, 1.0f, 0.09f, 0.032f);
			data.Color_FarPlane = Math::Vector4(0.5f, 0.5f, 0.5f, 50.f);
			mFov = 90.f;
			mNearPlane = 1.0F;
		}
		PointLightComponent::~PointLightComponent()
		{
		}
		void PointLightComponent::SetColor(Graphics::Color color)
		{
			data.Color_FarPlane = Math::Vector4(color.r, color.g, color.b, data.Color_FarPlane.w);
		}
		Math::Vector3 PointLightComponent::GetInternalPosition()
		{
			return Math::Vector3(data.Position);
		}
		void PointLightComponent::SetInternalPosition(Math::Vector3 pos)
		{
			data.Position = Math::Vector4(pos, 1.0f);
		}
	
		Graphics::Color PointLightComponent::GetColor()
		{
			return Graphics::Color(data.Color_FarPlane.r, data.Color_FarPlane.g, data.Color_FarPlane.b, 1.0f);
		}
		float PointLightComponent::GetIntensity()
		{
			return data.Intensity_Attenuation.x;
		}
		void PointLightComponent::SetFarPlane(float farplane)
		{
			data.Color_FarPlane.w = farplane;
		}
		float PointLightComponent::GetFarPlane()
		{
			return data.Color_FarPlane.w;
		}
		void PointLightComponent::SetNearPlane(float nearplane)
		{
			mNearPlane = nearplane;
		}
		float PointLightComponent::GetNearPlane()
		{
			return mNearPlane;
		}
		void PointLightComponent::SetFOV(float fov)
		{
			mFov = fov;
		}
		float PointLightComponent::GetFOV()
		{
			return mFov;
		}
		void PointLightComponent::SetIntensity(float intensity)
		{
			data.Intensity_Attenuation.x = intensity;
		}
		Math::Vector3 PointLightComponent::GetAttenuation()
		{
			return Math::Vector3(data.Intensity_Attenuation.y, data.Intensity_Attenuation.z, data.Intensity_Attenuation.w);
		}
		void PointLightComponent::SetAttenuation(Math::Vector3 att)
		{
			data.Intensity_Attenuation = Math::Vector4(data.Intensity_Attenuation.x, att.x, att.y, att.z);
		}
		Internal::Shader_PointLight_Struct& PointLightComponent::GetInternalData()
		{
			return data;
		}
	}
}