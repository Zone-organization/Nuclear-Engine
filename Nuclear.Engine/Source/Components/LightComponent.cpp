#include <Components/LightComponent.h>

namespace Nuclear
{
	namespace Components
	{
		LightComponent::LightComponent()
			: LightComponent(Type::Point)
		{
			
		}
		LightComponent::LightComponent(Type type)
		{
			mType = type;
			mData.Position = Math::Vector4(0.5f, 0.5f, 0.5f, 1.0f);
			mData.Direction = Math::Vector4(-0.5f, -0.5f, -0.5f, 1.0f);
			mData.Color_Intensity = Math::Vector4(1.0f, 1.0f, 1.0f, 1.0f);
			mData.Attenuation_FarPlane = Math::Vector4(1.0f, 0.09f, 0.032f, 50.0f);
			mData.InnerCutOf_OuterCutoff = Math::Vector4(cos(Math::radians(12.5f)), cos(Math::radians(15.0f)), 1.0f, 1.0f);
			mFov = 90.f;
			mNearPlane = 1.0F;
		}

		LightComponent::~LightComponent()
		{
		}
		void LightComponent::SetColor(Graphics::Color color)
		{
			mData.Color_Intensity = Math::Vector4(color.r, color.g, color.b, mData.Color_Intensity.w);
		}
		Math::Vector3 LightComponent::GetInternalPosition()
		{
			return Math::Vector3(mData.Position);
		}
		void LightComponent::SetInternalPosition(Math::Vector3 pos)
		{
			mData.Position = Math::Vector4(pos, 1.0f);
		}

		LightComponent::Type LightComponent::GetType() const
		{
			return mType;
		}

		void LightComponent::SetType(Type type)
		{
			mType = type;
		}

		Graphics::Color LightComponent::GetColor()
		{
			return Graphics::Color(mData.Color_Intensity.r, mData.Color_Intensity.g, mData.Color_Intensity.b, 1.0f);
		}
		float LightComponent::GetIntensity()
		{
			return mData.Color_Intensity.w;
		}
		void LightComponent::SetFarPlane(float farplane)
		{
			mData.Attenuation_FarPlane.w = farplane;
		}
		float LightComponent::GetFarPlane()
		{
			return mData.Attenuation_FarPlane.w;
		}
		void LightComponent::SetNearPlane(float nearplane)
		{
			mNearPlane = nearplane;
		}
		float LightComponent::GetNearPlane()
		{
			return mNearPlane;
		}
		void LightComponent::SetFOV(float fov)
		{
			mFov = fov;
		}
		float LightComponent::GetFOV()
		{
			return mFov;
		}
		void LightComponent::SetIntensity(float intensity)
		{
			mData.Color_Intensity.w = intensity;
		}
		Math::Vector3 LightComponent::GetAttenuation()
		{
			return 	Math::Vector3(mData.Attenuation_FarPlane);
		}
		void LightComponent::SetAttenuation(Math::Vector3 att)
		{
			mData.Attenuation_FarPlane = Math::Vector4(att, mData.Attenuation_FarPlane.w);
		}

		Math::Vector3 LightComponent::GetDirection() const
		{
			return Math::Vector3(mData.Direction);
		}
		void LightComponent::SetSpotlightCone(Math::Vector2 cutoff_outercutoff)
		{
			mData.InnerCutOf_OuterCutoff = Math::Vector4(cutoff_outercutoff.x, cutoff_outercutoff.y, 0.0f, 0.0f);

		}
		void LightComponent::SetDirection(Math::Vector3 dir)
		{
			mData.Direction = Math::Vector4(dir, 1.0f);
		}
		Math::Vector2 LightComponent::GetSpotlightCone() const
		{
			return Math::Vector2(mData.InnerCutOf_OuterCutoff);
		}

		Internal::LightData& LightComponent::GetInternalData()
		{
			return mData;
		}
	}
}