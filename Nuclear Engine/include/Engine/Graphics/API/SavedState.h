#pragma once
#include <Engine/Graphics/API/RenderAPI.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace API
		{
			struct SavedState
			{
				RasterizerState mRasterizerState;
				BlendState mBlendState;
				DepthStencilState mDepthStencilState;
				Texture mTexture;
				Sampler mSampler;
				PixelShader mPixelShader;
				VertexShader mVertexShader;
				IndexBuffer mIndexBuffer;
				VertexBuffer mVertexBuffer;
				ConstantBuffer mConstantBuffer;
			};
		}
	}
}