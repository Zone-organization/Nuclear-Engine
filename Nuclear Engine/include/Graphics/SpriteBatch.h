#pragma once
#include <vector>
#include <Math/Math.h>
#include <NE_Common.h>
#define MAX_SPRITES 10000
#define VERTEX_SIZE sizeof(VertexData)
#define SPRITE_SIZE 4 * VERTEX_SIZE
#define BUFFER_SIZE MAX_SPRITES * SPRITE_SIZE
#define INDICES_SIZE 6 * MAX_SPRITES

namespace NuclearEngine {
	namespace Graphics {
		class Sprite;
		class Texture;
		struct VertexData;

		class NEAPI SpriteBatch {
		public:
			SpriteBatch(unsigned int MaxSprites = MAX_SPRITES);
			~SpriteBatch();
			void Begin();
			void Submit(const Sprite &sprite);
			void End();
			void Draw();

		private:
			VertexData * VertexBuffer;
			uint VAO, VBO, EBO;
			//std::vector<glm::mat4> TransformationStack;
			//glm::mat4* TransformationBack;
			uint IndexCount;
			std::vector<Texture *> Textures;
			bool TexturesBinded;

			void init();
		};
	} // namespace graphics
} // namespace stella
