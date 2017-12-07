#pragma once

namespace NuclearEngine
{
	struct Texture_Data;
	struct Texture_Desc;
}

namespace NuclearRenderer {
	class NRBShader;

	class NRBTexture
	{
	public:
		virtual void Create(NuclearEngine::Texture_Data TexData, NuclearEngine::Texture_Desc Desc) = 0;
		virtual void Delete() = 0;

		virtual void VSBind(unsigned int index) = 0;
		virtual void VSBind(const char *samplerName, NRBShader *shader, unsigned int index) = 0;
		virtual void GSBind(unsigned int index) = 0;
		virtual void GSBind(const char *samplerName, NRBShader *shader, unsigned int index) = 0;
		virtual void PSBind(unsigned int index) = 0;
		virtual void PSBind(const char *samplerName, NRBShader *shader, unsigned int index) = 0;

		virtual unsigned int GLGetTextureID() = 0;
	};
}