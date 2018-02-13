#include <API\OpenGL\GLVertexShader.h>

#ifdef NE_COMPILE_CORE_OPENGL
#include <API\Shader_Types.h>
#include <API\OpenGL\GLConstantBuffer.h>
#include <API\OpenGL\GLContext.h>

#include <API\OpenGL\GLError.h>
namespace NuclearEngine
{
	namespace API
	{
		namespace OpenGL
		{
			static GLuint boundvshader = 0;
			void GLVertexShader::Fix_Reflected_ConstantBuffer_Slot(GLVertexShader* result, BinaryShaderBlob* blob)
			{
				std::map<std::string, Reflected_Constantbuffer>::iterator it;
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
			void GLVertexShader::Create(GLVertexShader* result, BinaryShaderBlob* desc)
			{
			
				if (desc->Finished)
				{
					if (desc->Language != API::ShaderLanguage::GLSL)
					{
						Log.Error("[GLVertexShader] OpenGL Renderer Backend expects all -Vertex- shaders in GLSL language!\n");
					}
					else
					{
						const char* code = desc->GLSL_SourceCode.c_str();

						result->_ProgramID = glCreateShaderProgramv(GL_VERTEX_SHADER, 1, &code);
						GLCheckError();
						//CheckShaderErrors(result->_ProgramID);
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

			void GLVertexShader::SetConstantBuffer(GLConstantBuffer* ubuffer, API::ShaderType type)
			{
				GLCall(glUniformBlockBinding(_ProgramID, glGetUniformBlockIndex(_ProgramID, ubuffer->GetName()), ubuffer->GetBindingIndex()));
			}

			void GLVertexShader::Bind()
			{
				if (boundvshader != _ProgramID)
				{
					//GLCall(glUseProgramStages(GLContext::pipelineid, GL_VERTEX_SHADER_BIT, _ProgramID));
					boundvshader = _ProgramID;
				}
			}
		}
	}
}
#endif