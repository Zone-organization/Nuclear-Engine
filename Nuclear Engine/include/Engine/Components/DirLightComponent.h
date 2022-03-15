#pragma once
#include <Base\NE_Common.h>
#include <Engine/Graphics/Color.h>
#include <Base\Math\Math.h>
#include <Engine\ECS\Entity.h>

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

		struct NEAPI DirLightComponent : public ECS::Component<DirLightComponent>
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