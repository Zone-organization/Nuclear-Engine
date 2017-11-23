#include "NuclearRendererOGL3\GLTextureCube.h"
#include <NuclearRendererBase\NRBContext.h>
#include <NuclearRendererOGL3\GLShader.h>
#include <NuclearCommon\Common_API_Types.h>
using namespace NuclearEngine;

namespace NuclearRenderer {

	int _GetGLTextureFormat(TextureFormat format);
	int _GetGLTextureWrap(TextureWrap textureWrap);
	template<class T>
	const T& _min(const T& a, const T& b)
	{
		return (b < a) ? b : a;
	}

	GLTextureCube::GLTextureCube()
	{
	}
	bool GLTextureCube::Create(const std::array<NuclearEngine::Texture_Data, 6>& data, NuclearEngine::Texture_Desc Desc)
	{
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

		for (GLuint i = 0; i < data.size(); i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
				_GetGLTextureFormat(Desc.Format),
				data[i].width,
				data[i].height,
				0,
				_GetGLTextureFormat(Desc.Format),
				GL_UNSIGNED_BYTE,
				data[i].databuf);
		}
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


		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, _GetGLTextureWrap(Desc.Wrap));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, _GetGLTextureWrap(Desc.Wrap));
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, _GetGLTextureWrap(Desc.Wrap));


		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return true;
	}

	void GLTextureCube::Delete()
	{
		glDeleteTextures(1, &textureID);
	}
	void GLTextureCube::Bind(unsigned int index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	}
	void GLTextureCube::Bind(const char * samplerName, NRBShader * shader, unsigned int index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		glUniform1i(glGetUniformLocation(shader->GetGLShaderID(), samplerName), index);

	}
	void GLTextureCube::Unbind()
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	}
	void GLTextureCube::Unbind(unsigned int index)
	{
		glActiveTexture(GL_TEXTURE0 + index);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
	unsigned int GLTextureCube::GLGetTextureID()
	{
		return textureID;
	}

	int _GetGLTextureFormat(TextureFormat format)
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

	int _GetGLTextureWrap(TextureWrap textureWrap)
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