#pragma once
#include <Engine/Graphics/Color.h>

namespace Nuclear 
{
	namespace Components 
	{
		namespace Internal
		{

			struct NEAPI Shader_DirLight_Struct
			{
				Math::Vector4 Direction;
				Math::Vector4 Color;

				//template<class Archive>
				//void serialize(Archive& archive)
				//{
				//	archive(CEREAL_NVP(Direction));
				//	archive(CEREAL_NVP(Color));
				//}
			};
		}

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
			Internal::Shader_DirLight_Struct GetInternalData();

	/*		template<class Archive>
			void serialize(Archive& archive)
			{
				archive(CEREAL_NVP(mCastShadows));
				archive(CEREAL_NVP(data));
			}*/
		protected:
			Internal::Shader_DirLight_Struct data;
		};

	}
}