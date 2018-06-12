#include "stella/graphics/layer.h"

#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace stella {
namespace graphics {
Layer::Layer(Renderer *renderer, Shader *shader, glm::mat4 projection)
    : Ren(renderer), Shad(shader), Projection(projection) {
  this->Shad->Enable();
  this->Shad->SetMat4("proj", projection);
  this->Shad->Disable();
}

Layer::~Layer() {
  this->Shad->Disable();
  for (auto i : this->Sprites)
    delete i;
  delete Ren;
}

void Layer::Add(Sprite *sprite) { this->Sprites.push_back(sprite); }

void Layer::Remove(Sprite *sprite) {
  auto it = std::find(this->Sprites.begin(), this->Sprites.end(), sprite);
  this->Sprites.erase(it);
}

void Layer::Render() {

  this->Shad->Enable();
  this->Ren->Begin();

  //glm::mat4 trans;
  //trans = glm::translate(trans, glm::vec3(360.0f, 202.0f, 0.0f));
  //trans = glm::scale(trans, glm::vec3(0.9f, 0.9f, 1.0f));
  //trans = glm::rotate(trans, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
  //trans = glm::translate(trans, glm::vec3(-360.0f, -202.0f, 0.0f));
  //this->Ren->PushTransformation(trans);

  for (auto i : Sprites)
    this->Ren->Submit(*i);
  
  //this->Ren->PopTransformation();

  this->Ren->End();
  this->Ren->Draw();
	this->Shad->Disable();
}
} // namespace graphics
} // namespace stella
