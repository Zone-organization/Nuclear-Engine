#pragma once

#include <string>
#include <glm/glm.hpp>

typedef float GLfloat;
typedef unsigned int GLuint;
typedef int GLint;
typedef unsigned char GLboolean;

namespace stella {
namespace graphics {

class Texture; 

class Renderable {
public:
  glm::vec2 Pos, Dimensions, RealDimensions;

  Renderable(GLint x, GLint y, GLint w, GLint h, Texture &texture, GLboolean visible = 1);
  Renderable(GLint x, GLint y, Texture &texture);
  Renderable(Texture &texture);
  virtual ~Renderable();

  // Getters
  inline glm::vec2 GetPos() const { return Pos; }
  inline glm::vec2 GetDimensions() const { return Dimensions; }
  inline GLfloat GetWidth() const { return Dimensions.x; }
  inline GLfloat GetHeight() const { return Dimensions.y; }
  inline GLfloat GetRotation() const { return Rotation; }
  inline glm::vec2 GetScale() const { return Scale; }
  GLuint GetTexID() const;
  const std::string &GetTexName() const;
  Texture *GetTexture() const;
  inline const GLboolean& IsVisible() const { return Visible; }

  // Setters
  void SetVisible(GLboolean vis) { Visible = vis; }
  void ToggleVisible() { Visible = !Visible; }
  void SetDimensions(glm::vec2 dimensions) { Dimensions = dimensions; }
  void SetRotation(float rotation) { Rotation = rotation; }
  void SetRelativeScale(glm::vec2 scale) { Scale = scale; }
  void SetScale(glm::vec2 scale) { Scale = scale; }
  
  void SetDirectScale(glm::vec2 scale) { Scale = scale/(this->Dimensions);  }
  //void SetDirectScale(glm::vec2 scale) { Scale = scale/(this->Dimensions); this->Dimensions *= this->Scale; }

protected:
  Texture &Tex;
  GLboolean Visible;
  float Rotation = 0.f;
  glm::vec2 Scale = glm::vec2(1.f,1.f);
};
} // namespace graphics
} // namespace stella
