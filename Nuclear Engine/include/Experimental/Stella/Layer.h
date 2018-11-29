#pragma once
#include <Base\NE_Common.h>
#include <Base/Math/Math.h>
#include <vector>
//#include "Engine\Graphics\API\UnifiedShader.h"
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
			Renderer * mRenderer;
			//API::UnifiedShader *mShader;
			glm::mat4 Projection;
			std::vector<Sprite *> Sprites;

			//Layer(Renderer *renderer, API::UnifiedShader *shader, glm::mat4 projection);
		};
	} 
}
