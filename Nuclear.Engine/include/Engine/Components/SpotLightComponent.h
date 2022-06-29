#pragma once
#include <Engine/Components/LightCommon.h>
#include <Engine/Graphics/Color.h>
#include <Engine/Graphics/ShadowMap.h>

namespace Nuclear
{
	namespace Rendering
	{
		class ShadowManager;
	}
	namespace Systems
	{
		class LightingSystem;
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

			Internal::Shader_SpotLight_Struct& GetInternalData();
			Math::Vector3 GetInternalPosition() const;
			void SetInternalPosition(Math::Vector3 pos);

			Graphics::ShadowMap* GetShadowMap();

		protected:
			friend class Rendering::ShadowManager;
			friend class Systems::LightingSystem;

			Math::Matrix4 LightSpace;

		private:
			Internal::Shader_SpotLight_Struct data;
			LightShadowType mShadowType;
			Graphics::ShadowMap mShadowMap;
		};
	}
}