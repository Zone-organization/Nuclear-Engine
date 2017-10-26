//#include "Components\Skybox.h"
//#include "Core\Renderer\Renderer.h"
//
//namespace NuclearEngine
//{
//	namespace Components
//	{
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
//
//
//
//		Skybox::Skybox(API::UniformBuffer* CameraCbuffer)
//		{
//			vertexBuffer = new API::VertexBuffer();
//			vertexBufferLayout = new API::InputLayout();
//			shader = new API::Shader(GLVS, GLFS,nullptr, DXVS, DXPS, nullptr);
//			shader->SetUniformBuffer(CameraCbuffer);
//
//			//_CameraCbuffer = CameraCbuffer;
//		}
//		Skybox::~Skybox()
//		{
//		}
//
//		void Skybox::Load(const std::vector<std::string>& filenames)
//		{
//			float _vertices[108] = {
//				// positions          
//				-1.0f,  1.0f, -1.0f,
//				-1.0f, -1.0f, -1.0f,
//				1.0f, -1.0f, -1.0f,
//				1.0f, -1.0f, -1.0f,
//				1.0f,  1.0f, -1.0f,
//				-1.0f,  1.0f, -1.0f,
//
//				-1.0f, -1.0f,  1.0f,
//				-1.0f, -1.0f, -1.0f,
//				-1.0f,  1.0f, -1.0f,
//				-1.0f,  1.0f, -1.0f,
//				-1.0f,  1.0f,  1.0f,
//				-1.0f, -1.0f,  1.0f,
//
//				1.0f, -1.0f, -1.0f,
//				1.0f, -1.0f,  1.0f,
//				1.0f,  1.0f,  1.0f,
//				1.0f,  1.0f,  1.0f,
//				1.0f,  1.0f, -1.0f,
//				1.0f, -1.0f, -1.0f,
//
//				-1.0f, -1.0f,  1.0f,
//				-1.0f,  1.0f,  1.0f,
//				1.0f,  1.0f,  1.0f,
//				1.0f,  1.0f,  1.0f,
//				1.0f, -1.0f,  1.0f,
//				-1.0f, -1.0f,  1.0f,
//
//				-1.0f,  1.0f, -1.0f,
//				1.0f,  1.0f, -1.0f,
//				1.0f,  1.0f,  1.0f,
//				1.0f,  1.0f,  1.0f,
//				-1.0f,  1.0f,  1.0f,
//				-1.0f,  1.0f, -1.0f,
//
//				-1.0f, -1.0f, -1.0f,
//				-1.0f, -1.0f,  1.0f,
//				1.0f, -1.0f, -1.0f,
//				1.0f, -1.0f, -1.0f,
//				-1.0f, -1.0f,  1.0f,
//				1.0f, -1.0f,  1.0f
//			};
//
//
//			vertexBuffer->Create(_vertices, sizeof(_vertices),API::Usage::Static);
//
//			vertexBufferLayout->Push(0, "POSITION", API::DataType::Float3, sizeof(float) * 3, 0);
//			vertexBuffer->SetInputLayout(vertexBufferLayout,shader);
//
//			API::Texture_Desc Desc;
//			Desc.Format = API::TextureFormat::R8G8B8A8;
//			Desc.Wrap = API::TextureWrap::ClampToEdge;
//			Desc.Filter = API::TextureFilter::Bilinear;
//
//			textureCube.Load(filenames, Desc);
//		}
//
//		void Skybox::Render()
//		{
//			shader->Bind();
//			// TODO: Remove The depth function to OneAPI
//			if (GetRenderer() == Renderers::OpenGL3)
//			{
//				glDepthFunc(GL_LEQUAL);
//			}
//			vertexBuffer->Bind();
//			textureCube.Bind();
//			Core::Context::Draw(36);
//			textureCube.Unbind();
//			vertexBuffer->Unbind();
//			if (GetRenderer() == Renderers::OpenGL3)
//			{
//				glDepthFunc(GL_LESS); // set depth function back to default
//			}
//			shader->Unbind();
//
//		}
//	}
//}