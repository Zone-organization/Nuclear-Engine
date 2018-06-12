#pragma once
#include <Base\NE_Common.h>
#include <Base/Math/Math.h>
#include <vector>
#include "shader.h"
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
			Shader *Shad;
			glm::mat4 Projection;
			std::vector<Sprite *> Sprites;

			Layer(Renderer *renderer, Shader *shader, glm::mat4 projection);
		};
	} 
}
