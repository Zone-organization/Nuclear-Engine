#include "stella/graphics/texture.h"

#include <glad/glad.h>
#include <soil.h>

namespace stella {
namespace graphics {
Texture::Texture(const std::string name, const char *texPath) : Name(name) {
  this->Cached = false;
  glGenTextures(1, &this->ID);
  this->load(texPath);
}

Texture::~Texture() { glDeleteTextures(1, &this->ID); }

void Texture::Bind() { glBindTexture(GL_TEXTURE_2D, this->ID); }

void Texture::Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

void Texture::load(const char *texPath) {
  int width, height;
  unsigned char *img =
      SOIL_load_image(texPath, &width, &height, 0, SOIL_LOAD_RGBA);
  //if (img == nullptr)
    //std::cout << "It wasn't possible to load " << texPath << std::endl;

  this->Width = (unsigned int)width;
  this->Height = (unsigned int)height;

  glBindTexture(GL_TEXTURE_2D, this->ID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->Width, this->Height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, img);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glGenerateMipmap(GL_TEXTURE_2D);

  SOIL_free_image_data(img);
  glBindTexture(GL_TEXTURE_2D, 0);
}
} // namespace graphics
} // namespace stella
