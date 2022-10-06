#pragma once
#include <Engine/Components/LightCommon.h>
#include <Engine/Graphics/Color.h>

namespace Nuclear 
{
	namespace Rendering
	{
		class ShadowPass;
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

			float GetIntensity();
			void SetIntensity(float intensity);

			float GetFarPlane();
			void SetFarPlane(float farplane);

			float GetNearPlane();
			void SetNearPlane(float nearplane);

			bool mCastShadows = false;
			Internal::Shader_DirLight_Struct& GetInternalData();

			LightShadowType GetShadowType() const;
			void SetShadowType(const LightShadowType& type);
			Math::Vector3 GetInternalPosition() const;
			void SetInternalPosition(Math::Vector3 pos);

		protected:
			friend class Rendering::ShadowPass;

			Math::Matrix4 LightSpace;

			Math::Vector4 _internalPos; //needed for simple shadow "not CSM"

			float mNearPlane, mFarPlane;
		private:
			Internal::Shader_DirLight_Struct data;
			LightShadowType mShadowType;
		};

	}
}