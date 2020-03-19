#pragma once
#include <Base\NE_Common.h>
#include <Engine\ECS/Entity.h>
#include <Engine\ECS/System.h>
#include <Engine\Components\MeshComponent.h>
#include <Engine\Components\CameraComponent.h>
#include <Engine\Components\DirLightComponent.h>
#include <Engine\Components\PointLightComponent.h>
#include <Engine\Components\SpotLightComponent.h>
#include <Engine\Assets\Mesh.h>
#include <Engine\Assets\Material.h>
#include <Engine/Graphics/BakeStatus.h>
#include <Diligent/Common/interface/RefCntAutoPtr.hpp>
#include <Diligent/Graphics/GraphicsEngine/interface/Shader.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Buffer.h>
#include <Diligent/Graphics/GraphicsEngine/interface/Sampler.h>
#include <Diligent/Graphics/GraphicsEngine/interface/DepthStencilState.h>
#include <Diligent/Graphics/GraphicsEngine/interface/PipelineState.h>
#include <vector>

namespace NuclearEngine
{
	namespace Graphics
	{
		struct RenderingPipelineDesc
		{
			Uint32 DirLights = 0;
			Uint32 SpotLights = 0;
			Uint32 PointLights = 0;

			IBuffer* CameraBufferPtr = nullptr;
			IBuffer* LightsBufferPtr = nullptr;
		};

		class NEAPI RenderingPipeline
		{
		public:
			virtual bool Bake(const RenderingPipelineDesc& desc) = 0;

			IPipelineState* GetPipeline();
			virtual void ReflectPixelShaderData();

			Uint32 GetID();

			virtual Assets::Texture GetDefaultTextureFromType(Uint8 Type);
			
			virtual BakeStatus GetStatus();

			//This can be filled automatically by ReflectPixelShaderData(), Or fill it manually
			//Note: It is very important to fill it in order for material creation work with the pipeline.
			std::vector<Assets::ShaderTexture> mPixelShaderTextureInfo;
		protected:
			RefCntAutoPtr<IPipelineState> mPipeline;
			BakeStatus mStatus = BakeStatus::NotInitalized;
			Uint32 mID = 0;
		};

	}
}