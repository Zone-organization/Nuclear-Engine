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

			void SetVariantSwitch(Uint32 VariantID, bool val);

			void SetEnableRender(bool val);
			void SetCastShadows(bool val);
			void SetRecieveShadows(bool val);

			bool GetEnableRender() const;
			bool GetCastShadows() const;
			bool GetRecieveShadows() const;

			void SetMaterial(Assets::Material* material);

			Assets::Mesh* GetMesh();
			Assets::Material* GetMaterial();
			Animation::Animator* GetAnimator();
		protected:
			Assets::Mesh* pMesh;
			Assets::Material* pMaterial;
			Animation::Animator* pAnimator;
			Graphics::ShaderPipelineSwitchController mPipelineCntrl;

			std::unordered_map<Uint32, bool> mCustomSwitches;

			Uint32 RenderQueue = 1;

			bool mDirty = true;
			bool mEnableRender = true;
			bool mCastShadows = true;
			bool mRecieveShadows = true;
		};

	}
}