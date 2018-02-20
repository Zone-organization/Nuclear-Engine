#include <API\OpenGL\GLVertexShader.h>

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
			static GLuint pipelineid = 0;
			void GLVertexShader::Fix_Reflected_ConstantBuffer_Slot(GLVertexShader* result, BinaryShaderBlob* blob)
			{
				std::unordered_map<std::string, Reflected_Constantbuffer>::iterator it;
				for (it = blob->Reflection.ConstantBuffers.begin(); it != blob->Reflection.ConstantBuffers.end(); it++)
				{
					it->second.BindingSlot = glGetUniformBlockIndex(result->_ProgramID, it->first.c_str());
					GLCheckError();
				}
			}

			GLVertexShader::GLVertexShader()
			{
				_ProgramID = 0;
			}

			GLVertexShader::~GLVertexShader()
			{
				_ProgramID = 0;
			}
			void CompileVertexshader(GLuint& shader, std::string source)
			{
				shader = glCreateShader(GL_VERTEX_SHADER);
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
							Log.Error("[GLVertexShader] Compiling Error -- Info: " + std::string(infoLog) + "\n");
					
						}
						// check for error end

						if (compiled) 
						{
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

			void GLVertexShader::Create(GLVertexShader* result, BinaryShaderBlob* desc)
			{			
				if (pipelineid == 0)
				{
					glGenProgramPipelines(1, &pipelineid);
				}

				if (desc->Finished)
				{
					if (desc->Language != API::ShaderLanguage::GLSL)
					{
						Log.Error("[GLVertexShader] OpenGL Renderer Backend expects all -Vertex- shaders in GLSL language!\n");
					}
					else
					{
						CompileVertexshader(result->_ProgramID, desc->GLSL_SourceCode);
					}
				}

			}

			void GLVertexShader::Delete(GLVertexShader * shader)
			{
				if (shader->_ProgramID != 0)
				{
					GLCall(glDeleteProgram(shader->_ProgramID));
				}
				shader->_ProgramID = 0;
			}

			void GLVertexShader::SetConstantBuffer(GLConstantBuffer* ubuffer)
			{
				GLCall(glUniformBlockBinding(_ProgramID, glGetUniformBlockIndex(_ProgramID, ubuffer->GetName()), ubuffer->GetBindingIndex()));
			}

			void GLVertexShader::Bind()
			{
				GLCall(glBindProgramPipeline(pipelineid));
				GLCall(glUseProgramStages(pipelineid, GL_VERTEX_SHADER_BIT, _ProgramID));			
			}
			GLuint GLVertexShader::GetPipelineID()
			{
				return pipelineid;
			}
		}
	}
}
#endif