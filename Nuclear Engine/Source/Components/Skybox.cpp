#include "Components\Skybox.h"
#include <Core\Context.h>

namespace NuclearEngine
{
	namespace Components
	{
//		const char *GLVS = R"(
//#version 330 core
//layout (location) in vec3 aPos;
//out vec3 TexCoords;
//layout (std140) uniform PTE_Camera
//{
//	 mat4 model;
//	 mat4 view;
//	 mat4 projection;
//};
//void main()
//{
//    TexCoords = aPos;
//    vec4 pos = projection * mat4(mat3(view)) * vec4(aPos, 1.0);
//    gl_Position = pos.xyww;
//}  )";
//
//		const char *GLFS = R"(
//#version 330 core
//out vec4 FragColor;
//in vec3 TexCoords;
//uniform samplerCube skybox;
//void main()
//{    
//    FragColor = texture(skybox, TexCoords);
//})";
//
//		const char *DXVS = R"(
//struct VertexInputType
//{
//    float4 position : POSITION;
//};
//
//struct PixelInputType
//{
//    float4 position : SV_POSITION;
//	float2 tex : TEXCOORD;
//};
//
//cbuffer PTE_Camera : register(b0)
//{
//	matrix Model;
//	matrix View;
//	matrix Projection;
//};
//
//PixelInputType main(VertexInputType input)
//{
//    PixelInputType output;
//	
//	// Calculate the position of the vertex against the world, view, and projection matrices.
//
//	matrix mvp;
//	mvp = mul(Model, input.position);
//    mvp = mul(View, output.position);
//    mvp = mul(Projection, output.position);
//
//    //Set Pos to xyww instead of xyzw, so that z will always be 1 (furthest from camera)
//    output.position = mul(float4(input.position, 1.0f), mvp).xyww;
//
//    output.tex = input.position;
//    
//    return output;
//})";
//		const char *DXPS = R"(
//struct PixelInputType
//{
//    float4 position : SV_POSITION;
//	float2 tex : TEXCOORD;
//};
//
//TextureCube shaderTexture : register(t0);;
//SamplerState _Sampler : register(s0);
//
//float4 main(PixelInputType input) : SV_TARGET
//{
//    return shaderTexture.Sample(_Sampler, input.tex);
//})";

		float _vertices[108] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			1.0f,  1.0f, -1.0f,
			1.0f,  1.0f,  1.0f,
			1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f, -1.0f,
			1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			1.0f, -1.0f,  1.0f
		};

		
		Skybox::Skybox(API::UniformBuffer* CameraCbuffer, std::array<NuclearEngine::Texture_Data, 6> data)
		{
			vertexBufferLayout = new API::InputLayout();			
			unsigned int slot = shader->GetUniformBufferSlot(CameraCbuffer, ShaderType::Vertex);
			//shader = new API::Shader(GLVS, GLFS,nullptr, DXVS, DXPS, nullptr);
			shader->SetUniformBuffer(CameraCbuffer, slot, ShaderType::Vertex);

			API::VertexBufferDesc VDesc;
			VDesc.data = _vertices;
			VDesc.size = sizeof(_vertices);
			VDesc.usage = BufferGPUUsage::Static;
			VDesc.accessflag = BufferCPUAccess::Default;

			vertexBuffer = new API::VertexBuffer(VDesc);

			vertexBufferLayout->Push("POSITION",0 , DataType::Float3, sizeof(float) * 3, 0);
			vertexBuffer->SetInputLayout(vertexBufferLayout, shader);

			Texture_Desc Desc;
			Desc.Format = TextureFormat::R8G8B8A8;
			Desc.Wrap = TextureWrap::ClampToEdge;
			Desc.Filter = TextureFilter::Bilinear;
			texcube = new API::TextureCube(data, Desc);
		}
		Skybox::~Skybox()
		{
		}

		void Skybox::Render()
		{
			shader->Bind();
			//// TODO: Remove The depth function to OneAPI
			//if (GetRenderer() == Renderers::OpenGL3)
			//{
			//	glDepthFunc(GL_LEQUAL);
			//}
			vertexBuffer->Bind();
			texcube->Bind("skybox", shader,0);
			Core::Context::Draw(36);
			texcube->Unbind();
			vertexBuffer->Unbind();
			//if (GetRenderer() == Renderers::OpenGL3)
			//{
			//	glDepthFunc(GL_LESS); // set depth function back to default
			//}
			shader->Unbind();

		}
	}
}