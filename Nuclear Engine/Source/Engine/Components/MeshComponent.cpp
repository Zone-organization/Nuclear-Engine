#include "Engine\Components\MeshComponent.h"

namespace NuclearEngine 
{
	namespace Components
	{
		MeshComponent::MeshComponent()
		{
		}
		MeshComponent::MeshComponent(Assets::Mesh * mesh, bool MultiRender)
		{
			mMesh = mesh;
			mMultiRender = MultiRender;
		}
		MeshComponent::MeshComponent(Assets::Mesh *mesh , Assets::Material * material, bool MultiRender)
		{
			mMesh = mesh;
			mMaterial = material;
			mMultiRender = MultiRender;
		}
		MeshComponent::~MeshComponent()
		{
			mMesh = nullptr;
			mMaterial = nullptr;
		}
	}
}