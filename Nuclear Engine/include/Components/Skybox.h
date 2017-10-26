//#pragma once
//#include <vector>
//#include <API/VertexBuffer.h>
//#include <API/IndexBuffer.h>
//#include <API/TextureCube.h>
//#include <API/InputLayout.h>
//#include <NE_Common.h>
//
//namespace NuclearEngine
//{
//	namespace Core {
//		class Renderer;
//	}
//	namespace Components
//	{
//		class NEAPI Skybox
//		{
//			friend class Core::Renderer;
//		private:
//			API::Shader *shader;
//			API::VertexBuffer *vertexBuffer;
//			API::InputLayout *vertexBufferLayout;
//			API::TextureCube textureCube;
//			API::UniformBuffer* _CameraCbuffer;
//		public:
//			Skybox(API::UniformBuffer* CameraCbuffer);
//			~Skybox();
//
//			void Load(const std::vector<std::string>& filenames);
//			void Render();
//		};
//	}
//}