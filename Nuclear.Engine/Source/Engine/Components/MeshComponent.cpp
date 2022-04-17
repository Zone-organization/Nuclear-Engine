#include "Engine\Components\MeshComponent.h"

namespace Nuclear 
{
	namespace Components
	{
		MeshComponent::MeshComponent()
		{
		}
		MeshComponent::MeshComponent(Assets::Mesh * mesh)
		{
			mMesh = mesh;
		}
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