#pragma once
#include <NE_Common.h>
#include <Math\Math.h>
#include <Graphics\ShaderPipelineSwitch.h>
#include <Serialization/Access.h>
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

			void SetEnableRendering(bool val);
			void SetCastShadow(bool val);
			void SetReceiveShadows(bool val);

			bool GetEnableRendering() const;
			bool GetCastShadow() const;
			bool GetReceiveShadows() const;

			void SetMaterial(Assets::Material* material);

			Assets::Mesh* GetMesh();
			Assets::Material* GetMaterial();
			Animation::Animator* GetAnimator();

			void SetRenderSystemFlags(bool hasdefferedpass, bool hasshadowpass);
		protected:
			friend Serialization::Access;

			Assets::Mesh* pMesh;
			Assets::Material* pMaterial;
			Animation::Animator* pAnimator;
			Graphics::ShaderPipelineSwitchController mPipelineCntrl;

			std::unordered_map<Uint32, bool> mCustomSwitches;

			Uint32 RenderQueue = 1;

			//Switches
			bool mEnableRendering = true;
			bool mCastShadow = true;
			bool mReceiveShadows = true;

			//Internal
			bool mRequestDeffered = false;
			bool mMaterialDirty = true;
			bool mDirty = true;
			bool mRenderSystemHasDefferedPass = false;
			bool mRenderSystemHasShadowPass = false;

			constexpr static auto serialize(auto& archive, auto& self)
			{
				return archive(self.mEnableRendering, self.mCastShadow, self.mReceiveShadows);
			}
		};

	}
}