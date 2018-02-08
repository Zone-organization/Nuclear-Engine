#include <API\OpenGL\GLShader.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <API\Shader_Types.h>
#include <API\OpenGL\GLConstantBuffer.h>
#include <API\OpenGL\GLError.h>
namespace NuclearEngine
{
	namespace API
	{
		namespace OpenGL
		{

			void GLShader::Fix_Reflected_ConstantBuffer_Slot(GLShader* result, BinaryShaderBlob* blob)
			{
				std::map<std::string, Reflected_Constantbuffer>::iterator it;
				for (it = blob->Reflection.ConstantBuffers.begin(); it != blob->Reflection.ConstantBuffers.end(); it++)
				{
					it->second.BindingSlot = glGetUniformBlockIndex(result->_ProgramID, it->first.c_str());
					GLCheckError();
				}
			}

			GLShader::GLShader()
			{
				_ProgramID = 0;
			}

			GLShader::~GLShader()
			{
				_ProgramID = 0;
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
						
			bool CreateShaderID(std::string sourcecode,ShaderType type, GLuint& ShaderID)
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
			
			void GLShader::Create(GLShader* result, ShaderDesc* desc)
			{
				Log.Info("[GLShader] Creating Shader: " + desc->Name + "\n");
			
				GLuint vertex = 0, fragment = 0, geometry = 0;
				bool vsuccess = true, fsuccess = true, gsuccess = true;
				if (desc->VertexShaderCode.Finished)
				{
					if (desc->VertexShaderCode.Language != API::ShaderLanguage::GLSL)
					{
						Log.Error("[GLShader] OpenGL Renderer Backend expects all -Vertex- shaders in GLSL language!\n");
					}
					else 
					{
						vsuccess = CreateShaderID(desc->VertexShaderCode.GLSL_SourceCode, ShaderType::Vertex, vertex);
					}
				}
				if (desc->PixelShaderCode.Finished)
				{
					if (desc->PixelShaderCode.Language != API::ShaderLanguage::GLSL)
					{
						Log.Error("[GLShader] OpenGL Renderer Backend expects all -Pixel- shaders in GLSL language!\n");
					}
					else {
						fsuccess = CreateShaderID(desc->PixelShaderCode.GLSL_SourceCode, ShaderType::Pixel, fragment);
					}
				}
				if (desc->GeometryShaderCode.Finished)
				{
					if (desc->GeometryShaderCode.Language != API::ShaderLanguage::GLSL)
					{
						Log.Error("[GLShader] OpenGL Renderer Backend expects all -Geometry- shaders in GLSL language!\n");

					}
					else {
						gsuccess = CreateShaderID(desc->GeometryShaderCode.GLSL_SourceCode, ShaderType::Geometry, geometry);
					}
				}					
				
				bool lsuccess = false;
				if (vsuccess != false && fsuccess != false && gsuccess != false)
				{
					// Shader Program
					result->_ProgramID = glCreateProgram();
					GLCheckError();
					if (desc->VertexShaderCode.Finished)
					{
						GLCall(glAttachShader(result->_ProgramID, vertex));
					}
					if (desc->PixelShaderCode.Finished)
					{
						GLCall(glAttachShader(result->_ProgramID, fragment));
					}
					if (desc->GeometryShaderCode.Finished)
					{
						GLCall(glAttachShader(result->_ProgramID, geometry));
					}
					
					GLCall(glLinkProgram(result->_ProgramID));

					lsuccess = CheckShaderErrors(result->_ProgramID, "Program");

					if (desc->VertexShaderCode.Finished)
					{
						GLCall(glDeleteShader(vertex));
					}
					if (desc->PixelShaderCode.Finished)
					{
						GLCall(glDeleteShader(fragment));
					}
					if (desc->GeometryShaderCode.Finished)
					{
						GLCall(glDeleteShader(geometry));
					}
											
				}
			}

			void GLShader::Delete(GLShader * shader)
			{
				if (shader->_ProgramID != 0)
				{
					GLCall(glDeleteProgram(shader->_ProgramID));
				}
				shader->_ProgramID = 0;
			}

			void GLShader::SetConstantBuffer(GLConstantBuffer* ubuffer,API::ShaderType type)
			{
				GLCall(glUniformBlockBinding(_ProgramID, glGetUniformBlockIndex(_ProgramID, ubuffer->GetName()), ubuffer->GetBindingIndex()));
			}

			void GLShader::Bind()
			{
				GLCall(glUseProgram(this->_ProgramID));
			}
		}
	}
}
#endif