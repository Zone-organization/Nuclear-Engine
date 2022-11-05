#include "Engine/Components/SkinnedMeshComponent.h"

namespace Nuclear
{
	namespace Components
	{
		SkinnedMeshComponent::SkinnedMeshComponent()
		{
		}

		SkinnedMeshComponent::SkinnedMeshComponent(Assets::Mesh* mesh, Assets::Material* material, Animation::Animator* animator)
		{
			mMesh = mesh;
			mMaterial = material;
			mAnimator = animator;
		}
	
		SkinnedMeshComponent::~SkinnedMeshComponent()
		{
		}
	}
}