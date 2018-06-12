#include "stella/graphics/sprite.h"

namespace stella {
namespace graphics {
Sprite::Sprite(GLint x, GLint y, Texture &texture,
               const std::vector<GLuint> &frames)
    : Renderable(x, y, texture), Frames(frames) {
  this->Color = 4294967295; // r = 255, g = 255, b = 255, a = 255
  this->NumberOfFrames = this->Frames.size();
  this->Frame = 0;

  this->Sprites =
      new SpriteSheet(texture, this->Dimensions.x, this->Dimensions.y);
}

Sprite::Sprite(GLint x, GLint y, Texture &texture, GLuint frame)
    : Renderable(x, y, texture) {
  this->Color = 4294967295; // r = 255, g = 255, b = 255, a = 255
  this->NumberOfFrames = 1;
  this->Frame = frame;

  this->Sprites =
      new SpriteSheet(texture, this->Dimensions.x, this->Dimensions.y);
}

Sprite::Sprite(GLint x, GLint y, GLint w, GLint h, Texture &texture,
               GLuint frame)
    : Renderable(x, y, w, h, texture) {
  this->Color = 4294967295; // r = 255, g = 255, b = 255, a = 255
  this->NumberOfFrames = 1;
  this->Frame = frame;

  this->Sprites =
      new SpriteSheet(texture, this->Dimensions.x, this->Dimensions.y);
}

Sprite::~Sprite() { delete Sprites; }

void Sprite::Update() { this->Frame = Animations.Update(); }

void Sprite::SetFrame(GLuint frame) { Frame = frame % this->NumberOfFrames; }

void Sprite::SetDirectFrame(GLuint frame) { Frame = frame; }

void Sprite::SetColorRGB(int r, int g, int b) {
	this->Color = 255 << 24 | b << 16 | g << 8 | r;
}
void Sprite::SetColorRGBA(unsigned int r, unsigned int g, unsigned int b, unsigned int a) {
	this->Color = a << 24 | b << 16 | g << 8 | r;
}
void Sprite::SetAlpha(int a) {
	this->Color = a << 24;
}
} // namespace graphics
} // namespace stella
