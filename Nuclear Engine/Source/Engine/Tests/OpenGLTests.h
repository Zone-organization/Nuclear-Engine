#pragma once
#include "TestCommon.h"

#ifndef _DEBUG
class OpenGLTests : public Core::Game
{
};
#else
#include <Engine/Graphics/API/OpenGL\GLError.h>
#include <iostream>
class OpenGLTests : public Core::Game
{
protected:
	Graphics::API::VertexBuffer TriangleVB;
	GLuint vertProg, fragProg, pipeline;
	Graphics::API::Sampler LinearSampler;
	Assets::Texture texture2;
	Assets::Texture texture1;

	float vertices[18] =
	{
		// positions         // colors
		0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f,  // bottom left
		0.0f,  0.5f, 0.0f,   0.0f, 1.0f    // top 

	};

	std::string VertexShader = R"(#version 330

out gl_PerVertex
{
    vec4 gl_Position;
};

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 TexCoord;

out vec2 oTexCoord;

void main()
{
    gl_Position = vec4(position, 1.0f);
    oTexCoord = TexCoord;
}
)";

	std::string PixelShader = R"(
#version 330
#extension GL_ARB_shading_language_420pack : enable

in vec2 TexCoord;

out vec4 SV_Target0;
layout(binding = 1) uniform sampler2D texture1;
layout(binding = 0) uniform sampler2D texture2;

void main()
{	
	SV_Target0 = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);
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
		Graphics::API::OpenGL::GLCheckError();
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			Graphics::API::OpenGL::GLCheckError();
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
			Graphics::API::OpenGL::GLCheckError();
			glCompileShader(shader);
			Graphics::API::OpenGL::GLCheckError();
			const GLuint program = glCreateProgram();
			Graphics::API::OpenGL::GLCheckError();
			if (program) {
				GLint compiled = GL_FALSE;
				glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
				Graphics::API::OpenGL::GLCheckError();
				glProgramParameteri(program, GL_PROGRAM_SEPARABLE, GL_TRUE);
				Graphics::API::OpenGL::GLCheckError();
				checkshadererror(shader);
				if (compiled) {
					glAttachShader(program, shader);
					Graphics::API::OpenGL::GLCheckError();
					glLinkProgram(program);
					Graphics::API::OpenGL::GLCheckError();
					glDetachShader(program, shader);
					Graphics::API::OpenGL::GLCheckError();
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

		glGenProgramPipelines(1, &pipeline);
		glUseProgramStages(pipeline, GL_VERTEX_SHADER_BIT, vertProg);
		glUseProgramStages(pipeline, GL_FRAGMENT_SHADER_BIT, fragProg);

		Graphics::API::VertexBufferDesc Desc;
		Desc.data = vertices;
		Desc.size = sizeof(vertices);
		Desc.usage = Graphics::API::BufferUsage::Dynamic;

		//Create the vertex Buffer
		Graphics::API::VertexBuffer::Create(&TriangleVB, Desc);

		Graphics::API::InputLayout TriangleIL;
		TriangleIL.AppendAttribute("POSITION", 0, Graphics::API::DataType::Float3);
		TriangleIL.AppendAttribute("TEXCOORD", 0, Graphics::API::DataType::Float2);

		TriangleVB.SetInputLayout(&TriangleIL, &Graphics::API::VertexShader());


	    texture1 = Managers::AssetManager::Import("Assets/Common/Textures/woodenbox.jpg");
		texture2 = Managers::AssetManager::Import("Assets/Common/Textures/crate_diffuse.png");

		//Create sampler
		Graphics::API::SamplerDesc Samplerdesc;
		Samplerdesc.Filter = Graphics::API::TextureFilter::Trilinear;
		Graphics::API::Sampler::Create(&LinearSampler, Samplerdesc);

		Core::Application::Display();
		Graphics::API::Context::SetPrimitiveType(Graphics::PrimitiveType::TriangleList);
	}

	void Render(float) override		// Render The Game
	{


		//Change Background Color to Blue in RGBA format
		Graphics::API::Context::Clear(Graphics::Color(0.2f, 0.3f, 0.3f, 1.0f), ClearColorBuffer | ClearDepthBuffer);

		glBindProgramPipeline(pipeline);
		Graphics::API::OpenGL::GLCheckError();

		LinearSampler.PSBind(0);
		LinearSampler.PSBind(1);
		if (Core::Input::Keyboard::IsKeyPressed(Core::Input::Keyboard::Key::Num1))
		{
			texture1.PSBind(0);
			texture2.PSBind(1);
		}
		else {
			texture1.PSBind(1);
			texture2.PSBind(0);
		}

		TriangleVB.Bind();
		Graphics::API::Context::Draw(3);

		Graphics::API::Context::PresentFrame();
	}

	void Shutdown() override
	{
		Graphics::API::VertexBuffer::Delete(&TriangleVB);
	}
};

#endif