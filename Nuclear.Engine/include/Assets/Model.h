#pragma once
#include <Assets/IAsset.h>
#include <Assets\MaterialTypes.h>

namespace Nuclear
{
	namespace Assets
	{
		class Mesh;
		class Animations;

		class NEAPI Model : public IAsset
		{
		public:
			Model();

			Assets::MaterialCreationInfo mMaterialInfo;
			Assets::Mesh* pMesh = nullptr;
			Assets::Animations* pAnimations = nullptr;
		};
	}
}