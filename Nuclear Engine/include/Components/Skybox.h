#pragma once
#include <vector>
#include <API/VertexBuffer.h>
#include <API/IndexBuffer.h>
#include <API/TextureCube.h>
#include <API/InputLayout.h>
#include <API/PipelineState.h>
#include <Core/FileSystem.h>
#include <Components\GenericCamera.h>
#include <NE_Common.h>
#include "Renderer\Renderer.h"

namespace NuclearEngine
{
	namespace Components
	{
		class NEAPI Skybox
		{
			friend class Core::Renderer;
		private:
			API::Shader *shader;
			API::VertexBuffer *vertexBuffer;
			API::InputLayout *vertexBufferLayout;
			API::TextureCube *texcube;
			Components::GenericCamera* _CameraCbuffer;
			API::PipelineState* cubemapstate;
		public:
			Skybox(Components::GenericCamera* CameraCbuffer, std::array<NuclearEngine::Texture_Data, 6>);
			~Skybox();

			void Render();
		};
	}
}