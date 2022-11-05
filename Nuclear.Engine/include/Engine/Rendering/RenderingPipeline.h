#pragma once
#include <Core\NE_Common.h>
#include <Engine/Graphics/Camera.h>
#include <Engine/Rendering/ShaderEffect.h>
#include <Engine/Rendering/ShadingModel.h>
#include <Engine/Graphics/BakeStatus.h> 
#include <Engine\Components\MeshComponent.h>
#include <Engine\Components\SkinnedMeshComponent.h>

namespace Nuclear
{
	namespace Systems {
		class RenderSystem;
	}

	namespace Rendering
	{
		//Defines how to render stuff
		class NEAPI RenderingPipeline
		{
		public:
			RenderingPipeline();

			virtual void BeginFrame(FrameRenderData* framedata) = 0;

			virtual void StartShaderModelRendering(ShadingModel* sm) = 0;

			virtual void Render(Components::MeshComponent& mesh, const Math::Matrix4& modelmatrix) = 0;

			virtual void Render(Components::SkinnedMeshComponent& skinnedmesh, const Math::Matrix4& modelmatrix) = 0;

			virtual void FinishShaderModelRendering() = 0;

			// Render A Mesh (expects the pipeline to be bound)
			virtual void InstantRender(Assets::Mesh* mesh, Assets::Material* material);
		};
	}
}