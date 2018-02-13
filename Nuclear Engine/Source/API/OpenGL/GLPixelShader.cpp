#include <API\OpenGL\GLPixelShader.h>

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
			static GLuint boundpshader = 0;
			void GLPixelShader::Fix_Reflected_ConstantBuffer_Slot(GLPixelShader* result, BinaryShaderBlob* blob)
			{
				std::map<std::string, Reflected_Constantbuffer>::iterator it;
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

		
			void GLPixelShader::Create(GLPixelShader* result, BinaryShaderBlob* desc)
			{
				if (desc->Finished)
				{
					if (desc->Language != API::ShaderLanguage::GLSL)
					{
						Log.Error("[GLPixelShader] OpenGL Renderer Backend expects all -Pixel- shaders in GLSL language!\n");
					}
					else
					{
						const char* code = desc->GLSL_SourceCode.c_str();

						result->_ProgramID = glCreateShaderProgramv(GL_FRAGMENT_SHADER, 1, &code);
						GLCheckError();
						//CheckShaderErrors(result->_ProgramID);
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

			void GLPixelShader::SetConstantBuffer(GLConstantBuffer* ubuffer, API::ShaderType type)
			{
				GLCall(glUniformBlockBinding(_ProgramID, glGetUniformBlockIndex(_ProgramID, ubuffer->GetName()), ubuffer->GetBindingIndex()));
			}

			void GLPixelShader::Bind()
			{
				if (boundpshader != _ProgramID)
				{
					//GLCall(glUseProgramStages(GLContext::pipelineid, GL_FRAGMENT_SHADER_BIT, _ProgramID));
					boundpshader = _ProgramID;
				}
			}
		}
	}
}
#endif