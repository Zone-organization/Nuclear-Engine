#include "Components\Sprite.h"

namespace NuclearEngine
{
	namespace Components
	{
		Sprite::Sprite(API::Texture * texture, Math::Vector2 position, Math::Vector2 size, API::Color color, Math::Vector2 velocity)
		{
			Position = position;
			Size = size;
			Velocity = velocity;
			Texture = texture;
		}
	}
}