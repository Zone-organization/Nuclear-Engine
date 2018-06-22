#pragma once
#include <Base\NE_Common.h>
#include <Engine\Graphics\API\RenderAPI.h>
#include <Base\Utilities\Delegate.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		typedef Utilities::Delegate<void(Graphics::API::VertexShader* shader, const Graphics::API::BinaryShaderBlob& Desc)> OnVertexShaderCreation;
		typedef Utilities::Delegate<void(Graphics::API::PixelShader* shader, const Graphics::API::BinaryShaderBlob& Desc)> OnPixelShaderCreation;
		typedef Utilities::Delegate<void(Graphics::API::Texture* texture, const Graphics::API::Texture_Data& TexData, const Graphics::API::Texture_Desc& Desc)> OnTextureCreation;

		class NEAPI GraphicsEngine
		{
		public:
			static bool Initialize();
			static std::vector<OnVertexShaderCreation> VertexShaderCreationCallbacks;
			static std::vector<OnPixelShaderCreation> PixelShaderCreationCallbacks;
			static std::vector<OnTextureCreation> TextureCreationCallbacks;

		};
	}
}