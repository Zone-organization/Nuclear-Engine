#ifndef SpriteRenderSystemImpl
#include <Systems\SpriteRenderSystem.h>
#endif
#include <Core\Context.h>
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

			SetActiveCamera(mDesc.InitialCamera);
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
			mVShader->SetConstantBuffer(&mCamera->GetCBuffer());
			return true;
		}
		
		void SpriteRenderSystem::Update(Core::EntityManager & es, Core::EventManager & events, Core::TimeDelta dt)
		{
		}
	}
}
