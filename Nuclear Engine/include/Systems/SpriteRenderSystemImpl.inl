#include <Core\Context.h>
#include <Math\Math.h>
namespace NuclearEngine
{
	namespace Systems
	{
		SpriteRenderSystem::SpriteRenderSystem()
		{
		}

		bool SpriteRenderSystem::Initialize(const SpriteRenderSystemDesc & mDesc)
		{
			float VTexCoord = -1.0f;

			if (Core::Context::GetRenderAPI() == Core::RenderAPI::OpenGL3)
			{
				VTexCoord = +1.0f;
			}
			// Configure NDC ScreenQuad
			float vertices[] = {
				// Pos            // Tex
				0.0f, 1.0f, 0.0f, 0.0f, VTexCoord,
				1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f, 0.0f,

				0.0f, 1.0f, 0.0f, 0.0f, VTexCoord,
				1.0f, 1.0f, 0.0f, 1.0f, VTexCoord,
				1.0f, 0.0f, 0.0f, 1.0f, 0.0f
			};
			
			API::VertexShader::Create( &mVShader, &API::CompileShader(mDesc.VertexShaderPath, API::ShaderType::Vertex));
			API::PixelShader::Create( &mPShader, &API::CompileShader(mDesc.PixelShaderPath, API::ShaderType::Pixel));

			API::VertexBufferDesc VBDesc;
			VBDesc.data = vertices;
			VBDesc.size = sizeof(vertices);
			VBDesc.usage = API::BufferUsage::Dynamic;
			API::InputLayout layout;
			layout.AppendAttribute("POSITION", 0, API::DataType::Float3);
			layout.AppendAttribute("TEXCOORD", 0, API::DataType::Float2);

			API::VertexBuffer::Create(&mVertexBuffer, VBDesc);
			mVertexBuffer.SetInputLayout(&layout, &mVShader);

			API::ConstantBuffer::Create(&mSpriteColorBuffer, "SpriteColor", sizeof(API::Color));

			mPShader.SetConstantBuffer(&mSpriteColorBuffer);

			if (mDesc.InitialCamera == nullptr)
			{
				Log.Warning("[SpriteRenderSystem] Initializing system without an active camera, besure to set it before rendering.\n");
				return false;
			}
			else {
				SetActiveCamera(mDesc.InitialCamera);
			}
			return true;
		}

		bool SpriteRenderSystem::SetActiveCamera(Components::GenericCamera * Camera)
		{
			if (Camera == nullptr)
			{
				Log.Error("[SpriteRenderSystem] Setting Active Camera to nullptr isn't allowed.\n");
				return false;
			}
			mCamera = Camera;
			mVShader.SetConstantBuffer(&mCamera->GetCBuffer());
			return true;
		}

		void SpriteRenderSystem::BeginDirectRender()
		{
			mVShader.Bind();
			mPShader.Bind();
			mVertexBuffer.Bind();

		}

		void SpriteRenderSystem::DirectRender(Components::Sprite * sprite)
		{
			Math::Matrix4 model(1.0f);
			model = Math::Translate(model, Math::Vector3(sprite->Position, 0.0f));
			
			model = Math::Translate(model, Math::Vector3(0.5f * sprite->Size.x, 0.5f * sprite->Size.y, 0.0f));
			model = Math::Rotate(model, sprite->Rotation, Math::Vector3(0.0f, 0.0f, 1.0f));
			model = Math::Translate(model, Math::Vector3(-0.5f * sprite->Size.x, -0.5f * sprite->Size.y, 0.0f));

			model = Math::Scale(model, Math::Vector3(sprite->Size, 1.0f));
			
			mSpriteColorBuffer.Update(&sprite->Color, sizeof(sprite->Color));

			mCamera->SetModelMatrix(model);

			sprite->Texture->PSBind(0);
			Core::Context::Draw(6);
		}

		void SpriteRenderSystem::EndDirectRender()
		{
		}
		
		void SpriteRenderSystem::Update(Core::EntityManager & es, Core::EventManager & events, Core::TimeDelta dt)
		{
		}
	}
}
