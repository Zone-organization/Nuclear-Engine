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
		//Defines how to render stuff
		class NEAPI RenderingPipeline
		{
		public:
			RenderingPipeline();

			virtual void BeginFrame(FrameRenderData* framedata) = 0;


			//Static meshes
			virtual void StartStaticShaderModelRendering(ShadingModel* sm) = 0;

			virtual void RenderStatic(Components::MeshComponent& mesh, const Math::Matrix4& modelmatrix) = 0;

			//Skinned meshes
			virtual void StartSkinnedShaderModelRendering(ShadingModel* sm) = 0;

			virtual void RenderSkinned(Components::MeshComponent& skinnedmesh, const Math::Matrix4& modelmatrix) = 0;

			virtual void FinishAllRendering() = 0;

		protected:
			// Render A Mesh (expects the pipeline to be bound)
			virtual void DrawStaticMesh(Assets::Mesh* mesh, Assets::Material* material);

			virtual void DrawSkinnedMesh(Assets::Mesh* mesh, Assets::Material* material);

			virtual void CheckValidMesh(Assets::Mesh* mesh, Assets::Material* material);

			bool mSkinnedRendering = false;
		};
	}
}