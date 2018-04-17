#pragma once
#include <NE_Common.h>
#include <Graphics/API/VertexShader.h>
#include <Graphics/API/PixelShader.h>

namespace NuclearEngine
{
	namespace Assets
	{
		class NEAPI Material
		{
		public:

			//If the shader is a nullptr the default shader is bound
			Graphics::API::VertexShader* mVShader;
			Graphics::API::PixelShader* mPShader;
		};
	}
}