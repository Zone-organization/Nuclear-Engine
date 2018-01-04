#pragma once
#include <API\Texture.h>
#include <API\Color.h>
#include <API\Shader.h>
#include <map>

namespace NuclearEngine {
	namespace Shading {

		class NEAPI Material {
		public:
			
			API::Texture *Diffuse = nullptr;
			API::Texture *Specular = nullptr;

		};
	}
}