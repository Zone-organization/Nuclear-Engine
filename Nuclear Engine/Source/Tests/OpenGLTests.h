#pragma once
#include "TestCommon.h"
#include <API\OpenGL\GLError.h>
#include <iostream>
class OpenGLTests : public Core::Game
{
protected:
	API::VertexBuffer TriangleVB;
	GLuint vertProg, fragProg, fragProg1, pipeline;

	float vertices[18] =
	{
		// positions         // colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 

	};

	std::string VertexShader = R"(#version 330

out gl_PerVertex
{
    vec4 gl_Position;
};

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 xsv_COLOR0;

void main()
{
    gl_Position = vec4(position, 1.0f);
    xsv_COLOR0 = color;
}
)";

	std::string PixelShader = R"(
#version 330

in vec3 xsv_COLOR0;

out vec4 SV_Target0;

void main()
{
    SV_Target0 = vec4(xsv_COLOR0, 1);
}
)";

	std::string PixelShader1 = R"(
#version 330

in vec3 xsv_COLOR0;

out vec4 SV_Target0;

void main()
{
    SV_Target0 = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}
)";

public:
	OpenGLTests()
	{
	}
	void checkshadererror(GLuint shader)
	{
		GLint success;
		char infoLog[1024];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		API::OpenGL::GLCheckError();
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			API::OpenGL::GLCheckError();
			Log.Info("[GLShader] Compiling Error -- In ");
			Log.Info(" VShader - Info : \n");
			Log.Info(infoLog);
			Log.Info("\n");
		}
	}
	void compile(GLuint& shader, GLenum type, std::string source)
	{
		shader = glCreateShader(type);
		if (shader) {
			const GLchar* strings = source.c_str();
			glShaderSource(shader, 1, &strings, NULL);
			API::OpenGL::GLCheckError();
			glCompileShader(shader);
			API::OpenGL::GLCheckError();
			const GLuint program = glCreateProgram();
			API::OpenGL::GLCheckError();
			if (program) {
				GLint compiled = GL_FALSE;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
				API::OpenGL::GLCheckError();
				glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);
				API::OpenGL::GLCheckError();
				checkshadererror(shader);
				if (compiled) {
					glAttachShader(program, shader);
					API::OpenGL::GLCheckError();
					glLinkProgram(program);
					API::OpenGL::GLCheckError();
					glDetachShader(program, shader);
					API::OpenGL::GLCheckError();
				}
				/* append-shader-info-log-to-program-info-log */
			}
			glDeleteShader(shader);
			shader = program;
		}
		else {
			shader = 0;
		}
	}
	void Load() override
	{
		//Load The Shader
		compile(vertProg, GL_VERTEX_SHADER, VertexShader);
		compile(fragProg, GL_FRAGMENT_SHADER, PixelShader);
		compile(fragProg1, GL_FRAGMENT_SHADER, PixelShader1);

		glGenProgramPipelines(1, &pipeline);
		glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vertProg);

		NuclearEngine::Platform::Clock clock;
		glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fragProg);

		clock.Restart();
		int active = 0;
		for (uint i = 0; i < 30; i++)
		{
			if (active != 1)
			{
				glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fragProg);
				active = 1;
			}
			if (active != 2)
			{
				glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fragProg1);
				active = 2;
			}
			if (active != 3)
			{
				glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, 0);
				active = 3;
			}
		}
		std::cout << clock.GetElapsedTime().AsMicroseconds() << std::endl;

		VertexBufferDesc Desc;
		Desc.data = vertices;
		Desc.size = sizeof(vertices);
		Desc.usage = BufferUsage::Dynamic;

		//Create the vertex Buffer
		API::VertexBuffer::Create(&TriangleVB, &Desc);

		API::InputLayout TriangleIL;
		TriangleIL.Push("POSITION", 0, DataType::Float3);
		TriangleIL.Push("COLOR", 0, DataType::Float3);

		TriangleVB.SetInputLayout(&TriangleIL, nullptr);

		Core::Application::Display();
		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);
	}

	void Render(float) override		// Render The Game
	{
		Core::Context::Begin();

		//Change Background Color to Blue in RGBA format
		Core::Context::Clear(API::Color(0.2f, 0.3f, 0.3f, 1.0f), ClearColorBuffer | ClearDepthBuffer);

		//TriangleShader.Bind();
		//glUseProgramStages(0, GL_VERTEX_SHADER_BIT, vertProg);
		//glUseProgramStages(0, GL_FRAGMENT_SHADER_BIT, fragProg);
		if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num1))
		{
			glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fragProg);
		}
		else if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num2))
		{
			glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fragProg1);
		}
		else if (Platform::Input::Keyboard::IsKeyPressed(Platform::Input::Keyboard::Key::Num3))
		{
			glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, 0);
		}
		glBindProgramPipeline(pipeline);
		API::OpenGL::GLCheckError();

		TriangleVB.Bind();
		Core::Context::Draw(3);

		Core::Context::End();
	}

	void Shutdown() override
	{
		API::VertexBuffer::Delete(&TriangleVB);
	}
};
