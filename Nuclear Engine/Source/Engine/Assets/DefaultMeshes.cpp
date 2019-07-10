#include <Engine\Assets\DefaultMeshes.h>
namespace NuclearEngine
{
	namespace Assets
	{

		static Assets::Mesh gCube;
		static Assets::Mesh gSphere;
		static Assets::Mesh gPlane;

		void DefaultMeshes::Initialize()
		{
			Assets::Mesh::CreateCube(&gCube);
			Assets::Mesh::CreateSphere(&gSphere);
			Assets::Mesh::CreatePlane(&gPlane);
		}
		Assets::Mesh* DefaultMeshes::GetCubeAsset()
		{
			return &gCube;
		}
		Assets::Mesh* DefaultMeshes::GetSphereAsset()
		{
			return &gSphere;
		}
		Assets::Mesh* DefaultMeshes::GetPlaneAsset()
		{
			return &gPlane;
		}
	}
}