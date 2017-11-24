#pragma once
#include <NuclearRendererBase\NRBTexture2D.h>
#include <NuclearRendererOGL3\GL_Common.h>

namespace NuclearRenderer {
	class NROGL3API GLTexture2D : public NRBTexture2D
	{
	public:
		GLTexture2D();

		bool Create(NuclearEngine::Texture_Data TexData, NuclearEngine::Texture_Desc Desc) override;
		bool Create(NuclearEngine::RenderTarget_Attachment_Desc Desc) override;
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
	};
}