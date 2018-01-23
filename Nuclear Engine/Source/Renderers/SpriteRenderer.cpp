#include <Renderers\SpriteRenderer.h>
#include <API\InputLayout.h>
#include <Core\Context.h>
#include <API\Shader_Types.h>

namespace NuclearEngine {
	namespace Renderers {
		SpriteRenderer::SpriteRenderer()
		{
		}
		SpriteRenderer::~SpriteRenderer()
		{
		}
		void SpriteRenderer::Initialize(API::Shader * _shader, Components::GenericCamera * _camera)
		{
			this->shader = _shader;
			this->camera = _camera;
			Init();
		}
		void SpriteRenderer::DrawSprite(API::Texture * texture, Math::Vector2 position, Math::Vector2 size, float rotate, API::Color color)
		{    
			// Prepare transformations
			shader->Bind();
			Math::Matrix4 model;
			model = Math::Translate(model, Math::Vector3(position, 0.0f));

			model = Math::Translate(model, Math::Vector3(0.5f * size.x, 0.5f * size.y, 0.0f));
			model = Math::Rotate(model, Math::Vector3(0.0f, 0.0f, 1.0f), rotate);
			model = Math::Translate(model, Math::Vector3(-0.5f * size.x, -0.5f * size.y, 0.0f));

			model = Math::Scale(model, Math::Vector3(size, 1.0f));

			SpriteColorBuffer.Update(&color, sizeof(color));

			camera->SetModelMatrix(model);

			texture->PSBind("SpriteTexture",shader,0);
			vbo.Bind();
			Core::Context::Draw(6);
		}
		void SpriteRenderer::DrawSprite(Components::Sprite * Sprite)
		{
			this->DrawSprite(Sprite->Texture, Sprite->Position, Sprite->Size, Sprite->Rotation, Sprite->Color);
		}
		void SpriteRenderer::Init()
		{ 
			// Configure VAO/VBO
			float vertices[] = {
				// Pos      // Tex
				0.0f, 1.0f, 0.0f, 1.0f,
				1.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,

				0.0f, 1.0f, 0.0f, 1.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 0.0f, 1.0f, 0.0f
			};

			VertexBufferDesc Desc;
			Desc.data = vertices;
			Desc.size = sizeof(vertices);
			Desc.usage = BufferUsage::Dynamic;
			API::InputLayout layout;
			layout.Push("POSITION", 0, DataType::Float4);

			API::VertexBuffer::Create(&vbo,&Desc);
			vbo.SetInputLayout(&layout, shader);

			API::ConstantBuffer::Create(&SpriteColorBuffer, "SpriteColor", sizeof(API::Color));
			shader->SetConstantBuffer(camera->GetCBuffer(), API::ShaderType::Vertex);
			shader->SetConstantBuffer(&SpriteColorBuffer,API::ShaderType::Pixel);
		}
	}
}