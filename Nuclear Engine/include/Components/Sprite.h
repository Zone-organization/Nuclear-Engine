#pragma once
#include <NE_Common.h>
#include <API\Texture.h>
#include <API\Color.h>
#include <Math\Math.h>
namespace NuclearEngine 
{
	namespace Components 
	{
		class NEAPI Sprite
		{
		public:
			Sprite(API::Texture *texture, Math::Vector2 position = Math::Vector2(1.0f, 1.0f), Math::Vector2 size = Math::Vector2(1.0f,1.0f), API::Color color = API::Color(1.0f, 1.0f, 1.0f, 1.0f), Math::Vector2 velocity = Math::Vector2(0.0f, 0.0f));

			API::Texture *Texture;
			Math::Vector2 Position, Size, Velocity;
			API::Color Color;
			float  Rotation;
			bool   IsSolid;
			bool   Destroyed;
		};

	}
}