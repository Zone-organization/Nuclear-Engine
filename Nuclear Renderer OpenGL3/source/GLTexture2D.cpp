#include <NuclearRendererOGL3\GLTexture2D.h>
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

	GLTexture2D::GLTexture2D() : textureID(0)
	{
	}

	bool GLTexture2D::Create(Texture_Data Data, Texture_Desc Desc)
	{
		
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		if (Desc.Format == TextureFormat::R8)
		{
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		}
	
		glTexImage2D(GL_TEXTURE_2D,
			0,
			GetGLTextureFormat(Desc.Format),
			Data.width,
			Data.height,
			0,
			GetGLTextureFormat(Desc.Format),
			GL_UNSIGNED_BYTE,
			Data.databuf);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetGLTextureWrap(Desc.Wrap));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetGLTextureWrap(Desc.Wrap));

		switch (Desc.Filter)
		{
		case TextureFilter::Point2D:
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			break;
		}
		case TextureFilter::Linear2D:
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			break;
		}
		case TextureFilter::Point:
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			break;
		}
		case TextureFilter::Bilinear:
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			break;
		}
		case TextureFilter::Trilinear:
		{
			glGenerateMipmap(GL_TEXTURE_2D);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
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
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, _min(2.0f, ContextDesc::MaxAnisotropicLevel));

				break;
			}
			case AnisotropicFilter::AnisotropicX4:
			{
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, _min(4.0f, ContextDesc::MaxAnisotropicLevel));
				break;
			}
			case AnisotropicFilter::AnisotropicX8:
			{
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, _min(8.0f, ContextDesc::MaxAnisotropicLevel));
				break;
			}
			case AnisotropicFilter::AnisotropicX16:
			{
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, _min(16.0f, ContextDesc::MaxAnisotropicLevel));
				//glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -10.4f);
				break;
			}
			default:
				break;
			}
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}

	bool GLTexture2D::Create(NuclearEngine::RenderTarget_Attachment_Desc Desc)
	{
		if (Desc.Read == true)
		{
			glGenRenderbuffers(1, &textureID);
			glBindRenderbuffer(GL_RENDERBUFFER, textureID);

			if (Desc.Format == TextureFormat::Depth)
			{
				
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, Desc.width, Desc.height);
			}
			else if (Desc.Format == TextureFormat::Stencil)
			{
				glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, Desc.width, Desc.height);
			}

			else
			{
				glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, Desc.width, Desc.width);
			}

			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			return true;
		}
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		if (Desc.Format == TextureFormat::Depth)
		{
			glTexImage2D(GL_TEXTURE_2D,
				0,
				GL_DEPTH_COMPONENT24,
				Desc.width,
				Desc.height,
				0,
				GL_DEPTH_COMPONENT,
				GetRTPrecision(Desc.precision),
				NULL);
		}
		else if (Desc.Format == TextureFormat::Stencil)
		{
			glTexImage2D(GL_TEXTURE_2D,
				0,
				GL_STENCIL_INDEX8,
				Desc.width,
				Desc.height,
				0,
				GL_STENCIL_INDEX,
				GetRTPrecision(Desc.precision),
				NULL);
		}
		else if (Desc.Format == TextureFormat::Depth_Stencil)
		{
			glTexImage2D(GL_TEXTURE_2D,
				0,
				GL_DEPTH24_STENCIL8,
				Desc.width,
				Desc.height,
				0,
				GL_DEPTH_STENCIL,
				GL_UNSIGNED_INT_24_8,
				NULL);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D,
				0,
				GetGLTextureFormat(Desc.Format),
				Desc.width,
				Desc.height,
				0,
				GetGLTextureFormat(Desc.Format),
				GetRTPrecision(Desc.precision),
				NULL);
		}
	

		switch (Desc.Filter)
		{
		case TextureFilter::Linear2D:
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			break;
		}
		default:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}

		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}

	void GLTexture2D::Delete()
	{
		glDeleteTextures(1, &textureID);
	}

	void GLTexture2D::Bind(unsigned int index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, textureID);
	}

	void GLTexture2D::Bind(const char * samplerName, NRBShader * shader, unsigned int index)
	{		
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, textureID);		
		glUniform1i(glGetUniformLocation(shader->GetGLShaderID(), samplerName), index);
		
	}

	void GLTexture2D::Unbind()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void GLTexture2D::Unbind(unsigned int index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	unsigned int GLTexture2D::GLGetTextureID()
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