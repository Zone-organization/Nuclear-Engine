#pragma once
#include <Base\NE_Common.h>
#include <Base/Math/Math.h>
#include <vector>

namespace NuclearEngine {
	namespace Graphics {
		namespace API {	class Texture;	}
		class SpriteSheet {
		public:
			SpriteSheet(API::Texture &texture, unsigned int framex, unsigned int framey,	unsigned int number_of_frames = 0);
			~SpriteSheet();

			Math::Vector2 GetUV(unsigned int frame);
			Math::Vector2ui GetDimensions() const;

			inline void SetOffset(unsigned int x, unsigned int y) {
				OffsetX = x;
				OffsetY = y;
			}
		private:
			API::Texture &Frames;
			unsigned int FrameX, FrameY, OffsetX, OffsetY, NumOfFrames, SizeInFramesX,
				SizeInFramesY;
		};
	}
}