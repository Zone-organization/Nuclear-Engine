#pragma once
#include <Base\NE_Common.h>
#include "animator.h"
#include "renderable.h"
#include "spritesheet.h"

namespace NuclearEngine {
	namespace Graphics {

		struct VertexData {
			glm::vec3 vertex;
			glm::vec2 uv;
			Float32 tid;
			unsigned int color;
		};

		class Sprite : public Renderable {
		public:
			Animator Animations;

			Sprite(Int32 x, Int32 y, API::Texture &texture, const std::vector<Uint32> &frames);
			Sprite(Int32 x, Int32 y, API::Texture &texture, Uint32 frame = 0);
			Sprite(Int32 x, Int32 y, Int32 w, Int32 h, API::Texture &texture,
				Uint32 frame = 0);
			~Sprite();

			void Update();

			// Getters
			inline unsigned int GetColor() const { return Color; }
			inline Uint32 GetCurrentFrame() const { return Frame; }
			inline SpriteSheet GetSpriteSheet() const { return *Sprites; }
			inline glm::vec2 GetFrameCoords() const { return Sprites->GetUV(this->Frame); }

			// Setters
			void SetFrame(Uint32 frame);
			void SetDirectFrame(Uint32 frame);
			void SetColorRGB(int r, int g, int b);
			void SetColorRGBA(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
			void SetAlpha(int a);
			void Flip() { this->SetScale(glm::vec2(-1.f, 0.f)); }

		private:
			//glm::vec4 Color;
			unsigned int Color;
			std::vector<Uint32> Frames;
			Uint32 Frame = 0, NumberOfFrames;
			SpriteSheet *Sprites;
		};
	} 
}