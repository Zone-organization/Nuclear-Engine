#pragma once
#include <Components\Model.h>
#include <Shading\Material.h>
namespace NuclearEngine {
	namespace Components {

		class NEAPI Cube : Model
		{
		public:
			Cube();
			Cube(InputSignatures Signature, Shading::Material* mat);

			virtual void Draw(API::Shader* shader);

		protected:
			Mesh_NoIndices *cube;
		};
	}
}