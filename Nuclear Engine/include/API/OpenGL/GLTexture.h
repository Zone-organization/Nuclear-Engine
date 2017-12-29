#pragma once
#include <API\OpenGL\GLCommon.h>
#include <API\API_Types.h>
#include <array>

#ifdef NE_COMPILE_OPENGL3_3
namespace NuclearEngine
{
	namespace API
	{
		namespace OpenGL
		{
			class GLShader;
			class NEAPI GLTexture
			{
			public:
				GLTexture();
				~GLTexture();

				static void Create(GLTexture* texture, Texture_Data *TexData, Texture_Desc *Desc);
				static void Create(GLTexture* texture, const std::array<Texture_Data*, 6>& data, Texture_Desc *Desc);
				void VSBind(unsigned int index);
				void VSBind(const char *samplerName, GLShader *shader, unsigned int index);
				void GSBind(unsigned int index);
				void GSBind(const char *samplerName, GLShader *shader, unsigned int index);
				void PSBind(unsigned int index);
				void PSBind(const char *samplerName, GLShader *shader, unsigned int index);

				unsigned int GLGetTextureID();

			private:
				GLuint textureID;

				GLenum type;
			};
		}
	}
}
#endif