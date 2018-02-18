#pragma once
#include <vector>
#include <API/VertexBuffer.h>
#include <API/IndexBuffer.h>
#include <API/Texture.h>
#include <API/Sampler.h>
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
			API::Sampler mSampler;
			Components::GenericCamera* _CameraCbuffer;
			API::DepthStencilState cubemapstate;
		public:
			Skybox();
			~Skybox();
			void Create(Components::GenericCamera* CameraCbuffer, const std::array<API::Texture_Data, 6>& data);
			void Create(Components::GenericCamera* CameraCbuffer, const std::array<std::string, 6>& paths);
			void Render();
		};
	}
}