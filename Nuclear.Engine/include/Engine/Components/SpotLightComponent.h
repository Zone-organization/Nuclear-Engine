#pragma once
#include <Engine/Graphics/Color.h>

namespace Nuclear
{
	namespace Components
	{
		namespace Internal
		{
			struct NEAPI Shader_SpotLight_Struct
			{
				Math::Vector4 Position;
				Math::Vector4 Direction;
				Math::Vector4 Intensity_Attenuation;
				Math::Vector4 InnerCutOf_OuterCutoff;
				Math::Vector4 Color;

				//template<class Archive>
				//void serialize(Archive& archive)
				//{
				//	archive(CEREAL_NVP(Position));
				//	archive(CEREAL_NVP(Direction));
				//	archive(CEREAL_NVP(Intensity_Attenuation));
				//	archive(CEREAL_NVP(InnerCutOf_OuterCutoff));
				//	archive(CEREAL_NVP(Color));
				//}
			};
		}

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

			bool mCastShadows = false;
			Internal::Shader_SpotLight_Struct GetInternalData();
			Math::Vector3 GetInternalPosition() const;
			void SetInternalPosition(Math::Vector3 pos);

			/*template<class Archive>
			void serialize(Archive& archive)
			{
				archive(CEREAL_NVP(mCastShadows));
				archive(CEREAL_NVP(data));
			}*/
		protected:
			Internal::Shader_SpotLight_Struct data;
		};
	}
}