#pragma once
#include <vector>
#include <Base/Math/Math.h>
#include <Base\NE_Common.h>
#include <Engine/Graphics/API\RenderAPI.h>
#include <Engine\Components\Sprite.h>
#include <Engine\Graphics\API\Texture.h>

namespace NuclearEngine {
	namespace Graphics {

		struct SpriteVertexData {
			Math::Vector3 vertex;
			Math::Vector2 uv;
			float tid;
			unsigned int color;
		};

#define MAX_SPRITES 10000

		class NEAPI SpriteBatch {
		public:
			SpriteBatch();
			~SpriteBatch();
			
			void Initialize();
			void Begin();
			void Submit(const Components::Sprite &sprite);
			void End();
			void Draw();

		private:
			SpriteVertexData * mVertexBufferData;
			API::VertexBuffer m_VB;
			API::IndexBuffer m_IB;
			std::vector<Math::Matrix4> TransformationStack;
			Math::Matrix4* TransformationBack;
			Uint32 IndexCount;
			std::vector<Graphics::API::Texture*> Textures;
			bool TexturesBinded;
		};
	}
} 
