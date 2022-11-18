#include "Engine\Components\MeshComponent.h"
#include "Engine\Assets\Material.h"
#include "Engine\Assets\Shader.h"

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
		void MeshComponent::UpdateRenderQueue(bool forcedirty)
		{
		}
		Uint32 MeshComponent::GetRenderQueue()
		{
			if (mMaterial)
			{
				if(mAnimator)
					mMaterial->GetShader()->mPipeline.GetVariant()
				else


				//if()
				//mMaterial->
			}
			return Uint32();
		}
		Graphics::ShaderPipelineVariant* MeshComponent::GetRenderingVariant()
		{
			return mVariant;
		}
	}
}