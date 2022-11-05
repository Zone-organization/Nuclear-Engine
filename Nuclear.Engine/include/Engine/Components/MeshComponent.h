#pragma once
#include <Core\NE_Common.h>
#include <Core\Math\Math.h>
#include <vector>

namespace Nuclear {
	namespace Assets 
	{
		class Mesh;
		class Material;
	}

	namespace Animation {
		class Animator;
	}
	namespace Components 
	{

		class NEAPI MeshComponent
		{
		public:			
			MeshComponent();
			MeshComponent(Assets::Mesh *mesh);
			MeshComponent(Assets::Mesh *mesh , Assets::Material * material);
			~MeshComponent();

			//if false the RenderSystem won't render the model
			bool mRender = true;
			bool mCastShadows = true;
			bool mRecieveShadows = true;

			Assets::Mesh * mMesh;
			Assets::Material * mMaterial;
			Animation::Animator* mAnimator = nullptr;
		};

	}
}