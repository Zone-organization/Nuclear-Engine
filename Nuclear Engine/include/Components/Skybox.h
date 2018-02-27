#pragma once
#include <API/VertexBuffer.h>
#include <API/IndexBuffer.h>
#include <API/Texture.h>
#include <API/Sampler.h>
//#include <API/InputLayout.h>
#include <API/DepthStencilState.h>
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
			static void Create(Skybox* skybox, Components::GenericCamera* CameraCbuffer, const std::array<API::Texture_Data, 6>& data);
			static void Create(Skybox* skybox, Components::GenericCamera* CameraCbuffer, const std::array<std::string, 6>& paths);

			API::VertexShader v_shader;
			API::PixelShader p_shader;
			API::VertexBuffer m_vb;
			API::Texture m_texcube;
			API::Sampler m_sampler;
			Components::GenericCamera* m_camera;
			API::DepthStencilState m_ds_state;
		};
	}
}