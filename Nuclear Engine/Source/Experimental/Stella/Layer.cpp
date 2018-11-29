#include "Experimental/Stella/Layer.h"

#include <algorithm>

namespace NuclearEngine {
	namespace Graphics {
		/*Layer::Layer(Renderer *renderer, API::UnifiedShader *shader, glm::mat4 projection)
			: mRenderer(renderer), mShader(shader), Projection(projection) 
		{
		}*/

		Layer::~Layer() {
			for (auto i : this->Sprites)
				delete i;
			delete mRenderer;
		}

		void Layer::Add(Sprite *sprite) { this->Sprites.push_back(sprite); }

		void Layer::Remove(Sprite *sprite) {
			auto it = std::find(this->Sprites.begin(), this->Sprites.end(), sprite);
			this->Sprites.erase(it);
		}

		void Layer::Render() {

			//this->mShader->Bind();
			this->mRenderer->Begin();

			//glm::mat4 trans;
			//trans = glm::translate(trans, glm::vec3(360.0f, 202.0f, 0.0f));
			//trans = glm::scale(trans, glm::vec3(0.9f, 0.9f, 1.0f));
			//trans = glm::rotate(trans, glm::radians(-5.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			//trans = glm::translate(trans, glm::vec3(-360.0f, -202.0f, 0.0f));
			//this->mRenderer->PushTransformation(trans);

			for (auto i : Sprites)
				this->mRenderer->Submit(*i);

			//this->mRenderer->PopTransformation();

			this->mRenderer->End();
			this->mRenderer->Draw();
		}
	}
}