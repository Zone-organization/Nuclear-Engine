#pragma once
#include <Base/Math/Math.h>
#include <Base\NE_Common.h>

namespace NuclearEngine {
	namespace Graphics {

		class Texture;

		class Renderable {
		public:
			glm::vec2 Pos, Dimensions, RealDimensions;

			Renderable(Int32 x, Int32 y, Int32 w, Int32 h, Texture &texture, bool visible = 1);
			Renderable(Int32 x, Int32 y, Texture &texture);
			Renderable(Texture &texture);
			virtual ~Renderable();

			// Getters
			inline glm::vec2 GetPos() const { return Pos; }
			inline glm::vec2 GetDimensions() const { return Dimensions; }
			inline Float32 GetWidth() const { return Dimensions.x; }
			inline Float32 GetHeight() const { return Dimensions.y; }
			inline Float32 GetRotation() const { return Rotation; }
			inline glm::vec2 GetScale() const { return Scale; }
			Uint32 GetTexID() const;
			const std::string &GetTexName() const;
			Texture *GetTexture() const;
			inline const bool& IsVisible() const { return Visible; }

			// Setters
			void SetVisible(bool vis) { Visible = vis; }
			void ToggleVisible() { Visible = !Visible; }
			void SetDimensions(glm::vec2 dimensions) { Dimensions = dimensions; }
			void SetRotation(float rotation) { Rotation = rotation; }
			void SetRelativeScale(glm::vec2 scale) { Scale = scale; }
			void SetScale(glm::vec2 scale) { Scale = scale; }

			void SetDirectScale(glm::vec2 scale) { Scale = scale / (this->Dimensions); }
			//void SetDirectScale(glm::vec2 scale) { Scale = scale/(this->Dimensions); this->Dimensions *= this->Scale; }

		protected:
			Texture & Tex;
			bool Visible;
			float Rotation = 0.f;
			glm::vec2 Scale = glm::vec2(1.f, 1.f);
		};
	} 
}