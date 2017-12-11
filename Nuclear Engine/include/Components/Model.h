#pragma once
#include <Components\Mesh.h>
#include <string>
#include <vector>

namespace NuclearEngine {
	namespace Components {

		struct Model_MaterialTex
		{
			MaterialTexture tex;
			const char* path;
		};

		class NEAPI Model
		{
		public:			
			Model();

			virtual void Draw(API::Shader* shader);
		private:
			std::vector<Mesh> meshes;
		};

	}
}