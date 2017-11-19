#pragma once
#include <array>

namespace NuclearEngine
{
	struct Texture_Data;
	struct Texture_Desc;
}

namespace NuclearRenderer {
	class NRBShader;

	class NRBTextureCube
	{
	public:

		virtual bool Create(const std::array<NuclearEngine::Texture_Data, 6>& data, NuclearEngine::Texture_Desc Desc) = 0;
		virtual void Delete() = 0;
		virtual void Bind(unsigned int index) = 0;
		virtual void Bind(const char *samplerName, NRBShader *shader, unsigned int index) = 0;
		virtual void Unbind() = 0;
		virtual void Unbind(unsigned int index) = 0;

		virtual unsigned int GLGetTextureID() = 0;
	};
}