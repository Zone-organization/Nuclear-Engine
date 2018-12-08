#include "Engine\Graphics\ShadowMap.h"
#include <Engine\Graphics\Context.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		ShadowMap::ShadowMap()
		{
		}
		ShadowMap::~ShadowMap()
		{
		}
		void ShadowMap::Initialize(Uint8 Width, Uint8 Height)
		{
			this->mWidth = Width;
			this->mHeight = Height;


			LLGL::RenderTargetDescriptor RTDesc;
			RTDesc.resolution.width = mWidth;
			RTDesc.resolution.height = mHeight;
			RTDesc.attachments.push_back({ LLGL::AttachmentType::DepthStencil });

			mDepthMap = Graphics::Context::GetRenderer()->CreateRenderTarget(RTDesc);
		}
		void ShadowMap::Update(std::vector<Components::DirectionalLight*> DirLights, std::vector<Components::PointLight*> PointLights, std::vector<Components::SpotLight*> SpotLights)
		{
		}
	}
}