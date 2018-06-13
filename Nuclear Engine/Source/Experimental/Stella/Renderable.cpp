#include "Experimental/Stella/Renderable.h"
#include <Engine\Graphics\API\Texture.h>
#include <vector>

namespace NuclearEngine {
	namespace Graphics {
		Renderable::Renderable(Int32 x, Int32 y, Int32 w, Int32 h, API::Texture &texture,
			bool visible)
			: Tex(texture) {
			this->Pos = glm::vec2((Float32)x, (Float32)y);
			this->Dimensions = glm::vec2((Float32)w, (Float32)h);
			this->RealDimensions = Dimensions;
			this->Visible = visible;
		}

		Renderable::Renderable(Int32 x, Int32 y, API::Texture &texture) : Tex(texture) {
			this->Pos = glm::vec2((Float32)x, (Float32)y);
			this->Dimensions = glm::vec2(this->Tex.GetDimensions());
			this->RealDimensions = Dimensions;
			this->Visible = true;
		}

		Renderable::Renderable(API::Texture &texture) : Tex(texture) {
			this->Pos = glm::vec2(0.0f, 0.0f);
			this->Dimensions = glm::vec2(this->Tex.GetDimensions());
			this->RealDimensions = Dimensions;
			this->Visible = true;
		}

		Renderable::~Renderable() {}

		API::Texture* Renderable::GetTexture() const { return &Tex; }
	}
}