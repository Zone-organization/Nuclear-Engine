#pragma once
#include <Base\NE_Common.h>
#include <Base/Math/Math.h>
#include <vector>
#include "Engine\Graphics\API\PixelShader.h"
#include "sprite.h"
#include "renderer.h"

namespace NuclearEngine {
	namespace Graphics {

		class Layer {
		public:
			virtual ~Layer();
			virtual void Add(Sprite *sprite);
			virtual void Remove(Sprite *sprite);
			virtual void Render();

		protected:
			Renderer * Ren;
			API::PixelShader *Shad;
			glm::mat4 Projection;
			std::vector<Sprite *> Sprites;

			Layer(Renderer *renderer, API::PixelShader *shader, glm::mat4 projection);
		};
	} 
}
