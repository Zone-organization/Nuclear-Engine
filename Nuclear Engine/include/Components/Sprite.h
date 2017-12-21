#pragma once
#include <NE_Common.h>
#include <API\Texture.h>
#include <Math\Math.h>
namespace NuclearEngine 
{
	namespace Components 
	{
		class NEAPI Sprite
		{
		public:
			Sprite(API::Texture *texture, Math::Vector2 position, Math::Vector2 size);


		protected:
			API::Texture *m_texture;
			Math::Vector2 m_position;
			Math::Vector2 m_size;
		};

	}
}