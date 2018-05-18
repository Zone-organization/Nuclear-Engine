#include "Engine\Components\MeshComponent.h"

namespace NuclearEngine 
{
	namespace Components
	{
		MeshComponent::MeshComponent(Assets::Mesh *mesh , Assets::Material * material)
		{
			mMesh = mesh;
			mMaterial = material;
		}
		MeshComponent::~MeshComponent()
		{
			mMesh = nullptr;
			mMaterial = nullptr;
		}
	}
}