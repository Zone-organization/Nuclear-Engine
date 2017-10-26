#pragma once
#include <NuclearRendererBase\NRBTexture2D.h>
#include <NuclearRendererOGL3\GL_Common.h>

namespace NuclearRenderer {
	class NROGL3API GLTexture2D : public NRBTexture2D
	{
	public:
		GLTexture2D();

		bool Create(NuclearEngine::Texture_Data TexData, NuclearEngine::Texture_Desc Desc) override;
		void Delete() override;
		void Bind(unsigned int index) override;
		void Bind(const char *samplerName, NRBShader *shader, unsigned int index) override;
		void Unbind() override;
		void Unbind(unsigned int index) override;

	private:
		GLuint textureID;
	};
}