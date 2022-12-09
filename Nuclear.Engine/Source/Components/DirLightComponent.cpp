#include "Components\DirLightComponent.h"

namespace Nuclear
{
	namespace Components
	{
		DirLightComponent::DirLightComponent()
		{
			data.Direction = Math::Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
			data.Color_Intensity = Math::Vector4(0.5f, 0.5f, 0.5f, 1.0f);
			mNearPlane = 1.0F;
			mFarPlane = 50.f;
		}
		DirLightComponent::~DirLightComponent()
		{
		}
		Graphics::Color DirLightComponent::GetColor()
		{
			return Graphics::Color(data.Color_Intensity.r, data.Color_Intensity.g, data.Color_Intensity.b);
		}
		void DirLightComponent::SetColor(Graphics::Color color)
		{
			data.Color_Intensity = Math::Vector4(color.r, color.g, color.b, data.Color_Intensity.w);
		}

		float DirLightComponent::GetIntensity()
		{
			return data.Color_Intensity.w;
		}
		void DirLightComponent::SetIntensity(float intensity)
		{
			data.Color_Intensity.w = intensity;
		}

		Math::Vector3 DirLightComponent::GetDirection()
		{
			return Math::Vector3(data.Direction);
		}
		void DirLightComponent::SetDirection(Math::Vector3 dir)
		{
			data.Direction = Math::Vector4(dir, 1.0f);
		}
		Internal::Shader_DirLight_Struct& DirLightComponent::GetInternalData()
		{
			return data;
		}

		LightShadowType DirLightComponent::GetShadowType() const
		{
			return mShadowType;
		}
		void DirLightComponent::SetShadowType(const LightShadowType& type)
		{
			mShadowType = type;
		}
		Math::Vector3 DirLightComponent::GetInternalPosition() const
		{
			return Math::Vector3(_internalPos);
		}
		void DirLightComponent::SetInternalPosition(Math::Vector3 pos)
		{
			_internalPos = Math::Vector4(pos, 1.0f);
		}

		void DirLightComponent::SetFarPlane(float farplane)
		{
			mFarPlane = farplane;
		}
		float DirLightComponent::GetFarPlane()
		{
			return mFarPlane;
		}
		void DirLightComponent::SetNearPlane(float nearplane)
		{
			mNearPlane = nearplane;
		}
		float DirLightComponent::GetNearPlane()
		{
			return mNearPlane;
		}
	}
}