#pragma once
#include <Components/LightCommon.h>
#include <Graphics/Color.h>

namespace Nuclear
{
	namespace Components
	{
		struct NEAPI PointLightComponent
		{
		public:
			PointLightComponent();
			~PointLightComponent();

			Graphics::Color GetColor();
			void SetColor(Graphics::Color color);

			float GetIntensity();
			void SetIntensity(float intensity);

			Math::Vector3 GetAttenuation();
			void SetAttenuation(Math::Vector3 att);

			bool mCastShadows = false;
			float GetFarPlane();
			void SetFarPlane(float farplane);

			float GetNearPlane();
			void SetNearPlane(float nearplane);

			float GetFOV();
			void SetFOV(float fov);

			Internal::Shader_PointLight_Struct& GetInternalData();
			Math::Vector3 GetInternalPosition();
			void SetInternalPosition(Math::Vector3 pos);

		protected:
			Internal::Shader_PointLight_Struct data;
			LightShadowType mShadowType;
			float mFov, mNearPlane;
		};

	}
}