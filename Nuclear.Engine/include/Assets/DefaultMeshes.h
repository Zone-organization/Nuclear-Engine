#pragma once
#include <Assets/Mesh.h>

namespace Nuclear
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

			static Assets::Mesh* GetScreenQuadAsset();
			static void RenderScreenQuad();
		};
	}
}