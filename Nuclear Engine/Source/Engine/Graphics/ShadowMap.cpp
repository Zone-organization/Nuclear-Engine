#include "Engine\Graphics\ShadowMap.h"

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

			API::RenderTarget::Create(&mDepthMap);
			mDepthMap.AttachDepthStencilBuffer(Math::Vector2ui(mWidth, mHeight));
		}
		void ShadowMap::Update(std::vector<Components::DirectionalLight*> DirLights, std::vector<Components::PointLight*> PointLights, std::vector<Components::SpotLight*> SpotLights)
		{
		}
	}
}