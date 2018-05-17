#pragma once
#include <Engine/Graphics/API/OpenGL\GLCommon.h>
#include <Engine/Graphics/API/Texture_Types.h>
#include <array>

#ifdef NE_COMPILE_CORE_OPENGL
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace OpenGL
			{
				class GLVertexShader;
				class GLPixelShader;

				class NEAPI GLTexture
				{
				public:
					GLTexture();
					~GLTexture();

					static void Create(GLTexture* texture, const Texture_Data& TexData, const Texture_Desc& Desc);
					static void Create(GLTexture* texture, const std::array<Texture_Data, 6>& data, const Texture_Desc& Desc);
					static void Delete(GLTexture* texture);

					void VSBind(unsigned int slot);
					void GSBind(unsigned int slot);
					void PSBind(unsigned int slot);

					Texture_Desc GetTextureDesc();

					//OpenGL only
					unsigned int GLGetTextureID();
				private:
					Texture_Desc desc;

					GLuint textureID;
					GLenum type;
				};
			}
		}
	}
}
#endif