#include "Components\Sprite.h"

namespace NuclearEngine
{
	namespace Components
	{
		Sprite::Sprite(API::Texture *texture, Math::Vector2 position, Math::Vector2 size)
		{
			this->m_texture = texture;
			this->m_position = position;
			this->m_size = size;
		}
	}
}