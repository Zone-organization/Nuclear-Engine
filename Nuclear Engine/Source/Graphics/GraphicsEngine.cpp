#include "..\..\include\Graphics\GraphicsEngine.h"

namespace NuclearEngine
{
	namespace Graphics
	{
		static std::vector<OnVertexShaderCreation> VertexShaderCreationCallbacks;
		static std::vector<OnPixelShaderCreation> PixelShaderCreationCallbacks;
		static std::vector<OnTextureCreation> TextureCreationCallbacks;

		bool GraphicsEngine::Initialize()
		{
			return false;
		}
		std::vector<OnVertexShaderCreation>* GraphicsEngine::GetVertexShaderCreationCallbacks()
		{
			return &VertexShaderCreationCallbacks;
		}
		std::vector<OnPixelShaderCreation>* GraphicsEngine::GetPixelShaderCreationCallbacks()
		{
			return &PixelShaderCreationCallbacks;
		}
		std::vector<OnTextureCreation>* GraphicsEngine::GetTextureCreationCallbacks()
		{
			return &TextureCreationCallbacks;
		}
	}
}