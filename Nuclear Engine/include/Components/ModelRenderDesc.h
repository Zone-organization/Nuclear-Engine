#pragma once
#include <NE_Common.h>
#include <Transform\Transform.h>
#include <ECS\Entity.h>
#include <Graphics/API/VertexShader.h>
#include <Graphics/API/PixelShader.h>
namespace NuclearEngine
{
	namespace Components
	{
		/*
		Describes how the render system should treat and render a '3D' model
		Essential for rendering the model
		*/
		class NEAPI ModelRenderDesc : public ECS::Component<ModelRenderDesc>
		{
		public:
			//Custom shaders to be rendered with
			//Note: You Must Provide all shaders in order to work
			bool CustomShaders = false;
			Graphics::API::VertexShader VShader;
			Graphics::API::PixelShader PShader;

			//if false the RenderSystem won't render the model
			bool Render = true;
		};

	}
}

