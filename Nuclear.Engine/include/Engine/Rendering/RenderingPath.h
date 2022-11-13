#pragma once
#include <Core\NE_Common.h>
#include <Engine/Graphics/Camera.h>
#include <Engine/Rendering/ShaderEffect.h>
#include <Engine/Rendering/ShadingModel.h>
#include <Engine/Graphics/BakeStatus.h> 
#include <Engine\Components\MeshComponent.h>

namespace Nuclear
{
	namespace Systems {
		class RenderSystem;
	}

	namespace Rendering
	{
		struct FrameRenderData;
		class ShaderPipeline;

		//Defines how to render stuff
		class NEAPI RenderingPath
		{
		public:
			RenderingPath();

			virtual void BeginFrame(FrameRenderData* framedata);

			virtual void UpdateAnimationCB(Animation::Animator* mAnimator);

			//Static meshes
			virtual void StartRendering(ShaderPipeline* pipeline) = 0;

			virtual void Render(Components::MeshComponent& mesh, const Math::Matrix4& modelmatrix) = 0;

		protected:
			void DrawMesh(Assets::Mesh* mesh, Assets::Material* material);
			ShaderPipeline* pActivePipeline = nullptr;
			FrameRenderData* pCurrentFrame = nullptr;
		};
	}
}