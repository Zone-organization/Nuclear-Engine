#include <Engine/Graphics/API/OpenGL\GLPixelShader.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <Engine/Graphics/API/Shader_Types.h>
#include <Engine/Graphics/API/OpenGL\GLConstantBuffer.h>
#include <Engine/Graphics/API/OpenGL\GLVertexShader.h>


namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace OpenGL
			{
				GLPixelShader::GLPixelShader()
				{
					mFragmentShaderID = 0;
				}

				GLPixelShader::~GLPixelShader()
				{
					mFragmentShaderID = 0;
				}
				void CompileFragmentshader(GLuint& shader, std::string source)
				{
					shader = glCreateShader(GL_FRAGMENT_SHADER);
					if (shader) {
						const GLchar* strings = source.c_str();
						glShaderSource(shader, 1, &strings, NULL);
						glCompileShader(shader);
						const GLuint program = glCreateProgram();
						if (program) {
							GLint compiled = GL_FALSE;
							glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
							glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);

							//Check for error begin
							GLint success;
							char infoLog[1024];
							glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
							if (!success)
							{
								glGetShaderInfoLog(shader, 1024, NULL, infoLog);
								Log.Error("[GLPixelShader] Compiling Error -- Info: " + std::string(infoLog) + "\n");

							}
							// check for error end

							if (compiled) {
								glAttachShader(program, shader);
								glLinkProgram(program);
								glDetachShader(program, shader);
							}
						}
						glDeleteShader(shader);
						shader = program;
					}
					else {
						shader = 0;
					}
				}

				void GLPixelShader::Create(GLPixelShader* result, const BinaryShaderBlob& desc)
				{
					if (desc.isValid)
					{
						if (desc.GLSL_SourceCode.size() == 0)
						{
							Log.Error("[GLPixelShader] GLSL Source Code is empty!\n");
						}
						else
						{
							CompileFragmentshader(result->mFragmentShaderID, desc.GLSL_SourceCode);
						}
					}

				}

				void GLPixelShader::Delete(GLPixelShader * shader)
				{
					if (shader->mFragmentShaderID != 0)
					{
						glDeleteProgram(shader->mFragmentShaderID);
					}
					shader->mFragmentShaderID = 0;
				}

				void GLPixelShader::SetConstantBuffer(GLConstantBuffer* ubuffer)
				{
					glUniformBlockBinding(mFragmentShaderID, glGetUniformBlockIndex(mFragmentShaderID, ubuffer->mName), ubuffer->mBindingIndex);
				}

				void GLPixelShader::Bind()
				{
					glUseProgramStages(GLVertexShader::GetPipelineID(), GL_FRAGMENT_SHADER_BIT, mFragmentShaderID);
				}
			}
		}
	}
}
#endif