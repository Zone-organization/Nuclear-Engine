#pragma once

#include <NE_Common.h>
#include <Math\Math.h>
#include <Graphics\API\Texture.h>
namespace NuclearEngine {
	namespace Components {

		class NEAPI Renderable2D {
		public:
			Math::Vector2 Pos, Dimensions, RealDimensions;

			Renderable2D(int x, int y, int w, int h, Graphics::API::Texture *texture, bool visible = true);
			Renderable2D(int x, int y, Graphics::API::Texture *texture);
			Renderable2D(Graphics::API::Texture *texture);
			virtual ~Renderable2D();

			// Getters
			inline Math::Vector2 GetPos() const { return Pos; }
			inline Math::Vector2 GetDimensions() const { return Dimensions; }
			inline float GetWidth() const { return Dimensions.x; }
			inline float GetHeight() const { return Dimensions.y; }
			inline float GetRotation() const { return Rotation; }
			inline Math::Vector2 GetScale() const { return Scale; }
			Graphics::API::Texture *GetTexture() const;
			inline const bool& IsVisible() const { return Visible; }

			// Setters
			void SetVisible(bool vis) { Visible = vis; }
			void ToggleVisible() { Visible = !Visible; }
			void SetDimensions(Math::Vector2 dimensions) { Dimensions = dimensions; }
			void SetRotation(float rotation) { Rotation = rotation; }
			void SetRelativeScale(Math::Vector2 scale) { Scale = scale; }
			void SetScale(Math::Vector2 scale) { Scale = scale; }

			void SetDirectScale(Math::Vector2 scale) { Scale = scale / (this->Dimensions); }
			//void SetDirectScale(Math::Vector2 scale) { Scale = scale/(this->Dimensions); this->Dimensions *= this->Scale; }

		protected:
			Graphics::API::Texture *Tex;
			bool Visible;
			float Rotation = 0.f;
			Math::Vector2 Scale = Math::Vector2(1.f, 1.f);
		};
	}
}
