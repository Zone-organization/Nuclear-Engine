#include "Graphics\SpriteBatch.h"

namespace NuclearEngine
{
	namespace Graphics
	{
#define VERTEX_SIZE sizeof(SpriteVertexData)
#define SPRITE_SIZE 4 * VERTEX_SIZE
#define BUFFER_SIZE MAX_SPRITES * SPRITE_SIZE
#define INDICES_SIZE 6 * MAX_SPRITES

		void SpriteBatch::Initialize()
		{
			Graphics::API::VertexBufferDesc Desc;
			Desc.data = NULL;
			Desc.size = BUFFER_SIZE;
			Desc.usage = Graphics::API::BufferUsage::Dynamic;

			Graphics::API::VertexBuffer::Create(&m_VB, Desc);

			Graphics::API::InputLayout RectangleIL;
			RectangleIL.AppendAttribute("POSITION", 0, Graphics::API::DataType::Float3); //Pos
			RectangleIL.AppendAttribute("TEXCOORD", 0, Graphics::API::DataType::Float2); //UV
			RectangleIL.AppendAttribute("TEXCOORD", 1, Graphics::API::DataType::Float); //Texture ID
			RectangleIL.AppendAttribute("TEXCOORD", 2, Graphics::API::DataType::Float4); //Color
			//RectangleVB.SetInputLayout(&RectangleIL, &VShader);

			GLint offset = 0;
			GLuint indices[INDICES_SIZE];
			for (GLint i = 0; i < INDICES_SIZE; i += 6) {
				indices[i] = offset;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;

				indices[i + 3] = offset;
				indices[i + 4] = offset + 2;
				indices[i + 5] = offset + 3;

				offset += 4;
			}
			Graphics::API::IndexBuffer::Create(&m_IB, indices, sizeof(indices));
		}
	}
}