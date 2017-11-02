#pragma once

namespace NuclearEngine
{
	struct Texture_Data;
	struct Texture_Desc;
}

namespace NuclearRenderer {
	class NRBShader;

	class NRBTexture2D
	{
	public:
		virtual bool Create(NuclearEngine::Texture_Data TexData, NuclearEngine::Texture_Desc Desc) = 0;
		virtual void Delete() = 0;
		virtual void Bind(unsigned int index) = 0;
		virtual void Bind(const char *samplerName, NRBShader *shader, unsigned int index) = 0;
		virtual void Unbind() = 0;
		virtual void Unbind(unsigned int index) = 0;

		virtual unsigned int GLGetTextureID() = 0;
	};
}