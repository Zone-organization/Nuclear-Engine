#include "Engine\Graphics\SpriteBatch.h"

#include <Engine\Graphics\API\Context.h>

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

			int offset = 0;
			uint indices[INDICES_SIZE];
			for (int i = 0; i < INDICES_SIZE; i += 6) {
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
		void SpriteBatch::Begin()
		{
			m_VB.Bind();
			mVertexBufferData = (SpriteVertexData*)m_VB.Map();
		}
		void SpriteBatch::Submit(const Components::Sprite & sprite)
		{
			const Math::Vector2 &position = sprite.GetPos();
			const Math::Vector2 &dimensions = sprite.GetDimensions();
			const float &rotation = sprite.GetRotation();
			const Math::Vector2 &scale = sprite.GetScale();
			const unsigned int &c = sprite.GetColor();

			const Math::Vector2 &uv = sprite.GetFrameCoords();
			const SpriteSheet &spritesheet = sprite.GetSpriteSheet();
			const uint &stW = spritesheet.GetWidth();
			const uint &stH = spritesheet.GetHeight();

			Graphics::API::Texture* texture = sprite.GetTexture();

			/*if (!texture->IsCached()) {
				Textures.push_back(texture);
				texture->SetCached((float)(Textures.size() - 1));
				this->TexturesBinded = false;
			}
			*/
			float uvoffsetX = dimensions.x / (float)stW;
			float uvoffsetY = dimensions.y / (float)stH;

			auto particular_transform = *this->TransformationBack;
			//particular_transform = Math::Translate(particular_transform, Math::Vector3(position + dimensions/2.f, 0.f)); // Translating half dimension to set the point of rotation to the center of the sprite
			particular_transform = Math::Translate(particular_transform, Math::Vector3(position, 0.f)); // Translating half dimension to set the point of rotation to the center of the sprite
			particular_transform = Math::Scale(particular_transform, Math::Vector3(scale, 1.f));
			particular_transform = Math::Translate(particular_transform, Math::Vector3(dimensions / 2.f, 0.f)); // Translating half dimension to set the point of rotation to the center of the sprite
			particular_transform = Math::Rotate(particular_transform, Math::radians(rotation), Math::Vector3(0.f, 0.f, 1.f));
			particular_transform = Math::Translate(particular_transform, Math::Vector3(-dimensions / 2.f, 0.f)); // Removing the added half dimension

			auto transformation_result = particular_transform * Math::Vector4(0.f, 0.f, 1.f, 1.f);
			//auto transformation_result = particular_transform * Math::Vector4(position, 1.f, 1.f); 
			mVertexBufferData->vertex = Math::Vector3(transformation_result.x, transformation_result.y, transformation_result.z);
			mVertexBufferData->uv = Math::Vector2(uv.x, uv.y);
			//mVertexBufferData->tid = texture->GetCacheID();
			mVertexBufferData->color = c;
			mVertexBufferData++;

			transformation_result = particular_transform * Math::Vector4(dimensions.x, 0.f, 1.f, 1.f);
			//transformation_result = particular_transform * Math::Vector4(position.x + dimensions.x, position.y, 1.f, 1.f); 
			mVertexBufferData->vertex = Math::Vector3(transformation_result.x, transformation_result.y, transformation_result.z);
			mVertexBufferData->uv = Math::Vector2(uv.x + uvoffsetX, uv.y);
			//mVertexBufferData->tid = texture->GetCacheID();
			mVertexBufferData->color = c;
			mVertexBufferData++;

			transformation_result = particular_transform * Math::Vector4(dimensions.x, dimensions.y, 1.f, 1.f);
			//transformation_result = particular_transform * Math::Vector4(position.x + dimensions.x, position.y + dimensions.y, 1.f, 1.f);
			mVertexBufferData->vertex = Math::Vector3(transformation_result.x, transformation_result.y, transformation_result.z);
			mVertexBufferData->uv = Math::Vector2(uv.x + uvoffsetX, uv.y - uvoffsetY);
			//mVertexBufferData->tid = texture->GetCacheID();
			mVertexBufferData->color = c;
			mVertexBufferData++;

			transformation_result = particular_transform * Math::Vector4(0.f, dimensions.y, 1.f, 1.f);
			//transformation_result = particular_transform * Math::Vector4(position.x, position.y + dimensions.y, 1.f, 1.f);
			mVertexBufferData->vertex = Math::Vector3(transformation_result.x, transformation_result.y, transformation_result.z);
			mVertexBufferData->uv = Math::Vector2(uv.x, uv.y - uvoffsetY);
			//mVertexBufferData->tid = texture->GetCacheID();
			mVertexBufferData->color = c;
			mVertexBufferData++;

			this->IndexCount += 6;
		}
		void SpriteBatch::End()
		{
			m_VB.Unmap();
		}
		void SpriteBatch::Draw()
		{
			for (unsigned int i = 0; i < Textures.size(); ++i) {
				Textures[i]->PSBind(i);
			}
			m_VB.Bind();
			m_IB.Bind();
			Graphics::API::Context::DrawIndexed(this->IndexCount);

			this->IndexCount = 0;
		}
	}
}