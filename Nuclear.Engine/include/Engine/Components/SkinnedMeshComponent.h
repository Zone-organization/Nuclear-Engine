#pragma once
#include <Core\NE_Common.h>

namespace Nuclear {
	namespace Animation
	{
		class Animator;
	}
	namespace Assets
	{
		class Mesh;
		class Material;
	}
	namespace Components
	{

		class NEAPI SkinnedMeshComponent
		{
		public:
			SkinnedMeshComponent();
			SkinnedMeshComponent(Assets::Mesh* mesh, Assets::Material* material, Animation::Animator* animator);
			~SkinnedMeshComponent();

			//if false the RenderSystem won't render the model
			bool mRender = true;
			bool mCastShadows = true;
			bool mRecieveShadows = true;

			Assets::Mesh* mMesh;
			Assets::Material* mMaterial;

			Animation::Animator* mAnimator = nullptr;
		};

	}
}