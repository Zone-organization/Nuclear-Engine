#include "stella/graphics/renderable.h"
#include "stella/graphics/texture.h"

#include <vector>
#include <glad/glad.h>

namespace stella {
namespace graphics {
Renderable::Renderable(GLint x, GLint y, GLint w, GLint h, Texture &texture,
                       GLboolean visible)
    : Tex(texture) {
  this->Pos = glm::vec2((GLfloat)x, (GLfloat)y);
  this->Dimensions = glm::vec2((GLfloat)w, (GLfloat)h);
  this->RealDimensions = Dimensions;
  this->Visible = visible;
}

Renderable::Renderable(GLint x, GLint y, Texture &texture) : Tex(texture) {
  this->Pos = glm::vec2((GLfloat)x, (GLfloat)y);
  this->Dimensions = glm::vec2(this->Tex.GetWidth(), this->Tex.GetHeight());
  this->RealDimensions = Dimensions;
  this->Visible = GL_TRUE;
}

Renderable::Renderable(Texture &texture) : Tex(texture) {
  this->Pos = glm::vec2(0.0f, 0.0f);
  this->Dimensions = glm::vec2(this->Tex.GetWidth(), this->Tex.GetHeight());
  this->RealDimensions = Dimensions;
  this->Visible = GL_TRUE;
}

Renderable::~Renderable() {}

GLuint Renderable::GetTexID() const { return Tex.GetID(); }
const std::string& Renderable::GetTexName() const { return Tex.GetName(); }
Texture* Renderable::GetTexture() const { return &Tex; }

} // namespace graphics
} // namespace stella
