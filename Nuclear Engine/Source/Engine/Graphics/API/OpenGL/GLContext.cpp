#include <Engine/Graphics/API/OpenGL\GLContext.h>
#include <SFML\Window.hpp>

#include <sstream>
#ifdef NE_COMPILE_CORE_OPENGL
static GLuint GL_PRIMITIVE_TYPE;


namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace OpenGL
			{
				void APIENTRY glDebugOutput(GLenum source,
					GLenum type,
					GLuint id,
					GLenum severity,
					GLsizei length,
					const GLchar *message,
					const void *userParam)
				{
					// ignore non-significant error/warning codes
					if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

					std::stringstream info;

					info << "\n--------------------------\n" << std::endl;
					info << "OpenGL Debug message (" << id << "): " << message << '\n';

					switch (source)
					{
					case GL_DEBUG_SOURCE_API:             info << "Source: API"; break;
					case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   info << "Source: Window System"; break;
					case GL_DEBUG_SOURCE_SHADER_COMPILER: info << "Source: Shader Compiler"; break;
					case GL_DEBUG_SOURCE_THIRD_PARTY:     info << "Source: Third Party"; break;
					case GL_DEBUG_SOURCE_APPLICATION:     info << "Source: Application"; break;
					case GL_DEBUG_SOURCE_OTHER:           info << "Source: Other"; break;
					} info << std::endl;

					switch (type)
					{
					case GL_DEBUG_TYPE_ERROR:               info << "Type: Error"; break;
					case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: info << "Type: Deprecated Behaviour"; break;
					case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  info << "Type: Undefined Behaviour"; break;
					case GL_DEBUG_TYPE_PORTABILITY:         info << "Type: Portability"; break;
					case GL_DEBUG_TYPE_PERFORMANCE:         info << "Type: Performance"; break;
					case GL_DEBUG_TYPE_MARKER:              info << "Type: Marker"; break;
					case GL_DEBUG_TYPE_PUSH_GROUP:          info << "Type: Push Group"; break;
					case GL_DEBUG_TYPE_POP_GROUP:           info << "Type: Pop Group"; break;
					case GL_DEBUG_TYPE_OTHER:               info << "Type: Other"; break;
					} info << std::endl;

					switch (severity)
					{
					case GL_DEBUG_SEVERITY_HIGH:         info << "Severity: high"; break;
					case GL_DEBUG_SEVERITY_MEDIUM:       info << "Severity: medium"; break;
					case GL_DEBUG_SEVERITY_LOW:          info << "Severity: low"; break;
					case GL_DEBUG_SEVERITY_NOTIFICATION: info << "Severity: notification"; break;
					} info << std::endl;
					info << "--------------------------" << std::endl;

					Log.Warning(info.str());
				}

				static GLenum gIndicesType = GL_UNSIGNED_INT;
				bool GLContext::Initialize(sf::Window * window)
				{
					bool result;
					result = window->setActive(true);
					if (result)
					{
						if (window->getSettings().attributeFlags == sf::ContextSettings::Debug)
						{
							glEnable(GL_DEBUG_OUTPUT);
							glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
							glDebugMessageCallback(glDebugOutput, nullptr);
							glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
						}
					}

					return result;
				}
				void GLContext::SetPrimitiveType(int primitivetype)
				{
					switch (primitivetype)
					{
					case 0:
					{
						GL_PRIMITIVE_TYPE = GL_POINTS;
						break;
					}
					case 1:
					{
						GL_PRIMITIVE_TYPE = GL_LINES;
						break;
					}
					case 2:
					{
						GL_PRIMITIVE_TYPE = GL_LINE_STRIP;
						break;
					}
					case 3:
					{
						GL_PRIMITIVE_TYPE = GL_TRIANGLES;
						break;
					}
					case 4:
					{
						GL_PRIMITIVE_TYPE = GL_TRIANGLE_STRIP;
						break;
					}
					default:
						break;
					}
				}

				void GLContext::Clear(Graphics::Color color, Uint32 flags, float depth, unsigned int stencil)
				{
					GLbitfield flaggl = 0;
					//Color buffer
					if (CHECK_BIT(flags, 0))
					{
						glClearColor(color.r, color.g, color.b, color.a);
						flaggl = flaggl | GL_COLOR_BUFFER_BIT;
					}
					//Depth Buffer
					if (CHECK_BIT(flags, 1))
					{
						glClearDepth(depth);
						flaggl = flaggl | GL_DEPTH_BUFFER_BIT;
					}
					//Stencil Buffer
					if (CHECK_BIT(flags, 2))
					{
						glClearStencil(stencil);
						flaggl = flaggl | GL_STENCIL_BUFFER_BIT;
					}
					glClear(flaggl);
				}

				void GLContext::Draw(unsigned int count)
				{
					glDrawArrays(GL_PRIMITIVE_TYPE, 0, count);

				}

				void GLContext::DrawIndexed(unsigned int vertexCount, unsigned int StartIndexLocation, int BaseVertexLocation)
				{
					glDrawElements(GL_PRIMITIVE_TYPE, vertexCount, gIndicesType, (int*)BaseVertexLocation);
				}

				void GLContext::SetIndicesType(const IndicesFormat & type)
				{
					if (type == IndicesFormat::UINT_R8)
					{
						gIndicesType = GL_UNSIGNED_BYTE;
					}
					else if (type == IndicesFormat::UINT_R16)
					{
						gIndicesType = GL_UNSIGNED_SHORT;
					}
					else
						gIndicesType = GL_UNSIGNED_INT;
				}
				void GLContext::SetViewPort(int x, int y, int width, int height)
				{
					glViewport(x, y, width, height);
				}

				void GLContext::SetScissors(int x, int y, int width, int height)
				{
					glScissor(x, y, width, height);
				}

				RasterizerState GLContext::GetRasterizerState()
				{
					RasterizerState Obj;
					Obj.GLObject.mCullEnabled = glIsEnabled(GL_CULL_FACE);

					return Obj;
				}

				BlendState GLContext::GetBlendState()
				{
					BlendState Obj;					

					for (unsigned int i = 0; i < 8; i++)
					{
						GLenum last_blend_src_rgb; glGetIntegeri_v(GL_BLEND_SRC_RGB,i, (GLint*)&last_blend_src_rgb);
						GLenum last_blend_dst_rgb; glGetIntegeri_v(GL_BLEND_DST_RGB,i, (GLint*)&last_blend_dst_rgb);
						GLenum last_blend_src_alpha; glGetIntegeri_v(GL_BLEND_SRC_ALPHA,i, (GLint*)&last_blend_src_alpha);
						GLenum last_blend_dst_alpha; glGetIntegeri_v(GL_BLEND_DST_ALPHA,i, (GLint*)&last_blend_dst_alpha);
						GLenum last_blend_equation_rgb; glGetIntegeri_v(GL_BLEND_EQUATION_RGB,i, (GLint*)&last_blend_equation_rgb);
						GLenum last_blend_equation_alpha; glGetIntegeri_v(GL_BLEND_EQUATION_ALPHA,i, (GLint*)&last_blend_equation_alpha);
						GLboolean last_enable_blend = glIsEnabledi(GL_BLEND, 1);

						Obj.GLObject.targets[i].BlendEnable = last_enable_blend;
						Obj.GLObject.targets[i].SrcBlend = last_blend_src_rgb;
						Obj.GLObject.targets[i].DestBlend = last_blend_dst_rgb;
						Obj.GLObject.targets[i].BlendOp = last_blend_equation_rgb;
						Obj.GLObject.targets[i].SrcBlendAlpha = last_blend_src_alpha;
						Obj.GLObject.targets[i].DestBlendAlpha = last_blend_dst_alpha;
						Obj.GLObject.targets[i].BlendOpAlpha = last_blend_equation_alpha;
						//Obj.GLObject.targets[i].RenderTargetWriteMask = Obj.GLObject.type.RenderTarget[i].RenderTargetWriteMask;
					}

					return Obj;
				}

				DepthStencilState GLContext::GetDepthStencilState()
				{
					DepthStencilState Obj;
					Obj.GLObject.mDepthEnabled = glIsEnabled(GL_DEPTH_TEST);
					Obj.GLObject.mStencilEnabled = glIsEnabled(GL_STENCIL_TEST);

					return Obj;
				}

				Texture GLContext::GetPSTexture()
				{
					GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);

					return Texture();
				}

				Sampler GLContext::GetPSSampler()
				{
					GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
					//glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
					return Sampler();
				}

				PixelShader GLContext::GetPixelShader()
				{
					return PixelShader();
				}

				VertexShader GLContext::GetVertexShader()
				{
					return VertexShader();
				}

				IndexBuffer GLContext::GetIndexBuffer()
				{
					IndexBuffer Obj;
					glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, (GLint*)&Obj.GLObject.mIdxBuffer);
					return Obj;
				}

				VertexBuffer GLContext::GetVertexBuffer()
				{
					VertexBuffer Obj;
					glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&Obj.GLObject.mVAO);
					return Obj;
				}

				ConstantBuffer GLContext::GetConstantBuffer()
				{
					return ConstantBuffer();
				}

				void GLContext::Query_VAO_EBO_State()
				{
					std::string baseMessage;
					Log.Info("[OpenGL] Querying VAO state:");
					int vab, eabb, eabbs, mva, isOn(1), vaabb;
					glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vab);
					glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &eabb);
					glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &eabbs);

					baseMessage.append("  VAO: " + std::to_string(vab) + "\n");
					baseMessage.append("  IBO: " + std::to_string(eabb) + ", size=" + std::to_string(eabbs) + "\n");

					glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &mva);
					for (unsigned i = 0; i < (unsigned)mva; ++i)
					{
						glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &isOn);
						if (isOn)
						{
							glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, &vaabb);
							baseMessage.append("  attrib #" + std::to_string(i) + ": VBO=" + std::to_string(vaabb) + "\n");
						}
					}
					Log.Info(baseMessage);
				}

			}
		}
	}
}

#endif