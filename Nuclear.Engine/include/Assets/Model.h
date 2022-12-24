#pragma once
#include <Assets/IAsset.h>

namespace Nuclear
{
	namespace Assets
	{
		class Mesh;
		class MaterialData;
		class Animations;

		class NEAPI Model : public IAsset
		{
		public:
			Model();
			Assets::Mesh* pMesh = nullptr;
			Assets::MaterialData* pMaterialData = nullptr;
			Assets::Animations* pAnimations = nullptr;
		};
	}
}