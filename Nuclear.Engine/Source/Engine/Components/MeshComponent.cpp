#include "Engine\Components\MeshComponent.h"

namespace Nuclear 
{
	namespace Components
	{
		MeshComponent::MeshComponent()
		{
			mMesh = nullptr;
			mMaterial = nullptr;
			mAnimator = nullptr;
		}
		MeshComponent::MeshComponent(Assets::Mesh* mesh, Assets::Material* material, Animation::Animator* Animator)
		{
			mMesh = mesh;
			mMaterial = material;
			mAnimator = Animator;
		}
		MeshComponent::~MeshComponent()
		{
			mMesh = nullptr;
			mMaterial = nullptr;
			mAnimator = nullptr;
		}
	}
}