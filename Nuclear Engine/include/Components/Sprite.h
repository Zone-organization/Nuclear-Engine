#pragma once
#include <NE_Common.h>
#include <Graphics/API/Texture.h>
#include <Graphics/Color.h>
#include <Math\Math.h>
namespace NuclearEngine 
{
	namespace Components 
	{
		class NEAPI Sprite
		{
		public:
			Sprite(Graphics::API::Texture *texture, Math::Vector2 position = Math::Vector2(1.0f, 1.0f), Math::Vector2 size = Math::Vector2(1.0f,1.0f), Graphics::Color color = Graphics::Color(1.0f, 1.0f, 1.0f, 1.0f), Math::Vector2 velocity = Math::Vector2(0.0f, 0.0f));

			Graphics::API::Texture *Texture;
			Math::Vector2 Position, Size, Velocity;
			Graphics::Color Color;
			float  Rotation;
			bool   IsSolid;
			bool   Destroyed;
		};

	}
}