#include "stella/graphics/framebuffer.h"
#include "stella/graphics/shader.h"

#include <glad/glad.h>

Framebuffer::Framebuffer(stella::graphics::Display &display) : Display(display) {
	this->init();
}

Framebuffer::~Framebuffer() {
	if (this->FBO) glDeleteFramebuffers(1, &this->FBO);
}

void Framebuffer::Draw() {
	Display.SetClearColor(0.f, 0.f, 0.f);
	Display.Clear();
	glBindVertexArray(this->VAO);
	glBindTexture(GL_TEXTURE_2D, this->GetTexture());
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Framebuffer::Bind() {
  float TexW = this->CurrentTextureResolution.x;
  float TexH = this->CurrentTextureResolution.y;

  // Updates FBO's texture resolusion on window resizing
  if (TexW != this->Display.GetWidth() || TexH != this->Display.GetHeight())
    this->RefreshTextureResolution();

	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
}

void Framebuffer::RefreshTextureResolution() {
  this->CurrentTextureResolution = glm::vec2(this->Display.GetWidth(), this->Display.GetHeight());

  glBindTexture(GL_TEXTURE_2D, this->FBOtex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->CurrentTextureResolution.x, this->CurrentTextureResolution.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Framebuffer::ActivateTexture(GLenum tex_id) {
	glActiveTexture(tex_id);
	glBindTexture(GL_TEXTURE_2D, this->GetTexture());
}

void Framebuffer::Unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::init() {
  this->CurrentTextureResolution = glm::vec2(this->Display.GetWidth(), this->Display.GetHeight());

	// Framebuffer
	glGenFramebuffers(1, &this->FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);

	glGenTextures(1, &this->FBOtex);
	glBindTexture(GL_TEXTURE_2D, this->FBOtex);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->CurrentTextureResolution.x, this->CurrentTextureResolution.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->FBOtex, 0);

	//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		//std::cout << "Error: Framebuffer is not complete." << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Quad
	float vertices[] = {
			// Positions   // Texture coords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
	};
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}
	
