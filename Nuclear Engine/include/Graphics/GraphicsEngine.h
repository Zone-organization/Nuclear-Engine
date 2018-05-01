#pragma once
#include <NE_Common.h>
#include <Graphics\API\RenderAPI.h>
#include <Utilities\Delegate.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		typedef Utilities::Delegate<void(Graphics::API::VertexShader* shader, Graphics::API::BinaryShaderBlob* Desc)> OnVertexShaderCreation;
		typedef Utilities::Delegate<void(Graphics::API::PixelShader* shader, Graphics::API::BinaryShaderBlob* Desc)> OnPixelShaderCreation;
		typedef Utilities::Delegate<void(Graphics::API::Texture* texture, const Graphics::API::Texture_Data& TexData, const Graphics::API::Texture_Desc& Desc)> OnTextureCreation;

		class NEAPI GraphicsEngine
		{
		public:
			static bool Initialize();
			static std::vector<OnVertexShaderCreation>* GetVertexShaderCreationCallbacks();
			static std::vector<OnPixelShaderCreation>* GetPixelShaderCreationCallbacks();
			static std::vector<OnTextureCreation>* GetTextureCreationCallbacks();
		};
	}
}