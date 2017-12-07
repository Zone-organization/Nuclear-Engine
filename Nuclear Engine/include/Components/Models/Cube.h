#pragma once
#include <Components\Model.h>
namespace NuclearEngine {
	namespace Components {

		struct Material {
			API::Texture* Diffuse = nullptr;
			API::Texture* Specular = nullptr;
		};

		class NEAPI Cube : Model
		{
		public:
			Cube();
			Cube(InputSignatures Signature, Material* mat);

			virtual void Draw(API::Shader* shader);

		protected:
			Mesh_NoIndices *cube;
		};
	}
}