#pragma once
#include <Engine/Graphics/API/OpenGL\GLCommon.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <Base\Math\Math.h>
#include <Engine/Graphics/API/Texture_Types.h>
#include <array>

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

					static bool Create(GLTexture* texture, const Texture_Data& TexData, const Texture_Desc& Desc);
					static bool Create(GLTexture* texture, const std::array<Texture_Data, 6>& data, const Texture_Desc& Desc);
					static void Delete(GLTexture* texture);

					void VSBind(Uint8 slot);
					void GSBind(Uint8 slot);
					void PSBind(Uint8 slot);

					Math::Vector3ui GetDimensions(Uint8 Mipmaplevel);

					//OpenGL only
					unsigned int GLGetTextureID();
				private:
					GLuint textureID;
					GLenum type;
				};
			}
		}
	}
}
#endif