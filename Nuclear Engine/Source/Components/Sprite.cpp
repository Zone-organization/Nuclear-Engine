#include <Components\Sprite.h>

namespace NuclearEngine {
	namespace Components {
		Sprite::Sprite(int x, int y, Graphics::API::Texture *texture, const std::vector<uint> &frames)
			: Renderable2D(x, y, texture), Frames(frames) {
			this->Color = 4294967295; // r = 255, g = 255, b = 255, a = 255
			this->NumberOfFrames = this->Frames.size();
			this->Frame = 0;

			this->Sprites =	new Graphics::SpriteSheet(texture, this->Dimensions.x, this->Dimensions.y);
		}

		Sprite::Sprite(int x, int y, Graphics::API::Texture *texture, uint frame)
			: Renderable2D(x, y, texture) {
			this->Color = 4294967295; // r = 255, g = 255, b = 255, a = 255
			this->NumberOfFrames = 1;
			this->Frame = frame;

			this->Sprites =	new Graphics::SpriteSheet(texture, this->Dimensions.x, this->Dimensions.y);
		}

		Sprite::Sprite(int x, int y, int w, int h, Graphics::API::Texture *texture,
			uint frame)
			: Renderable2D(x, y, w, h, texture) {
			this->Color = 4294967295; // r = 255, g = 255, b = 255, a = 255
			this->NumberOfFrames = 1;
			this->Frame = frame;

			this->Sprites =	new Graphics::SpriteSheet(texture, this->Dimensions.x, this->Dimensions.y);
		}

		Sprite::~Sprite() { delete Sprites; }

		void Sprite::Update() { this->Frame = Animations.Update(); }

		void Sprite::SetFrame(uint frame) { Frame = frame % this->NumberOfFrames; }

		void Sprite::SetDirectFrame(uint frame) { Frame = frame; }

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
