#pragma once
#include <vector>
#include <Graphics/API/RenderAPI.h>
namespace NuclearEngine
{
	namespace Graphics
	{
		class SpriteBatch
		{
		public:
			SpriteBatch(unsigned int MaxVertices = 10000);
			~SpriteBatch();

			void Begin();
			void End();

			void Render();
		private:
			
			Graphics::API::VertexBuffer VB;
			unsigned int MaxVertices;
		};
	}
}