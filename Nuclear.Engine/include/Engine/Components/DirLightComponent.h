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
		struct NEAPI DirLightComponent
		{
		public:
			DirLightComponent();
			~DirLightComponent();

			Graphics::Color GetColor();
			void SetColor(Graphics::Color color);

			Math::Vector3 GetDirection();
			void SetDirection(Math::Vector3 dir);

			bool mCastShadows = false;
			Internal::Shader_DirLight_Struct& GetInternalData();

			LightShadowType GetShadowType() const;
			void SetShadowType(const LightShadowType& type);
			Graphics::ShadowMap* GetShadowMap();
			Math::Vector3 GetInternalPosition() const;
			void SetInternalPosition(Math::Vector3 pos);

		protected:
			friend class Rendering::ShadowManager;
			friend class Systems::LightingSystem;

			Math::Matrix4 LightSpace;

			Math::Vector4 _internalPos; //needed for simple shadow "not CSM"

		private:
			Graphics::ShadowMap mShadowMap;
			Internal::Shader_DirLight_Struct data;
			LightShadowType mShadowType;
		};

	}
}