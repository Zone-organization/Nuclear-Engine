#include "Graphics/API/OpenGL\GLSampler.h"

#ifdef NE_COMPILE_CORE_OPENGL
#include <Graphics/API/OpenGL\GLError.h>
#include <Graphics\API\Context.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace OpenGL
			{
				GLSampler::GLSampler()
				{
					SamplerID = 0;
				}
				GLSampler::~GLSampler()
				{
					SamplerID = 0;
				}
				int GetGLTextureWrap(TextureWrap textureWrap)
				{
					switch (textureWrap)
					{
					case TextureWrap::Repeat: return GL_REPEAT;
					case TextureWrap::MirroredRepeat: return GL_MIRRORED_REPEAT;
					case TextureWrap::ClampToEdge: return GL_CLAMP_TO_EDGE;
					case TextureWrap::ClampToBorder: return GL_CLAMP_TO_BORDER;
					default: return GL_REPEAT;
					}
				}
				void GLSampler::Create(GLSampler * result, const SamplerDesc & desc)
				{
					glGenSamplers(1, &result->SamplerID);

					glSamplerParameteri(result->SamplerID, GL_TEXTURE_WRAP_S, GetGLTextureWrap(desc.WrapU));
					glSamplerParameteri(result->SamplerID, GL_TEXTURE_WRAP_T, GetGLTextureWrap(desc.WrapV));
					glSamplerParameteri(result->SamplerID, GL_TEXTURE_WRAP_R, GetGLTextureWrap(desc.WrapW));

					switch (desc.Filter)
					{
					case TextureFilter::Point2D:
					{
						GLCall(glSamplerParameteri(result->SamplerID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
						GLCall(glSamplerParameteri(result->SamplerID, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
						break;
					}
					case TextureFilter::Linear2D:
					{
						GLCall(glSamplerParameteri(result->SamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
						GLCall(glSamplerParameteri(result->SamplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
						break;
					}
					case TextureFilter::Point:
					{
						GLCall(glSamplerParameteri(result->SamplerID, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
						GLCall(glSamplerParameteri(result->SamplerID, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR));
						break;
					}
					case TextureFilter::Bilinear:
					{
						GLCall(glSamplerParameteri(result->SamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
						GLCall(glSamplerParameteri(result->SamplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
						break;
					}
					case TextureFilter::Trilinear:
					{
						GLCall(glSamplerParameteri(result->SamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
						GLCall(glSamplerParameteri(result->SamplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
						break;
					}

					default:
						GLCall(glSamplerParameteri(result->SamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
						GLCall(glSamplerParameteri(result->SamplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
						break;
					}
					if (Graphics::API::ContextDesc::MaxAnisotropicLevel != 0.0f)
					{
						if (desc.AnisoFilter != AnisotropicFilter::None)
						{
							glSamplerParameterf(result->SamplerID, GL_TEXTURE_MAX_ANISOTROPY_EXT, Math::min(static_cast<float>(desc.AnisoFilter), Graphics::API::ContextDesc::MaxAnisotropicLevel));
						}
					}

					//glSamplerParameterf(result->SamplerID, GL_TEXTURE_MIN_LOD, desc.MinLOD);
					//glSamplerParameterf(result->SamplerID, GL_TEXTURE_MAX_LOD, desc.MaxLOD);

					glSamplerParameteri(result->SamplerID, GL_TEXTURE_COMPARE_MODE, GL_NONE);
					GLfloat bcolor[4] = { desc.BorderColor.r,desc.BorderColor.g,desc.BorderColor.b,desc.BorderColor.a };
					glSamplerParameterfv(result->SamplerID, GL_TEXTURE_BORDER_COLOR, bcolor);
				}
				void GLSampler::Delete(GLSampler * result)
				{
					glDeleteSamplers(1, &result->SamplerID);

				}
				void GLSampler::VSBind(unsigned int Slot)
				{
					glBindSampler(Slot, SamplerID);
				}
				void GLSampler::GSBind(unsigned int Slot)
				{
					glBindSampler(Slot, SamplerID);

				}
				void GLSampler::PSBind(unsigned int Slot)
				{
					glBindSampler(Slot, SamplerID);
				}
			}
		}
	}
}
#endif