#include <Engine\Components\Renderable2D.h>

namespace NuclearEngine {
	namespace Components {

		Renderable2D::Renderable2D(int x, int y, int w, int h, Graphics::API::Texture *texture,bool visible)
			: Tex(texture) {
			//TODO: Remove slow and errorous float casts
			this->Pos = Math::Vector2((float)x, (float)y);
			this->Dimensions = Math::Vector2((float)w, (float)h);
			this->RealDimensions = Dimensions;
			this->Visible = visible;
		}

		Renderable2D::Renderable2D(int x, int y, Graphics::API::Texture *texture) : Tex(texture) {
			this->Pos = Math::Vector2((float)x, (float)y);
			this->Dimensions = Math::Vector2(this->Tex->GetWidth(), this->Tex->GetHeight());
			this->RealDimensions = Dimensions;
			this->Visible = true;
		}

		Renderable2D::Renderable2D(Graphics::API::Texture *texture) : Tex(texture) {
			this->Pos = Math::Vector2(0.0f, 0.0f);
			this->Dimensions = Math::Vector2(this->Tex->GetWidth(), this->Tex->GetHeight());
			this->RealDimensions = Dimensions;
			this->Visible = true;
		}

		Renderable2D::~Renderable2D() {}

		Graphics::API::Texture * Renderable2D::GetTexture() const { return Tex; }
	}
}