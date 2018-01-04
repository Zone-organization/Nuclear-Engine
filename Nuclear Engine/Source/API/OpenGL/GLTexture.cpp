#include <API\OpenGL\GLTexture.h>

#ifdef NE_COMPILE_OPENGL3_3
#include <API\OpenGL\GLShader.h>
#include <API\API_Types.h>
#include <Core\Context.h>

namespace NuclearEngine
{
	namespace API
	{
		namespace OpenGL
		{

			int GetGLTextureFormat(TextureFormat format);
			int GetGLTextureWrap(TextureWrap textureWrap);
			
			GLTexture::GLTexture() : textureID(0) , type(0)
			{
			}

			GLTexture::~GLTexture()
			{
				if (textureID != 0)
				{
					glDeleteTextures(1, &textureID);
				}

				textureID = 0;
				type = 0;
			}

			void GLTexture::Create(GLTexture* result,Texture_Data *Data, Texture_Desc *Desc)
			{
				if (Desc->Type == TextureType::Texture1D)
				{
					result->type = GL_TEXTURE_1D;
				}
				else if (Desc->Type == TextureType::Texture2D)
				{
					result->type = GL_TEXTURE_2D;
				}
				else if (Desc->Type == TextureType::Texture3D)
				{
					result->type = GL_TEXTURE_3D;
				}

				glGenTextures(1, &result->textureID);
				glBindTexture(result->type, result->textureID);

				if (Desc->Format == TextureFormat::R8)
				{
					glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
				}


				if (Desc->Type == TextureType::Texture1D)
				{
					glTexImage1D(result->type,
						0,
						GetGLTextureFormat(Desc->Format),
						Data->width,
						0,
						GetGLTextureFormat(Desc->Format),
						GL_UNSIGNED_BYTE,
						Data->databuf);
				}
				else if (Desc->Type == TextureType::Texture2D)
				{
					glTexImage2D(result->type,
						0,
						GetGLTextureFormat(Desc->Format),
						Data->width,
						Data->height,
						0,
						GetGLTextureFormat(Desc->Format),
						GL_UNSIGNED_BYTE,
						Data->databuf);
				}
				else if (Desc->Type == TextureType::Texture3D)
				{
					glTexImage3D(result->type,
						0,
						GetGLTextureFormat(Desc->Format),
						Data->width,
						Data->height,
						Data->depth,
						0,
						GetGLTextureFormat(Desc->Format),
						GL_UNSIGNED_BYTE,
						Data->databuf);
				}
				glTexParameteri(result->type, GL_TEXTURE_WRAP_S, GetGLTextureWrap(Desc->Wrap));
				glTexParameteri(result->type, GL_TEXTURE_WRAP_T, GetGLTextureWrap(Desc->Wrap));

				switch (Desc->Filter)
				{
				case TextureFilter::Point2D:
				{
					glTexParameteri(result->type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(result->type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					break;
				}
				case TextureFilter::Linear2D:
				{
					glTexParameteri(result->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(result->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					break;
				}
				case TextureFilter::Point:
				{
					glGenerateMipmap(result->type);
					glTexParameteri(result->type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(result->type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					break;
				}
				case TextureFilter::Bilinear:
				{
					glGenerateMipmap(result->type);
					glTexParameteri(result->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(result->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					break;
				}
				case TextureFilter::Trilinear:
				{
					glGenerateMipmap(result->type);
					glTexParameteri(result->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(result->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					break;
				}

				default:
					break;
				}

				if (Core::ContextDesc::SupportAnisotropic == true)
				{
					switch (Desc->AnisoFilter)
					{
					case AnisotropicFilter::AnisotropicX2:
					{
						glTexParameterf(result->type, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(2.0f, Core::ContextDesc::MaxAnisotropicLevel));
						break;
					}
					case AnisotropicFilter::AnisotropicX4:
					{
						glTexParameterf(result->type, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(4.0f, Core::ContextDesc::MaxAnisotropicLevel));
						break;
					}
					case AnisotropicFilter::AnisotropicX8:
					{
						glTexParameterf(result->type, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(8.0f, Core::ContextDesc::MaxAnisotropicLevel));
						break;
					}
					case AnisotropicFilter::AnisotropicX16:
					{
						glTexParameterf(result->type, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(16.0f, Core::ContextDesc::MaxAnisotropicLevel));
						break;
					}
					default:
						break;
					}
				}
				
				glBindTexture(result->type, 0);
			}

			void GLTexture::Create(GLTexture * result,const std::array<NuclearEngine::Texture_Data*, 6>& data, NuclearEngine::Texture_Desc* Desc)
			{
				glGenTextures(1, &result->textureID);
				result->type = GL_TEXTURE_CUBE_MAP;

				glBindTexture(GL_TEXTURE_CUBE_MAP, result->textureID);

				for (GLuint i = 0; i < data.size(); i++)
				{
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
						GetGLTextureFormat(Desc->Format),
						data[i]->width,
						data[i]->height,
						0,
						GetGLTextureFormat(Desc->Format),
						GL_UNSIGNED_BYTE,
						data[i]->databuf);
				}
				switch (Desc->Filter)
				{
				case TextureFilter::Point2D:
				{
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					break;
				}
				case TextureFilter::Linear2D:
				{
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					break;
				}
				case TextureFilter::Point:
				{
					glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
					break;
				}
				case TextureFilter::Bilinear:
				{
					glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					break;
				}
				case TextureFilter::Trilinear:
				{
					glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
					break;
				}

				default:
					break;
				}


				if (Core::ContextDesc::SupportAnisotropic == true)
				{
					switch (Desc->AnisoFilter)
					{
					case AnisotropicFilter::AnisotropicX2:
					{
						glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(2.0f, Core::ContextDesc::MaxAnisotropicLevel));
						break;
					}
					case AnisotropicFilter::AnisotropicX4:
					{
						glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(4.0f, Core::ContextDesc::MaxAnisotropicLevel));
						break;
					}
					case AnisotropicFilter::AnisotropicX8:
					{
						glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(8.0f, Core::ContextDesc::MaxAnisotropicLevel));
						break;
					}
					case AnisotropicFilter::AnisotropicX16:
					{
						glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(16.0f, Core::ContextDesc::MaxAnisotropicLevel));
						break;
					}
					default:
						break;
					}
				}

				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GetGLTextureWrap(Desc->Wrap));
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GetGLTextureWrap(Desc->Wrap));
				glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GetGLTextureWrap(Desc->Wrap));

				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

			}

			void GLTexture::VSBind(unsigned int index)
			{
				glActiveTexture(GL_TEXTURE0 + index);
				glBindTexture(type, textureID);
			}

			void GLTexture::VSBind(const char * samplerName, GLShader * shader, unsigned int index)
			{
				glActiveTexture(GL_TEXTURE0 + index);
				glBindTexture(type, textureID);
				glUniform1i(glGetUniformLocation(shader->_ProgramID, samplerName), index);
			}
			void GLTexture::PSBind(unsigned int index)
			{
				glActiveTexture(GL_TEXTURE0 + index);
				glBindTexture(type, textureID);
			}

			void GLTexture::PSBind(const char * samplerName, GLShader * shader, unsigned int index)
			{
				glActiveTexture(GL_TEXTURE0 + index);
				glBindTexture(type, textureID);
				glUniform1i(glGetUniformLocation(shader->_ProgramID, samplerName), index);
			}
			void GLTexture::GSBind(unsigned int index)
			{
				glActiveTexture(GL_TEXTURE0 + index);
				glBindTexture(type, textureID);
			}

			void GLTexture::GSBind(const char * samplerName, GLShader * shader, unsigned int index)
			{
				glActiveTexture(GL_TEXTURE0 + index);
				glBindTexture(type, textureID);
				glUniform1i(glGetUniformLocation(shader->_ProgramID, samplerName), index);
			}

			unsigned int GLTexture::GLGetTextureID()
			{
				return textureID;
			}
			int GetGLTextureFormat(TextureFormat format)
			{
				switch (format)
				{
				case  TextureFormat::R8: return GL_RED;
				case  TextureFormat::R8G8: return GL_RG;
				case  TextureFormat::R8G8B8: return GL_RGB;
				case  TextureFormat::R8G8B8A8: return GL_RGBA;
				default: return GL_RGBA;
				}

			}

			int GetGLTextureWrap(TextureWrap textureWrap)
			{
				switch (textureWrap)
				{
				case TextureWrap::Repeat: return GL_REPEAT;
				case TextureWrap::MirroredReapeat: return GL_MIRRORED_REPEAT;
				case TextureWrap::ClampToEdge: return GL_CLAMP_TO_EDGE;
				case TextureWrap::ClampToBorder: return GL_CLAMP_TO_BORDER;
				default: return GL_REPEAT;
				}

			}

		}
	}
}

#endif