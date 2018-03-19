#pragma once
#include <Graphics/API/VertexBuffer.h>
#include <Graphics/API/IndexBuffer.h>
#include <Graphics/API/Texture.h>
#include <Graphics/API/Sampler.h>
//#include <Graphics/API/InputLayout.h>
#include <Graphics/API/DepthStencilState.h>
#include <Core/FileSystem.h>
#include <Components\GenericCamera.h>
#include <Core\Entity.h>

namespace NuclearEngine
{
	namespace Components
	{			
		//TODO: Move the Vertexbuffer, sampler, state, shader to a global manager instead of recreating them at each object
		// Move the create function to a manager since components shouldn't have any logic at all!!
		class NEAPI Skybox : public Core::Component<Skybox>
		{
		public:
			Skybox();
			~Skybox();
			//TODO:move these functions from here as
			static void Create(Skybox* skybox, Components::GenericCamera* CameraCbuffer, const std::array<Graphics::API::Texture_Data, 6>& data);
			static void Create(Skybox* skybox, Components::GenericCamera* CameraCbuffer, const std::array<std::string, 6>& paths);

			Graphics::API::VertexShader v_shader;
			Graphics::API::PixelShader p_shader;
			Graphics::API::VertexBuffer m_vb;
			Graphics::API::Texture m_texcube;
			Graphics::API::Sampler m_sampler;
			Components::GenericCamera* m_camera;
			Graphics::API::DepthStencilState m_ds_state;
		};
	}
}