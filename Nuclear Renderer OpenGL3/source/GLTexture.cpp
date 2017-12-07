#include <NuclearRendererOGL3\GLTexture.h>
#include <NuclearRendererOGL3\GLShader.h>
#include <NuclearRendererBase\NRBContext.h>
#include <NuclearCommon\Common_API_Types.h>
using namespace NuclearEngine;

namespace NuclearRenderer {

	int GetGLTextureFormat(TextureFormat format);
	int GetGLTextureWrap(TextureWrap textureWrap);
	int GetRTPrecision(RenderTargetPrecision precision);
	template<class T>
	const T& _min(const T& a, const T& b)
	{
		return (b < a) ? b : a;
	}

	GLTexture::GLTexture() : textureID(0)
	{
	}

	void GLTexture::Create(Texture_Data Data, Texture_Desc Desc)
	{
		if (Desc.Type == TextureType::Texture1D)
		{
			type = GL_TEXTURE_1D;
		}
		else if (Desc.Type == TextureType::Texture2D)
		{
			type = GL_TEXTURE_2D;
		}
		else if (Desc.Type == TextureType::Texture3D)
		{
			type = GL_TEXTURE_3D;
		}

		glGenTextures(1, &textureID);
		glBindTexture(type, textureID);

		if (Desc.Format == TextureFormat::R8)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		}
				

			if (Desc.Type == TextureType::Texture1D)
			{
				glTexImage1D(type,
					0,
					GetGLTextureFormat(Desc.Format),
					Data.width,
					0,
					GetGLTextureFormat(Desc.Format),
					GL_UNSIGNED_BYTE,
					Data.databuf);
			}
			else if (Desc.Type == TextureType::Texture2D)
			{
				glTexImage2D(type,
					0,
					GetGLTextureFormat(Desc.Format),
					Data.width,
					Data.height,
					0,
					GetGLTextureFormat(Desc.Format),
					GL_UNSIGNED_BYTE,
					Data.databuf);
			}
			else if (Desc.Type == TextureType::Texture3D)
			{
				glTexImage3D(type,
					0,
					GetGLTextureFormat(Desc.Format),
					Data.width,
					Data.height,
					Data.depth,
					0,
					GetGLTextureFormat(Desc.Format),
					GL_UNSIGNED_BYTE,
					Data.databuf);
			}
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GetGLTextureWrap(Desc.Wrap));
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GetGLTextureWrap(Desc.Wrap));

		switch (Desc.Filter)
		{
		case TextureFilter::Point2D:
		{
			glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			break;
		}
		case TextureFilter::Linear2D:
		{
			glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			break;
		}
		case TextureFilter::Point:
		{
			glGenerateMipmap(type);
			glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			break;
		}
		case TextureFilter::Bilinear:
		{
			glGenerateMipmap(type);
			glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			break;
		}
		case TextureFilter::Trilinear:
		{
			glGenerateMipmap(type);
			glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			break;
		}
				
		default:
			break;
		}

		if (ContextDesc::SupportAnisotropic == true)
		{
			switch (Desc.AnisoFilter)
			{
			case AnisotropicFilter::AnisotropicX2:
			{
				glTexParameterf(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, _min(2.0f, ContextDesc::MaxAnisotropicLevel));

				break;
			}
			case AnisotropicFilter::AnisotropicX4:
			{
				glTexParameterf(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, _min(4.0f, ContextDesc::MaxAnisotropicLevel));
				break;
			}
			case AnisotropicFilter::AnisotropicX8:
			{
				glTexParameterf(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, _min(8.0f, ContextDesc::MaxAnisotropicLevel));
				break;
			}
			case AnisotropicFilter::AnisotropicX16:
			{
				glTexParameterf(type, GL_TEXTURE_MAX_ANISOTROPY_EXT, _min(16.0f, ContextDesc::MaxAnisotropicLevel));
				//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -10.4f);
				break;
			}
			default:
				break;
			}
		}

		glBindTexture(type, 0);
	}

	void GLTexture::Delete()
	{
		glDeleteTextures(1, &textureID);
	}

	void GLTexture::VSBind(unsigned int index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(type, textureID);
	}

	void GLTexture::VSBind(const char * samplerName, NRBShader * shader, unsigned int index)
	{		
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(type, textureID);
		glUniform1i(glGetUniformLocation(shader->GetGLShaderID(), samplerName), index);		
	}
	void GLTexture::PSBind(unsigned int index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(type, textureID);
	}

	void GLTexture::PSBind(const char * samplerName, NRBShader * shader, unsigned int index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(type, textureID);
		glUniform1i(glGetUniformLocation(shader->GetGLShaderID(), samplerName), index);
	}
	void GLTexture::GSBind(unsigned int index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(type, textureID);
	}

	void GLTexture::GSBind(const char * samplerName, NRBShader * shader, unsigned int index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(type, textureID);
		glUniform1i(glGetUniformLocation(shader->GetGLShaderID(), samplerName), index);
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
	int GetRTPrecision(RenderTargetPrecision precision)
	{
		switch (precision)
		{
		case RenderTargetPrecision::Float: return GL_FLOAT;
		case RenderTargetPrecision::Half_Float: return GL_HALF_FLOAT;
		case RenderTargetPrecision::Unsigned_Int: return GL_UNSIGNED_INT;
		default: return GL_FLOAT;
		}
	}
}