#pragma once
#include <Shading\Technique.h>

namespace NuclearEngine {
	namespace Shading {
		namespace Techniques {

			class NEAPI PhongShading : Technique
			{
			public:
				PhongShading()
				{
					this->m_type = Technique_Type::LightShading;

					this->m_shaderpath = "Assets/NuclearEngine/Shaders/Techniques/PhongShading.hlsl";
				}
			};

		}
	}
}