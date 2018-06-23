#include <Engine/Graphics/API/OpenGL\GLUnifiedShader.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <Engine/Graphics/API/Shader_Types.h>
#include <Engine/Graphics/API/OpenGL\GLConstantBuffer.h>

#include <Engine/Graphics/API/OpenGL\GLError.h>
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace OpenGL
			{
				bool CheckShaderErrors(GLuint shader, const char* type);
				bool CreateShaderID(const std::string& sourcecode, ShaderType type, GLuint& ShaderID);

				void GLUnifiedShader::Fix_Reflected_ConstantBuffer_Slot(GLUnifiedShader* result, const BinaryShaderBlob& blob)
				{
					/*for (auto it = blob.Reflection.ConstantBuffers.begin(); it != blob.Reflection.ConstantBuffers.end(); it++)
					{
						it->second.BindingSlot = glGetUniformBlockIndex(result->ShaderID, it->first.c_str());
						GLCheckError();
					}*/
				}

				GLUnifiedShader::GLUnifiedShader()
				{
					ShaderID = 0;
				}

				GLUnifiedShader::~GLUnifiedShader()
				{
					ShaderID = 0;
				}		
				
				bool GLUnifiedShader::Create(GLUnifiedShader* result, const BinaryShaderBlob& VS_Desc, const BinaryShaderBlob& PS_Desc)
				{
					GLuint vertex = 0, fragment = 0;
					bool vsuccess = true, fsuccess = true;
					if (VS_Desc.isValid)
					{
						vsuccess = CreateShaderID(VS_Desc.GLSL_SourceCode, ShaderType::Vertex, vertex);
					}
					if (PS_Desc.isValid)
					{
						fsuccess = CreateShaderID(PS_Desc.GLSL_SourceCode, ShaderType::Pixel, fragment);						
					}
				

					bool lsuccess = false;
					if (vsuccess != false && fsuccess != false)
					{
						// Shader Program
						result->ShaderID = glCreateProgram();
						GLCheckError();
						if (VS_Desc.isValid)
						{
							GLCall(glAttachShader(result->ShaderID, vertex));
						}
						if (PS_Desc.isValid)
						{
							GLCall(glAttachShader(result->ShaderID, fragment));
						}
				
						GLCall(glLinkProgram(result->ShaderID));

						lsuccess = CheckShaderErrors(result->ShaderID, "Program");

						if (VS_Desc.isValid)
						{
							GLCall(glDeleteShader(vertex));
						}
						if (PS_Desc.isValid)
						{
							GLCall(glDeleteShader(fragment));
						}
					}

					return true;
				}

				void GLUnifiedShader::Delete(GLUnifiedShader * shader)
				{
					if (shader->ShaderID != 0)
					{
						GLCall(glDeleteProgram(shader->ShaderID));
					}
					shader->ShaderID = 0;
				}

				void GLUnifiedShader::SetConstantBuffer(GLConstantBuffer* ubuffer)
				{
					GLCall(glUniformBlockBinding(ShaderID, glGetUniformBlockIndex(ShaderID, ubuffer->GetName()), ubuffer->GetBindingIndex()));
				}

				void GLUnifiedShader::Bind()
				{
					GLCall(glUseProgram(this->ShaderID));
				}

				GLuint GLUnifiedShader::GetID()
				{
					return ShaderID;
				}

				bool CheckShaderErrors(GLuint shader, const char* type)
				{
					GLint success;
					char infoLog[1024];
					if (type != "Program")
					{
						GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
						if (!success)
						{
							GLCall(glGetShaderInfoLog(shader, 1024, NULL, infoLog));
							Log.Info("[GLShader] Compiling Error -- In ");
							Log.Info(type);
							Log.Info(" Shader - Info : \n");
							Log.Info(infoLog);
							Log.Info("\n");
						}
					}
					else
					{
						GLCall(glGetProgramiv(shader, GL_LINK_STATUS, &success));
						if (!success)
						{
							GLCall(glGetProgramInfoLog(shader, 1024, NULL, infoLog));
							Log.Info("[GLShader] Linking Error - Info : ");
							Log.Info(infoLog);
							Log.Info("\n");
						}
					}

					if (!success)
					{
						return false;
					}
					return true;
				}

				bool CreateShaderID(const std::string& sourcecode, ShaderType type, GLuint& ShaderID)
				{
					GLenum s_type;
					std::string m_type;

					switch (type)
					{
					case ShaderType::Vertex:
						s_type = GL_VERTEX_SHADER;
						m_type = "Vertex";
						break;
					case ShaderType::Pixel:
						s_type = GL_FRAGMENT_SHADER;
						m_type = "Pixel";
						break;
					case ShaderType::Geometry:
						s_type = GL_GEOMETRY_SHADER;
						m_type = "Geometry";
						break;
					default:
						Exceptions::NotImplementedException("Only (Vertex & Pixel & Geometry) shaders are supported!!\n");
						return false;
					}

					const char* code = sourcecode.c_str();
					ShaderID = glCreateShader(s_type);
					GLCheckError();
					GLCall(glShaderSource(ShaderID, 1, &code, NULL));
					GLCall(glCompileShader(ShaderID));
					return CheckShaderErrors(ShaderID, m_type.c_str());
				}


			}
		}
	}
}
#endif