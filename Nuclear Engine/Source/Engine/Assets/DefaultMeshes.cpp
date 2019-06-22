#include <Engine\Assets\DefaultMeshes.h>
namespace NuclearEngine
{
	namespace Assets
	{

		static Assets::Mesh gCube;
		static Assets::Mesh gSphere;

		void DefaultMeshes::Initialize()
		{
			Assets::Mesh::CreateCube(&gCube);
			Assets::Mesh::CreateSphere(&gSphere);
		}
		Assets::Mesh* DefaultMeshes::GetCubeAsset()
		{
			return &gCube;
		}
		Assets::Mesh* DefaultMeshes::GetSphereAsset()
		{
			return &gSphere;
		}
	}
}