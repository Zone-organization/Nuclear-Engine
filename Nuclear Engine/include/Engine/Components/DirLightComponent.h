#pragma once
#include <Core\NE_Common.h>
#include <Engine/Graphics/Color.h>
#include <Core\Math\Math.h>

namespace NuclearEngine 
{
	namespace Components 
	{
		namespace Internal
		{

			struct NEAPI Shader_DirLight_Struct
			{
				Math::Vector4 Direction;
				Math::Vector4 Color;
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


			Internal::Shader_DirLight_Struct GetInternalData();
		protected:
			Internal::Shader_DirLight_Struct data;
		};

	}
}