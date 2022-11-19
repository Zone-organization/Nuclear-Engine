#pragma once
#include <Core\NE_Common.h>
#include <Core\Math\Math.h>
#include <Engine\Graphics\ShaderPipelineSwitch.h>
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

	namespace Graphics {
		class ShaderPipelineVariant;
	}
	namespace Components 
	{

		class NEAPI MeshComponent
		{
		public:			
			MeshComponent();
			MeshComponent(Assets::Mesh* mesh, Assets::Material* material = nullptr, Animation::Animator* Animator = nullptr);
			~MeshComponent();

			void Update(bool forcedirty = false);
			Uint32 GetRenderQueue() const;
			Graphics::ShaderPipelineVariant* GetRenderingVariant();

			//if false the RenderSystem won't render the model
			bool mRender = true;
			bool mCastShadows = true;
			bool mRecieveShadows = true;

			Assets::Mesh* GetMesh();
			Assets::Material* GetMaterial();
			Animation::Animator* GetAnimator();
		protected:
			Assets::Mesh* pMesh;
			Assets::Material* pMaterial;
			Animation::Animator* pAnimator;
			Graphics::ShaderPipelineSwitchController mPipelineCntrl;
			Uint32 RenderQueue = -1;

			bool mDirty = true;

		};

	}
}