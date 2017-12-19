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
			Sprite(API::Texture *texture);


		protected:
			API::Texture *m_texture;
		};

	}
}