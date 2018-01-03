#include <API\OpenGL\GLShader.h>

#ifdef NE_COMPILE_OPENGL3_3

#include <API\OpenGL\GLConstantBuffer.h>
#include <API\Shader_Types.h>

namespace NuclearEngine
{
	namespace API
	{
		namespace OpenGL
		{

			GLShader::GLShader()
			{
				_ProgramID = 0;
			}

			GLShader::~GLShader()
			{
				if (_ProgramID != 0)
				{
					glDeleteProgram(_ProgramID);
				}
				_ProgramID = 0;
			}

			bool CheckShaderErrors(GLuint shader, const char* type)
			{
				GLint success;
				char infoLog[1024];
				if (type != "Program")
				{
					glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
					if (!success)
					{
						glGetShaderInfoLog(shader, 1024, NULL, infoLog);
						Log->Info("[GLShader] Compiling Error -- In ");
						Log->Info(type);
						Log->Info(" Shader - Info : \n");
						Log->Info(infoLog);
						Log->Info("\n");
					}
				}
				else
				{
					glGetProgramiv(shader, GL_LINK_STATUS, &success);
					if (!success)
					{
						glGetProgramInfoLog(shader, 1024, NULL, infoLog);
						Log->Info("[GLShader] Linking Error - Info : ");
						Log->Info(infoLog);
						Log->Info("\n");
					}
				}

				if (!success)
				{
					return false;
				}
				return true;
			}

			void GLShader::Create(GLShader* result, BinaryShaderBlob* VertexShaderCode, BinaryShaderBlob* PixelShaderCode, BinaryShaderBlob* GeometryShaderCode)
			{
				if (VertexShaderCode != nullptr)
				{
					if (VertexShaderCode->Language != ShaderLanguage::GLSL)
					{
						Log->Error("[GLShader] OpenGL Renderer Backend expects all -Vertex- shaders in GLSL language!\n");
					}
				}
				if (PixelShaderCode != nullptr)
				{
					if (PixelShaderCode->Language != ShaderLanguage::GLSL)
					{
						Log->Error("[GLShader] OpenGL Renderer Backend expects all -Pixel- shaders in GLSL language!\n");
					}
				}
				if (GeometryShaderCode != nullptr)
				{
					if (GeometryShaderCode->Language != ShaderLanguage::GLSL)
					{
						Log->Error("[GLShader] OpenGL Renderer Backend expects all -Geometry- shaders in GLSL language!\n");

					}
				}
							
				GLuint vertex, fragment, geometry;
				bool vsuccess, fsuccess, gsuccess, lsuccess;

				// Vertex Shader
				const char* vscode = VertexShaderCode->GLSL_SourceCode.c_str();
				vertex = glCreateShader(GL_VERTEX_SHADER);
				glShaderSource(vertex, 1, &vscode, NULL);
				glCompileShader(vertex);
				vsuccess = CheckShaderErrors(vertex, "Vertex");

				const char* fscode = PixelShaderCode->GLSL_SourceCode.c_str();
				// Fragment Shader
				fragment = glCreateShader(GL_FRAGMENT_SHADER);
				glShaderSource(fragment, 1, &fscode, NULL);
				glCompileShader(fragment);
				fsuccess = CheckShaderErrors(fragment, "Fragment (Pixel)");

				if (GeometryShaderCode != nullptr)
				{
					const char* gscode = GeometryShaderCode->GLSL_SourceCode.c_str();
					geometry = glCreateShader(GL_GEOMETRY_SHADER);
					glShaderSource(geometry, 1, &gscode, NULL);
					glCompileShader(geometry);
					gsuccess = CheckShaderErrors(geometry, "Geometry");

				}
				if (GeometryShaderCode == nullptr)
				{
					// No geo shader provided so gsuccess is true to prevent  Run-Time Check Failure
					gsuccess = true;
				}

				if (vsuccess != false && fsuccess != false && gsuccess != false)
				{
					// Shader Program
					result->_ProgramID = glCreateProgram();
					glAttachShader(result->_ProgramID, vertex);
					glAttachShader(result->_ProgramID, fragment);
					if (GeometryShaderCode != nullptr)
					{
						glAttachShader(result->_ProgramID, geometry);
					}
					glLinkProgram(result->_ProgramID);

					lsuccess = CheckShaderErrors(result->_ProgramID, "Program");

					glDeleteShader(vertex);
					glDeleteShader(fragment);
					if (GeometryShaderCode != nullptr)
					{
						glDeleteShader(geometry);
					}						
				}

				glDeleteShader(vertex);
				glDeleteShader(fragment);
				if (GeometryShaderCode != nullptr)
				{
					glDeleteShader(geometry);
				}
			}

			void GLShader::SetConstantBuffer(GLConstantBuffer* ubuffer, ShaderType type)
			{
				glUniformBlockBinding(_ProgramID, glGetUniformBlockIndex(_ProgramID, ubuffer->GetName()), ubuffer->GetBindingIndex());
			}

			void GLShader::Bind()
			{
				glUseProgram(this->_ProgramID);
			}
		}
	}
}
#endif