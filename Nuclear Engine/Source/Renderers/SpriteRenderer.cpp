#include <Renderers\SpriteRenderer.h>
#include <API\InputLayout.h>
#include <Core\Context.h>

namespace NuclearEngine {
	namespace Renderers {
		SpriteRenderer::SpriteRenderer(API::Shader * _shader, Components::GenericCamera* _camera)
		{
			this->shader = _shader;
			this->camera = _camera;
			Init();				
		}
		SpriteRenderer::SpriteRenderer()
		{
		}
		SpriteRenderer::~SpriteRenderer()
		{
		}
		void SpriteRenderer::DrawSprite(API::Texture * texture, Math::Vector2 position, Math::Vector2 size, float rotate, API::Color color)
		{    
			std::cout << color;
			// Prepare transformations
			this->shader->Bind();
			Math::Matrix4 model;
			model = Math::Translate(model, Math::Vector3(position, 0.0f));

			model = Math::Translate(model, Math::Vector3(0.5f * size.x, 0.5f * size.y, 0.0f));
			model = Math::Rotate(model, Math::Vector3(0.0f, 0.0f, 1.0f), rotate);
			model = Math::Translate(model, Math::Vector3(-0.5f * size.x, -0.5f * size.y, 0.0f));

			model = Math::Scale(model, Math::Vector3(size, 1.0f));

			SpriteColorBuffer->Update(&color, sizeof(color));

			camera->SetModelMatrix(model);

			texture->PSBind("SpriteTexture",shader,0);
			vbo->Bind();
			Core::Context::Draw(6);
			vbo->Unbind();		
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

			API::VertexBufferDesc Desc;
			Desc.data = vertices;
			Desc.size = sizeof(vertices);

			vbo = new API::VertexBuffer(Desc);

			API::InputLayout layout;
			layout.Push("POSITION", 0, DataType::Float4);

			vbo->SetInputLayout(&layout, this->shader);

			SpriteColorBuffer = new API::ConstantBuffer("SpriteColor", sizeof(API::Color));
			this->shader->SetConstantBuffer(SpriteColorBuffer, ShaderType::Pixel);
		}
	}
}