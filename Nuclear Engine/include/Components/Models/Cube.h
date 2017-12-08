#pragma once
#include <Components\Model.h>
namespace NuclearEngine {
	namespace Components {

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