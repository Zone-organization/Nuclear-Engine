#include <Engine/Graphics/API/OpenGL\GLPixelShader.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <Engine/Graphics/API/Shader_Types.h>
#include <Engine/Graphics/API/OpenGL\GLConstantBuffer.h>
#include <Engine/Graphics/API/OpenGL\GLVertexShader.h>

#include <Engine/Graphics/API/OpenGL\GLError.h>
namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace OpenGL
			{
				void GLPixelShader::Fix_Reflected_ConstantBuffer_Slot(GLPixelShader* result, BinaryShaderBlob* blob)
				{
					std::unordered_map<std::string, Reflected_Constantbuffer>::iterator it;
					for (it = blob->Reflection.ConstantBuffers.begin(); it != blob->Reflection.ConstantBuffers.end(); it++)
					{
						it->second.BindingSlot = glGetUniformBlockIndex(result->_ProgramID, it->first.c_str());
						GLCheckError();
					}
				}

				GLPixelShader::GLPixelShader()
				{
					_ProgramID = 0;
				}

				GLPixelShader::~GLPixelShader()
				{
					_ProgramID = 0;
				}
				void CompileFragmentshader(GLuint& shader, std::string source)
				{
					shader = glCreateShader(GL_FRAGMENT_SHADER);
					if (shader) {
						const GLchar* strings = source.c_str();
						GLCall(glShaderSource(shader, 1, &strings, NULL));
						GLCall(glCompileShader(shader));
						const GLuint program = glCreateProgram();
						GLCheckError();
						if (program) {
							GLint compiled = GL_FALSE;
							GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled));
							GLCall(glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE));

							//Check for error begin
							GLint success;
							char infoLog[1024];
							GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &success));
							if (!success)
							{
								GLCall(glGetShaderInfoLog(shader, 1024, NULL, infoLog));
								Log.Error("[GLPixelShader] Compiling Error -- Info: " + std::string(infoLog) + "\n");

							}
							// check for error end

							if (compiled) {
								GLCall(glAttachShader(program, shader));
								GLCall(glLinkProgram(program));
								GLCall(glDetachShader(program, shader));
							}
						}
						GLCall(glDeleteShader(shader));
						shader = program;
					}
					else {
						shader = 0;
					}
				}

				void GLPixelShader::Create(GLPixelShader* result, BinaryShaderBlob* desc)
				{
					if (desc->Finished)
					{
						if (desc->GLSL_SourceCode.size() == 0)
						{
							Log.Error("[GLPixelShader] GLSL Source Code is empty!\n");
						}
						else
						{
							CompileFragmentshader(result->_ProgramID, desc->GLSL_SourceCode);
						}
					}

				}

				void GLPixelShader::Delete(GLPixelShader * shader)
				{
					if (shader->_ProgramID != 0)
					{
						GLCall(glDeleteProgram(shader->_ProgramID));
					}
					shader->_ProgramID = 0;
				}

				void GLPixelShader::SetConstantBuffer(GLConstantBuffer* ubuffer)
				{
					GLCall(glUniformBlockBinding(_ProgramID, glGetUniformBlockIndex(_ProgramID, ubuffer->GetName()), ubuffer->GetBindingIndex()));
				}

				void GLPixelShader::Bind()
				{
					GLCall(glUseProgramStages(GLVertexShader::GetPipelineID(), GL_FRAGMENT_SHADER_BIT, _ProgramID));
				}
			}
		}
	}
}
#endif