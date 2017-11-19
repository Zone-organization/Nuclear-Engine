#pragma once
#include <NuclearRendererBase\NRBTextureCube.h>
#include <NuclearRendererOGL3\GL_Common.h>

namespace NuclearRenderer {
	class NROGL3API GLTextureCube : public NRBTextureCube
	{
	public:
		GLTextureCube();

		bool Create(const std::array<NuclearEngine::Texture_Data, 6>& data, NuclearEngine::Texture_Desc Desc) override;
		void Delete() override;
		void Bind(unsigned int index) override;
		void Bind(const char *samplerName, NRBShader *shader, unsigned int index) override;
		void Unbind() override;
		void Unbind(unsigned int index) override;

		unsigned int GLGetTextureID() override;

	private:
		GLuint textureID;
	};
}