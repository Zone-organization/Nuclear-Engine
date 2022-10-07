#pragma once
#include <Core\NE_Common.h>
#include <Engine/Graphics/Camera.h>
#include <Engine/Rendering/ShaderEffect.h>
#include <Engine/Rendering/ShadingModel.h>
#include <Engine/Graphics/BakeStatus.h> 
#include <unordered_map>

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

			virtual void StartRendering(FrameRenderData* framedata, DrawQueue* queue) = 0;

			virtual void RenderFrame(FrameRenderData* framedata, DrawQueue* queue) = 0;

			virtual void FinishRendering(FrameRenderData* framedata, DrawQueue* queue) = 0;


			virtual void RenderQueue(FrameRenderData* framedata, DrawQueue* queue) = 0;

			// Render A Mesh (expects the pipeline to be bound)
			virtual void InstantRender(Assets::Mesh* mesh, Assets::Material* material);
		};
	}
}