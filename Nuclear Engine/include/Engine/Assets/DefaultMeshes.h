#pragma once
#include <Engine/Assets/Mesh.h>

namespace NuclearEngine
{
	namespace Assets
	{
		class NEAPI DefaultMeshes
		{
		public:
			static void Initialize();

			static Assets::Mesh* GetCubeAsset();
			static Assets::Mesh* GetSphereAsset();
			static Assets::Mesh* GetPlaneAsset();
		};
	}
}