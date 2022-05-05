#pragma once
#include <Engine/Graphics/Color.h>

namespace Nuclear
{
	namespace Components
	{
		namespace Internal
		{
			struct NEAPI Shader_PointLight_Struct
			{
				Math::Vector4 Position;
				Math::Vector4 Intensity_Attenuation;
				Math::Vector4 Color;

		/*		template<class Archive>
				void serialize(Archive& archive)
				{
					archive(CEREAL_NVP(Position));
					archive(CEREAL_NVP(Intensity_Attenuation));
					archive(CEREAL_NVP(Color));
				}*/
			};
		}

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
			Internal::Shader_PointLight_Struct GetInternalData();
			Math::Vector3 GetInternalPosition();
			void SetInternalPosition(Math::Vector3 pos);

		/*	template<class Archive>
			void serialize(Archive& archive)
			{
				archive(CEREAL_NVP(mCastShadows));
				archive(CEREAL_NVP(data));
			}*/
		protected:
			Internal::Shader_PointLight_Struct data;
		};

	}
}