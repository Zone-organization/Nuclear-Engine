#pragma once
#include <vector>
#include <API/VertexBuffer.h>
#include <API/IndexBuffer.h>
#include <API/Texture.h>
#include <API/InputLayout.h>
#include <API/DepthStencilState.h>
#include <Core/FileSystem.h>
#include <Components\GenericCamera.h>
#include <NE_Common.h>

namespace NuclearEngine
{
	namespace Components
	{
		class NEAPI Skybox
		{
		private:
			API::Shader shader;
			API::VertexBuffer vertexBuffer;
			API::Texture texcube;
			Components::GenericCamera* _CameraCbuffer;
			API::DepthStencilState cubemapstate;
		public:
			Skybox(Components::GenericCamera* CameraCbuffer, std::array<NuclearEngine::Texture_Data, 6>);
			~Skybox();

			void Render();
		};
	}
}