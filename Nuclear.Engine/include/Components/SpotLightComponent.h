#pragma once
#include <Components/LightCommon.h>
#include <Graphics/Color.h>

namespace Nuclear
{
	namespace Rendering
	{
		class ShadowPass;
	}
	namespace Components
	{

		struct NEAPI SpotLightComponent
		{
		public:
			SpotLightComponent();
			~SpotLightComponent();

			Graphics::Color GetColor() const;
			void SetColor(Graphics::Color color);

			Math::Vector3 GetDirection() const;
			void SetDirection(Math::Vector3 dir);

			float GetIntensity() const;
			void SetIntensity(float intensity);

			Math::Vector3 GetAttenuation() const;
			void SetAttenuation(Math::Vector3 att);

			Math::Vector2 GetSpotlightCone() const;
			void SetSpotlightCone(Math::Vector2 cutoff_outercutoff);

			LightShadowType GetShadowType() const;
			void SetShadowType(const LightShadowType& type);

			bool mCastShadows = false;
			float GetFarPlane();
			void SetFarPlane(float farplane);

			float GetNearPlane();
			void SetNearPlane(float nearplane);

			float GetFOV();
			void SetFOV(float fov);

			Internal::Shader_SpotLight_Struct& GetInternalData();
			Math::Vector3 GetInternalPosition() const;
			void SetInternalPosition(Math::Vector3 pos);


		protected:
			friend class Rendering::ShadowPass;

			Math::Matrix4 LightSpace;

		private:
			Internal::Shader_SpotLight_Struct data;
			LightShadowType mShadowType;
			float mFov, mNearPlane, mFarPlane;
		};
	}
}