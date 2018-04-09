#pragma once
#include <vector>
#include <Math/Math.h>
#include <NE_Common.h>
#include <Graphics/API\RenderAPI.h>
#include <Components\Sprite.h>
namespace NuclearEngine {
	namespace Graphics {
		class Sprite;
		class Texture;
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
			void Submit(const Sprite &sprite);
			void End();
			void Draw();

		private:
			SpriteVertexData * mVertexBufferData;
			API::VertexBuffer m_VB;
			API::IndexBuffer m_IB;
			std::vector<Math::Matrix4> TransformationStack;
			Math::Matrix4* TransformationBack;
			uint IndexCount;
			std::vector<Texture *> Textures;
			bool TexturesBinded;
		};
	}
} 
