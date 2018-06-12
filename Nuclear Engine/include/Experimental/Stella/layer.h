#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "stella/graphics/shader.h"
#include "stella/graphics/sprite.h"
#include "stella/graphics/renderer.h"

namespace stella {
namespace graphics {

class Layer {
public:
  virtual ~Layer();
  virtual void Add(Sprite *sprite);
  virtual void Remove(Sprite *sprite);
  virtual void Render();

protected:
  Renderer *Ren;
  Shader *Shad;
  glm::mat4 Projection;
  std::vector<Sprite *> Sprites;

  Layer(Renderer *renderer, Shader *shader, glm::mat4 projection);
};
} // namespace graphics
} // namespace stella
