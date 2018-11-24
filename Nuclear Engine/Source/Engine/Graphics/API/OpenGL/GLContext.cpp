#include <Engine/Graphics/API/OpenGL\GLContext.h>


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
					glDrawElements(GL_PRIMITIVE_TYPE, vertexCount, GL_UNSIGNED_INT, 0);
				}


				void GLContext::SetViewPort(int x, int y, int width, int height)
				{
					glViewport(x, y, width, height);
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
					for (unsigned i = 0; i < mva; ++i)
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