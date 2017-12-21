#pragma once
#include "Components\Sprite.h"
#include "API\Shader.h"
#include <API\VertexBuffer.h>
#include <API\Color.h>
#include <Components\GenericCamera.h>

namespace NuclearEngine {
	namespace Renderers {

		class NEAPI SpriteRenderer
		{
		public:
			SpriteRenderer(API::Shader *shader, Components::GenericCamera* camera);
			SpriteRenderer();
			~SpriteRenderer();

			void DrawSprite(API::Texture *texture, Math::Vector2 position,	Math::Vector2 size = Math::Vector2(10, 10), float rotate = 0.0f, API::Color color = API::Color(1.0f,1.0f,1.0f,1.0f));
		private:
			void Init();
			API::Shader *shader;
			Components::GenericCamera* camera;
			API::VertexBuffer *vbo;
			API::ConstantBuffer *SpriteColorBuffer;
		};
	}
}