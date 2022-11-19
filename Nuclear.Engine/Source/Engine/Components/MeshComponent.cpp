#include "Engine\Components\MeshComponent.h"
#include "Engine\Assets\Material.h"
#include "Engine\Assets\Shader.h"

namespace Nuclear 
{
	namespace Components
	{
		MeshComponent::MeshComponent()
		{
			pMesh = nullptr;
			pMaterial = nullptr;
			pAnimator = nullptr;
		}
		MeshComponent::MeshComponent(Assets::Mesh* mesh, Assets::Material* material, Animation::Animator* Animator)
		{
			pMesh = mesh;
			pMaterial = material;
			pAnimator = Animator;
		}
		MeshComponent::~MeshComponent()
		{
			pMesh = nullptr;
			pMaterial = nullptr;
			pAnimator = nullptr;
		}
		void MeshComponent::Update(bool forcedirty)
		{
			if (mDirty)
			{
				if (pMaterial)
				{
					mPipelineCntrl.SetPipeline(&pMaterial->GetShader()->mPipeline);

					Uint32 mAnimationHash = Utilities::Hash("ANIMATION");
					Uint32 mRecieveShadowHash = Utilities::Hash("SHADOWS");
					
					mPipelineCntrl.SetSwitch(mAnimationHash, bool(pAnimator));
					mPipelineCntrl.SetSwitch(mRecieveShadowHash, mRecieveShadows);

					mPipelineCntrl.Update();
				}
				else {
					mRender = false;  //Dont render meshes with invalid material
					RenderQueue = -1;
				}

				mDirty = false;
			}
		}
		Uint32 MeshComponent::GetRenderQueue() const
		{
			return RenderQueue;
		}
		Graphics::ShaderPipelineVariant* MeshComponent::GetRenderingVariant()
		{
			return mPipelineCntrl.GetActiveVariant();
		}
		Assets::Mesh* MeshComponent::GetMesh()
		{
			return pMesh;
		}
		Assets::Material* MeshComponent::GetMaterial()
		{
			return pMaterial;
		}
		Animation::Animator* MeshComponent::GetAnimator()
		{
			return pAnimator;
		}
	}
}