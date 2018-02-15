#include <API\OpenGL\GLTexture.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <API\OpenGL\GLShader.h>
#include <API\OpenGL\GLVertexShader.h>
#include <API\OpenGL\GLPixelShader.h>

#include <Core\Context.h>
#include <API\OpenGL\GLError.h>

namespace NuclearEngine
{
	namespace API
	{
		namespace OpenGL
		{

			int GetGLFormat(Format format);
			int GetGLTextureWrap(TextureWrap textureWrap);
			
			GLTexture::GLTexture() : textureID(0) , type(0)
			{
			}

			GLTexture::~GLTexture()
			{
				textureID = 0;
				type = 0;
			}

			void GLTexture::Create(GLTexture* result,const Texture_Data& Data, const Texture_Desc& Desc)
			{
				if (Desc.Type == TextureType::Texture1D)
				{
					result->type = GL_TEXTURE_1D;
				}
				else if (Desc.Type == TextureType::Texture2D)
				{
					result->type = GL_TEXTURE_2D;
				}
				else if (Desc.Type == TextureType::Texture3D)
				{
					result->type = GL_TEXTURE_3D;
				}

				GLCall(glGenTextures(1, &result->textureID));
				GLCall(glBindTexture(result->type, result->textureID));

				if (Desc.Format == Format::R8)
				{
					GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
				}


				if (Desc.Type == TextureType::Texture1D)
				{
					GLCall(glTexImage1D(result->type,
						0,
						GetGLFormat(Desc.Format),
						Data.Width,
						0,
						GetGLFormat(Desc.Format),
						GL_UNSIGNED_BYTE,
						Data.Img_Data_Buf));
				}
				else if (Desc.Type == TextureType::Texture2D)
				{
					GLCall(glTexImage2D(result->type,
						0,
						GetGLFormat(Desc.Format),
						Data.Width,
						Data.Height,
						0,
						GetGLFormat(Desc.Format),
						GL_UNSIGNED_BYTE,
						Data.Img_Data_Buf));
				}
				else if (Desc.Type == TextureType::Texture3D)
				{
					GLCall(glTexImage3D(result->type,
						0,
						GetGLFormat(Desc.Format),
						Data.Width,
						Data.Height,
						Data.Depth,
						0,
						GetGLFormat(Desc.Format),
						GL_UNSIGNED_BYTE,
						Data.Img_Data_Buf));
				}
				GLCall(glTexParameteri(result->type, GL_TEXTURE_WRAP_S, GetGLTextureWrap(Desc.Wrap)));
				GLCall(glTexParameteri(result->type, GL_TEXTURE_WRAP_T, GetGLTextureWrap(Desc.Wrap)));

				switch (Desc.Filter)
				{
				case TextureFilter::Point2D:
				{
					GLCall(glTexParameteri(result->type, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
					GLCall(glTexParameteri(result->type, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
					break;
				}
				case TextureFilter::Linear2D:
				{
					GLCall(glTexParameteri(result->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
					GLCall(glTexParameteri(result->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
					break;
				}
				case TextureFilter::Point:
				{
					GLCall(glGenerateMipmap(result->type));
					GLCall(glTexParameteri(result->type, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
					GLCall(glTexParameteri(result->type, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
					break;
				}
				case TextureFilter::Bilinear:
				{
					GLCall(glGenerateMipmap(result->type));
					GLCall(glTexParameteri(result->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
					GLCall(glTexParameteri(result->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
					break;
				}
				case TextureFilter::Trilinear:
				{
					GLCall(glGenerateMipmap(result->type));
					GLCall(glTexParameteri(result->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
					GLCall(glTexParameteri(result->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
					break;
				}

				default:
					break;
				}

				if (Core::ContextDesc::MaxAnisotropicLevel != 0.0f)
				{
					switch (Desc.AnisoFilter)
					{
					case AnisotropicFilter::AnisotropicX2:
					{
						GLCall(glTexParameterf(result->type, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(2.0f, Core::ContextDesc::MaxAnisotropicLevel)));
						break;
					}
					case AnisotropicFilter::AnisotropicX4:
					{
						GLCall(glTexParameterf(result->type, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(4.0f, Core::ContextDesc::MaxAnisotropicLevel)));
						break;
					}
					case AnisotropicFilter::AnisotropicX8:
					{
						GLCall(glTexParameterf(result->type, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(8.0f, Core::ContextDesc::MaxAnisotropicLevel)));
						break;
					}
					case AnisotropicFilter::AnisotropicX16:
					{
						GLCall(glTexParameterf(result->type, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(16.0f, Core::ContextDesc::MaxAnisotropicLevel)));
						break;
					}
					default:
						break;
					}
				}
				if (Desc.Format == Format::R8)
				{
					GLCall(glPixelStorei(GL_PACK_ALIGNMENT, 1));
				}
				GLCall(glBindTexture(result->type, 0));
			}

			void GLTexture::Create(GLTexture * result,const std::array<API::Texture_Data, 6>& data, const Texture_Desc& Desc)
			{
				GLCall(glGenTextures(1, &result->textureID));
				result->type = GL_TEXTURE_CUBE_MAP;

				GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, result->textureID));

				for (GLuint i = 0; i < data.size(); i++)
				{
					GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
						GetGLFormat(Desc.Format),
						data[i].Width,
						data[i].Height,
						0,
						GetGLFormat(Desc.Format),
						GL_UNSIGNED_BYTE,
						data[i].Img_Data_Buf));
				}
				switch (Desc.Filter)
				{
				case TextureFilter::Point2D:
				{
					GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
					GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
					break;
				}
				case TextureFilter::Linear2D:
				{
					GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
					GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
					break;
				}
				case TextureFilter::Point:
				{
					GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
					GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
					GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
					break;
				}
				case TextureFilter::Bilinear:
				{
					GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
					GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
					GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
					break;
				}
				case TextureFilter::Trilinear:
				{
					GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
					GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
					GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
					break;
				}

				default:
					break;
				}


				if (Core::ContextDesc::MaxAnisotropicLevel != 0.0f)
				{
					switch (Desc.AnisoFilter)
					{
					case AnisotropicFilter::AnisotropicX2:
					{
						GLCall(glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(2.0f, Core::ContextDesc::MaxAnisotropicLevel)));
						break;
					}
					case AnisotropicFilter::AnisotropicX4:
					{
						GLCall(glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(4.0f, Core::ContextDesc::MaxAnisotropicLevel)));
						break;
					}
					case AnisotropicFilter::AnisotropicX8:
					{
						GLCall(glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(8.0f, Core::ContextDesc::MaxAnisotropicLevel)));
						break;
					}
					case AnisotropicFilter::AnisotropicX16:
					{
						GLCall(glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, std::min(16.0f, Core::ContextDesc::MaxAnisotropicLevel)));
						break;
					}
					default:
						break;
					}
				}

				GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GetGLTextureWrap(Desc.Wrap)));
				GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GetGLTextureWrap(Desc.Wrap)));
				GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GetGLTextureWrap(Desc.Wrap)));
		
				GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));

			}

			void GLTexture::Delete(GLTexture * texture)
			{
				if (texture->textureID != 0)
				{
					GLCall(glDeleteTextures(1, &texture->textureID));
				}

				texture->textureID = 0;
				texture->type = 0;
			}

			void GLTexture::SetInShader(const char * samplerName, GLVertexShader * shader, unsigned int index)
			{
				GLCall(glActiveTexture(GL_TEXTURE0 + index));
				GLCall(glBindTexture(type, textureID));
				GLCall(glActiveShaderProgram(GLVertexShader::GetPipelineID(),shader->_ProgramID));
				GLCall(glUniform1i(glGetUniformLocation(shader->_ProgramID, samplerName), index));
			}

			void GLTexture::SetInShader(const char * samplerName, GLPixelShader * shader, unsigned int index)
			{
				GLCall(glActiveTexture(GL_TEXTURE0 + index));
				GLCall(glBindTexture(type, textureID));
				GLCall(glActiveShaderProgram(GLVertexShader::GetPipelineID(), shader->_ProgramID));
				GLCall(glUniform1i(glGetUniformLocation(shader->_ProgramID, samplerName), index));
			}

			void GLTexture::VSBind(unsigned int index)
			{
				GLCall(glActiveTexture(GL_TEXTURE0 + index));
				GLCall(glBindTexture(type, textureID));
			}

			void GLTexture::VSBind(const char * samplerName, GLShader * shader, unsigned int index)
			{
				GLCall(glActiveTexture(GL_TEXTURE0 + index));
				GLCall(glBindTexture(type, textureID));
				GLCall(glUniform1i(glGetUniformLocation(shader->_ProgramID, samplerName), index));
			}
			void GLTexture::PSBind(unsigned int index)
			{
				GLCall(glActiveTexture(GL_TEXTURE0 + index));
				GLCall(glBindTexture(type, textureID));
			}

			void GLTexture::PSBind(const char * samplerName, GLShader * shader, unsigned int index)
			{
				GLCall(glActiveTexture(GL_TEXTURE0 + index));
				GLCall(glBindTexture(type, textureID));
				GLCall(glUniform1i(glGetUniformLocation(shader->_ProgramID, samplerName), index));
			}
			void GLTexture::GSBind(unsigned int index)
			{
				GLCall(glActiveTexture(GL_TEXTURE0 + index));
				GLCall(glBindTexture(type, textureID));
			}

			void GLTexture::GSBind(const char * samplerName, GLShader * shader, unsigned int index)
			{
				GLCall(glActiveTexture(GL_TEXTURE0 + index));
				GLCall(glBindTexture(type, textureID));
				GLCall(glUniform1i(glGetUniformLocation(shader->_ProgramID, samplerName), index));
			}

			unsigned int GLTexture::GLGetTextureID()
			{
				return textureID;
			}
			int GetGLFormat(Format format)
			{
				switch (format)
				{
				case  Format::R8: return GL_RED;
				case  Format::R8G8: return GL_RG;
				case  Format::R8G8B8: return GL_RGB;
				case  Format::R8G8B8A8: return GL_RGBA;
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