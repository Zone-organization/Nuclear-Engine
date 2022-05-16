#pragma once
#include <Engine/Components/LightCommon.h>
#include <Engine/Graphics/Color.h>

namespace Nuclear 
{
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