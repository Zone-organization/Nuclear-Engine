#pragma once
#include <Base\NE_Common.h>
#include <Base/Math/Math.h>
#include <vector>

namespace NuclearEngine {
	namespace Graphics {
		class Texture;

		class SpriteSheet {
		public:
			SpriteSheet(const Texture &texture, unsigned int framex, unsigned int framey,
				unsigned int number_of_frames = 0);
			~SpriteSheet();

			glm::vec2 GetUV(unsigned int frame);
			unsigned int GetWidth() const;
			unsigned int GetHeight() const;

			inline void SetOffset(unsigned int x, unsigned int y) {
				OffsetX = x;
				OffsetY = y;
			}
		private:
			const Texture &Frames;
			unsigned int FrameX, FrameY, OffsetX, OffsetY, NumOfFrames, SizeInFramesX,
				SizeInFramesY;
		};
	} 
}