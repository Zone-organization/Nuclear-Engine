#include "Engine\Components\MeshComponent.h"
#include "Engine\Assets\Material.h"
#include "Engine\Assets\Shader.h"

namespace Nuclear 
{
	namespace Components
	{
		MeshComponent::MeshComponent()
		{
			RenderQueue = 1;
			pMesh = nullptr;
			pMaterial = nullptr;
			pAnimator = nullptr;
		}
		MeshComponent::MeshComponent(Assets::Mesh* mesh, Assets::Material* material, Animation::Animator* Animator)
		{
			RenderQueue = 1;
			pMesh = mesh;
			pMaterial = material;
			pAnimator = Animator;
		}
		MeshComponent::~MeshComponent()
		{
			RenderQueue = 1;
			pMesh = nullptr;
			pMaterial = nullptr;
			pAnimator = nullptr;
		}
		void MeshComponent::Update(bool forcedirty)
		{
			if (mDirty)
			{
				if (mEnableRender)
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
						mEnableRender = false;  //Dont render meshes with invalid material
						RenderQueue = 0;
					}
				}
				else
				{
					RenderQueue = 0;
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
		void MeshComponent::SetEnableRender(bool val)
		{
			if (mEnableRender != val)
			{
				mEnableRender = val;
				mDirty = true;
			}
		}
		void MeshComponent::SetCastShadows(bool val)
		{
			if (mCastShadows != val)
			{
				mCastShadows = val;
				mDirty = true;
			}
		}
		void MeshComponent::SetRecieveShadows(bool val)
		{
			if (mRecieveShadows != val)
			{
				mRecieveShadows = val;
				mDirty = true;
			}
		}
		bool MeshComponent::GetEnableRender() const
		{
			return mEnableRender;
		}
		bool MeshComponent::GetCastShadows() const
		{
			return mCastShadows;
		}
		bool MeshComponent::GetRecieveShadows() const
		{
			return mRecieveShadows;
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