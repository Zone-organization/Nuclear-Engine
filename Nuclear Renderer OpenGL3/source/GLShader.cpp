#include <NuclearRendererOGL3\GLShader.h>
#include <NuclearRendererBase\NRBUniformBuffer.h>
#include <NuclearCommon\Utilities\Logger.h>
#include <NuclearCommon\Common_API_Types.h>
#include <iostream>
using namespace NuclearEngine;
using namespace NuclearCommon;

namespace NuclearRenderer
{
	GLShader::GLShader()
	{
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

	bool GLShader::Create(BinaryShaderBlob* VertexShaderCode, BinaryShaderBlob* PixelShaderCode, BinaryShaderBlob* GeometryShaderCode)
	{
		bool result = true;
		if (VertexShaderCode != nullptr)
		{
			if (VertexShaderCode->Language != ShaderLanguage::GLSL)
			{
				Log->Error("[GLShader] OpenGL Renderer Backend expects all -Vertex- shaders in GLSL language!\n");

				result = false;
			}
		}
		if (PixelShaderCode != nullptr)
		{
			if (PixelShaderCode->Language != ShaderLanguage::GLSL)
			{
				Log->Error("[GLShader] OpenGL Renderer Backend expects all -Pixel- shaders in GLSL language!\n");

				result = false;
			}
		}
		if (GeometryShaderCode != nullptr)
		{
			if (GeometryShaderCode->Language != ShaderLanguage::GLSL)
			{
				Log->Error("[GLShader] OpenGL Renderer Backend expects all -Geometry- shaders in GLSL language!\n");

				result = false;
			}
		}

		if (result == false)
		{
			return false;
		}
		GLuint vertex, fragment, geometry;
		bool vsuccess, fsuccess, gsuccess, lsuccess;
		char infoLog[1024];

		// Vertex Shader
		const char* vscode= VertexShaderCode->glslsourcecode.c_str();
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vscode, NULL);
		glCompileShader(vertex);
		vsuccess = CheckShaderErrors(vertex, "Vertex");

		const char* fscode = PixelShaderCode->glslsourcecode.c_str();
		// Fragment Shader
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fscode, NULL);
		glCompileShader(fragment);
		fsuccess = CheckShaderErrors(fragment, "Fragment (Pixel)");

		if (GeometryShaderCode != nullptr)
		{
			const char* gscode = GeometryShaderCode->glslsourcecode.c_str();
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
			this->_ProgramID = glCreateProgram();
			glAttachShader(this->_ProgramID, vertex);
			glAttachShader(this->_ProgramID, fragment);
			if (GeometryShaderCode != nullptr)
			{
				glAttachShader(this->_ProgramID, geometry);
			}
			glLinkProgram(this->_ProgramID);

			lsuccess = CheckShaderErrors(_ProgramID, "Program");

			glDeleteShader(vertex);
			glDeleteShader(fragment);
			if (GeometryShaderCode != nullptr)
			{
				glDeleteShader(geometry);
			}

			if (lsuccess)
				return true;
			else
				return false;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if (GeometryShaderCode != nullptr)
		{
			glDeleteShader(geometry);
		}

		return false;

	}

	void GLShader::SetUniformBuffer(NRBUniformBuffer* ubuffer, unsigned int slot, NuclearEngine::ShaderType type)
	{
		glUniformBlockBinding(GetGLShaderID(), slot, ubuffer->GetBindingIndex());		
	}

	unsigned int GLShader::GetUniformBufferSlot(NRBUniformBuffer* ubuffer, NuclearEngine::ShaderType type)
	{
		return glGetUniformBlockIndex(GetGLShaderID(), ubuffer->GetName());;
	}

	void GLShader::Bind()
	{
		glUseProgram(this->_ProgramID);
	}

	void GLShader::Unbind()
	{
		glUseProgram(0);
	}
	void GLShader::Delete()
	{
		glDeleteProgram(_ProgramID);
	}
	unsigned int GLShader::GetGLShaderID()
	{
		return _ProgramID;
	}
	void * GLShader::GetDXBufferPointer()
	{
		return nullptr;
	}
	unsigned long GLShader::GetDXBufferSize()
	{
		return 0;
	}
}