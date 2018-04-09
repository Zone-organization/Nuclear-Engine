#pragma once
#include <vector>
#include <Math\Math.h>
#include <Graphics/API/Texture.h>

namespace NuclearEngine {
	namespace Graphics {

		class NEAPI SpriteSheet {
		public:
			SpriteSheet(Graphics::API::Texture *texture, unsigned int framex, unsigned int framey,	unsigned int number_of_frames = 0);
			~SpriteSheet();

			Math::Vector2 GetUV(unsigned int frame);
			unsigned int GetWidth() const;
			unsigned int GetHeight() const;

			inline void SetOffset(unsigned int x, unsigned int y) {
				OffsetX = x;
				OffsetY = y;
			}
		private:
			Graphics::API::Texture *Frames;
			unsigned int FrameX, FrameY, OffsetX, OffsetY, NumOfFrames, SizeInFramesX,	SizeInFramesY;
		};
	}
}
