#pragma once
#include <vector>
#include <API/VertexBuffer.h>
#include <API/IndexBuffer.h>
#include <API/TextureCube.h>
#include <API/InputLayout.h>
#include <API/PipelineState.h>
#include <Core/FileSystem.h>
#include <NE_Common.h>
#include "Core\Renderer3D.h"

namespace NuclearEngine
{
	namespace Components
	{
		class NEAPI Skybox
		{
			friend class Core::Renderer3D;
		private:
			API::Shader *shader;
			API::VertexBuffer *vertexBuffer;
			API::InputLayout *vertexBufferLayout;
			API::TextureCube *texcube;
			API::UniformBuffer* _CameraCbuffer;
			API::PipelineState* cubemapstate;
		public:
			Skybox(API::UniformBuffer* CameraCbuffer, std::array<NuclearEngine::Texture_Data, 6>);
			~Skybox();

			void Render();
		};
	}
}