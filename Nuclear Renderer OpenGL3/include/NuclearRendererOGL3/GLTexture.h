#pragma once
#include <NuclearRendererBase\NRBTexture.h>
#include <NuclearRendererOGL3\GL_Common.h>

namespace NuclearRenderer {
	class NROGL3API GLTexture : public NRBTexture
	{
	public:
		GLTexture();

		void Create(NuclearEngine::Texture_Data TexData, NuclearEngine::Texture_Desc Desc) override;
		void Create(const std::array<NuclearEngine::Texture_Data, 6>& data, NuclearEngine::Texture_Desc Desc) override;
		void Delete() override;
		void VSBind(unsigned int index) override;
		void VSBind(const char *samplerName, NRBShader *shader, unsigned int index) override;
		void GSBind(unsigned int index) override;
		void GSBind(const char *samplerName, NRBShader *shader, unsigned int index) override;
		void PSBind(unsigned int index) override;
		void PSBind(const char *samplerName, NRBShader *shader, unsigned int index) override;

		unsigned int GLGetTextureID() override;

	private:
		GLuint textureID;

		GLenum type;
	};
}