#pragma once
#include <Shading\Technique.h>

namespace NuclearEngine {
	namespace Shading {
		namespace Techniques {

			class NEAPI PhongShading : public Technique
			{
			public:
				PhongShading()
				{
					this->m_shaderpath = "Assets/NuclearEngine/Shaders/Techniques/PhongShading.hlsl";
				}
			};

		}
	}
}