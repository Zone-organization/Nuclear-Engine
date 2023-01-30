#pragma once
#include <string>

namespace Nuclear
{
	namespace Assets
	{
		class Mesh;
		class Material;
		class Animations;
		namespace Importers
		{
			//Not an asset
			struct Model
			{
				std::string mName;
				Assets::Mesh* pMesh = nullptr;
				Assets::Material* pMaterial = nullptr;
				Assets::Animations* pAnimations = nullptr;
			};

		}
	}
}