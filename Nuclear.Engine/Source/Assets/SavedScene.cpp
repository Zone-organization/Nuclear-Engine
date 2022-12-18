#include <Assets\SavedScene.h>
#include <sstream>
#include <Components\Components.h>
#include <Systems\PhysXSystem.h>
#include <Assets/DefaultMeshes.h>
#include "..\PhysX\PhysXTypes.h"
#include <Assets/Material.h>
#include <Core/Engine.h>
#include <Utilities/Logger.h>

namespace Nuclear
{
	namespace Assets
	{
		SavedScene::SavedScene()
			: IAsset(AssetType::SerializedScene)
		{
		}
		SavedScene::~SavedScene()
		{
		}
	}
}