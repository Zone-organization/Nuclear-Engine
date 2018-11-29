#include "Engine/Graphics/API/OpenGL/GLSampler.h"

#ifdef NE_COMPILE_CORE_OPENGL

#include <Engine\Graphics\API\Context.h>

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
					mSamplerID = 0;
				}
				GLSampler::~GLSampler()
				{
					mSamplerID = 0;
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
					glGenSamplers(1, &result->mSamplerID);

					glSamplerParameteri(result->mSamplerID, GL_TEXTURE_WRAP_S, GetGLTextureWrap(desc.WrapU));
					glSamplerParameteri(result->mSamplerID, GL_TEXTURE_WRAP_T, GetGLTextureWrap(desc.WrapV));
					glSamplerParameteri(result->mSamplerID, GL_TEXTURE_WRAP_R, GetGLTextureWrap(desc.WrapW));

					switch (desc.Filter)
					{
					case TextureFilter::Point2D:
					{
						glSamplerParameteri(result->mSamplerID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
						glSamplerParameteri(result->mSamplerID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
						break;
					}
					case TextureFilter::Linear2D:
					{
						glSamplerParameteri(result->mSamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glSamplerParameteri(result->mSamplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
						break;
					}
					case TextureFilter::Point:
					{
						glSamplerParameteri(result->mSamplerID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
						glSamplerParameteri(result->mSamplerID, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
						break;
					}
					case TextureFilter::Bilinear:
					{
						glSamplerParameteri(result->mSamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glSamplerParameteri(result->mSamplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						break;
					}
					case TextureFilter::Trilinear:
					{
						glSamplerParameteri(result->mSamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glSamplerParameteri(result->mSamplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						break;
					}

					default:
						glSamplerParameteri(result->mSamplerID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
						glSamplerParameteri(result->mSamplerID, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
						break;
					}
					if (Graphics::API::ContextDesc::MaxAnisotropicLevel != 0.0f)
					{
						if (desc.AnisoFilter != AnisotropicFilter::None)
						{
							glSamplerParameterf(result->mSamplerID, GL_TEXTURE_MAX_ANISOTROPY_EXT, Math::min(static_cast<float>(desc.AnisoFilter), Graphics::API::ContextDesc::MaxAnisotropicLevel));
						}
					}

					//glSamplerParameterf(result->mSamplerID, GL_TEXTURE_MIN_LOD, desc.MinLOD);
					//glSamplerParameterf(result->mSamplerID, GL_TEXTURE_MAX_LOD, desc.MaxLOD);

					glSamplerParameteri(result->mSamplerID, GL_TEXTURE_COMPARE_MODE, GL_NONE);
					GLfloat bcolor[4] = { desc.BorderColor.r,desc.BorderColor.g,desc.BorderColor.b,desc.BorderColor.a };
					glSamplerParameterfv(result->mSamplerID, GL_TEXTURE_BORDER_COLOR, bcolor);
				}
				void GLSampler::Delete(GLSampler * result)
				{
					glDeleteSamplers(1, &result->mSamplerID);

				}
				void GLSampler::VSBind(unsigned int Slot)
				{
					glBindSampler(Slot, mSamplerID);
				}
				void GLSampler::GSBind(unsigned int Slot)
				{
					glBindSampler(Slot, mSamplerID);

				}
				void GLSampler::PSBind(unsigned int Slot)
				{
					glBindSampler(Slot, mSamplerID);
				}
			}
		}
	}
}
#endif