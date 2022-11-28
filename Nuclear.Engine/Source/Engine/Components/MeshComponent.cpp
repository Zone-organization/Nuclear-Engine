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
				if (mEnableRendering)
				{
					if (pMaterial)
					{
						if (mMaterialDirty)
						{
							auto assetptr = pMaterial->GetShader();
							mPipelineCntrl.SetPipeline(&assetptr->mPipeline);
							mMaterialDirty = false;
						}

						Uint32 mAnimationHash = Utilities::Hash("NE_ANIMATION");
						Uint32 mRecieveShadowHash = Utilities::Hash("NE_SHADOWS");

						mPipelineCntrl.SetSwitch(mAnimationHash, bool(pAnimator));
						mPipelineCntrl.SetSwitch(mRecieveShadowHash, mReceiveShadows);

						for (auto& i : mCustomSwitches)
						{
							mPipelineCntrl.SetSwitch(i.first, i.second);
						}
						mPipelineCntrl.Update();
					}
					else {
						mEnableRendering = false;  //Dont render meshes with invalid material
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
		void MeshComponent::SetVariantSwitch(Uint32 VariantID, bool val)
		{
			mCustomSwitches[VariantID] = val;
			mDirty = true;
		}
		void MeshComponent::SetEnableRendering(bool val)
		{
			if (mEnableRendering != val)
			{
				mEnableRendering = val;
				mDirty = true;
			}
		}
		void MeshComponent::SetCastShadow(bool val)
		{
			if (mCastShadow != val)
			{
				mCastShadow = val;
				mDirty = true;
			}
		}
		void MeshComponent::SetReceiveShadows(bool val)
		{
			if (mReceiveShadows != val)
			{
				mReceiveShadows = val;
				mDirty = true;
			}
		}
		bool MeshComponent::GetEnableRendering() const
		{
			return mEnableRendering;
		}
		bool MeshComponent::GetCastShadow() const
		{
			return mCastShadow;
		}
		bool MeshComponent::GetReceiveShadows() const
		{
			return mReceiveShadows;
		}
		void MeshComponent::SetMaterial(Assets::Material* material)
		{
			pMaterial = material;
			mDirty = true;
			mMaterialDirty = true;
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