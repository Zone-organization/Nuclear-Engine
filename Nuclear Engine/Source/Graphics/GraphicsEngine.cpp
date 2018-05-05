#include "..\..\include\Graphics\GraphicsEngine.h"

namespace NuclearEngine
{
	namespace Graphics
	{
		std::vector<OnVertexShaderCreation> GraphicsEngine::VertexShaderCreationCallbacks;
		std::vector<OnPixelShaderCreation> GraphicsEngine::PixelShaderCreationCallbacks;
		std::vector<OnTextureCreation> GraphicsEngine::TextureCreationCallbacks;

		bool GraphicsEngine::Initialize()
		{
			return false;
		}
		
	}
}