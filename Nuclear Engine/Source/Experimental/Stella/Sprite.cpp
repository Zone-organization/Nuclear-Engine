#include "Experimental/Stella/Sprite.h"

namespace NuclearEngine {
	namespace Graphics {
		Sprite::Sprite(Int32 x, Int32 y, API::Texture &texture,
			const std::vector<Uint32> &frames)
			: Renderable(x, y, texture), Frames(frames) {
			this->Color = 4294967295; // r = 255, g = 255, b = 255, a = 255
			this->NumberOfFrames = this->Frames.size();
			this->Frame = 0;

			this->Sprites =
				new SpriteSheet(texture, this->Dimensions.x, this->Dimensions.y);
		}

		Sprite::Sprite(Int32 x, Int32 y, API::Texture &texture, Uint32 frame)
			: Renderable(x, y, texture) {
			this->Color = 4294967295; // r = 255, g = 255, b = 255, a = 255
			this->NumberOfFrames = 1;
			this->Frame = frame;

			this->Sprites =
				new SpriteSheet(texture, this->Dimensions.x, this->Dimensions.y);
		}

		Sprite::Sprite(Int32 x, Int32 y, Int32 w, Int32 h, API::Texture &texture,
			Uint32 frame)
			: Renderable(x, y, w, h, texture) {
			this->Color = 4294967295; // r = 255, g = 255, b = 255, a = 255
			this->NumberOfFrames = 1;
			this->Frame = frame;

			this->Sprites =
				new SpriteSheet(texture, this->Dimensions.x, this->Dimensions.y);
		}

		Sprite::~Sprite() { delete Sprites; }

		void Sprite::Update() { this->Frame = Animations.Update(); }

		void Sprite::SetFrame(Uint32 frame) { Frame = frame % this->NumberOfFrames; }

		void Sprite::SetDirectFrame(Uint32 frame) { Frame = frame; }

		void Sprite::SetColorRGB(int r, int g, int b) {
			this->Color = 255 << 24 | b << 16 | g << 8 | r;
		}
		void Sprite::SetColorRGBA(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
			this->Color = a << 24 | b << 16 | g << 8 | r;
		}
		void Sprite::SetAlpha(int a) {
			this->Color = a << 24;
		}
	}
}