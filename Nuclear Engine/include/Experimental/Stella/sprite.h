#pragma once

#include "animator.h"
#include "renderable.h"
#include "spritesheet.h"

namespace stella {
namespace graphics {

struct VertexData {
  glm::vec3 vertex;
  glm::vec2 uv;
  GLfloat tid;
  unsigned int color;
};

class Sprite : public Renderable {
public:
  Animator Animations;

  Sprite(GLint x, GLint y, Texture &texture, const std::vector<GLuint> &frames);
  Sprite(GLint x, GLint y, Texture &texture, GLuint frame = 0);
  Sprite(GLint x, GLint y, GLint w, GLint h, Texture &texture,
         GLuint frame = 0);
  ~Sprite();

  void Update();

  // Getters
	inline unsigned int GetColor() const { return Color; }
  inline GLuint GetCurrentFrame() const { return Frame; }
  inline SpriteSheet GetSpriteSheet() const { return *Sprites; }
  inline glm::vec2 GetFrameCoords() const { return Sprites->GetUV(this->Frame); }

  // Setters
  void SetFrame(GLuint frame);
  void SetDirectFrame(GLuint frame);
	void SetColorRGB(int r, int g, int b);
  void SetColorRGBA(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
	void SetAlpha(int a);
	void Flip() { this->SetScale(glm::vec2(-1.f, 0.f)); }

private:
  //glm::vec4 Color;
  unsigned int Color;
  std::vector<GLuint> Frames;
  GLuint Frame = 0, NumberOfFrames;
  SpriteSheet *Sprites;
};
} // namespace graphics
} // namespace stella
