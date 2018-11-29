#include <Engine/Graphics/API/OpenGL\GLVertexShader.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <Engine/Graphics/API/Shader_Types.h>
#include <Engine/Graphics/API/OpenGL\GLConstantBuffer.h>


namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			namespace OpenGL
			{
				static GLuint pipelineid = 0;
			
				GLVertexShader::GLVertexShader()
				{
					mVertexShaderID = 0;
				}

				GLVertexShader::~GLVertexShader()
				{
					mVertexShaderID = 0;
				}
				void CompileVertexshader(GLuint& shader, std::string source)
				{
					shader = glCreateShader(GL_VERTEX_SHADER);
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
								Log.Error("[GLVertexShader] Compiling Error -- Info: " + std::string(infoLog) + "\n");

							}
							// check for error end

							if (compiled)
							{
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

				void GLVertexShader::Create(GLVertexShader* result, const BinaryShaderBlob& desc)
				{
					if (pipelineid == 0)
					{
						glGenProgramPipelines(1, &pipelineid);
					}

					if (desc.isValid)
					{
						if (desc.GLSL_SourceCode.size() == 0)
						{
							Log.Error("[GLVertexShader] GLSL Source Code is empty!\n");
						}
						else
						{
							CompileVertexshader(result->mVertexShaderID, desc.GLSL_SourceCode);
						}
					}

				}

				void GLVertexShader::Delete(GLVertexShader * shader)
				{
					if (shader->mVertexShaderID != 0)
					{
						glDeleteProgram(shader->mVertexShaderID);
					}
					shader->mVertexShaderID = 0;
				}

				void GLVertexShader::SetConstantBuffer(GLConstantBuffer* ubuffer)
				{
					glUniformBlockBinding(mVertexShaderID, glGetUniformBlockIndex(mVertexShaderID, ubuffer->mName), ubuffer->mBindingIndex);
				}

				void GLVertexShader::Bind()
				{
					glBindProgramPipeline(pipelineid);
					glUseProgramStages(pipelineid, GL_VERTEX_SHADER_BIT, mVertexShaderID);
				}
				GLuint GLVertexShader::GetPipelineID()
				{
					return pipelineid;
				}
			}
		}
	}
}
#endif