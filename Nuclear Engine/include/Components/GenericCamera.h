#pragma once
#include <NE_Common.h>
#include <Math\Math.h>
#include <API\ConstantBuffer.h>
#include <API\Shader.h>
//#include <Components\Component.h>

namespace NuclearEngine
{
	namespace Components
	{

		class NEAPI GenericCamera //: public Component
		{
		public:
			GenericCamera(Math::Vector3 _position = Math::Vector3(0.0f, 0.0f, 0.0f));
			~GenericCamera();


			void Initialize(Math::Matrix4 projectionMatrix);
			void Update();

			void SetModelMatrix(Math::Matrix4 modelMatrix);
			void SetViewMatrix(Math::Matrix4 viewMatrix);
			void SetProjectionMatrix(Math::Matrix4 projectionMatrix);
			void SetPosition(Math::Vector3 cameraposition);

			Math::Matrix4 GetModelMatrix();
			Math::Matrix4 GetViewMatrix();
			Math::Matrix4 GetProjectionMatrix();
			Math::Vector3 GetPosition();

			//Note: Shader CBuffer name is "NE_Camera", Binding Index Must be Zero!!
			API::ConstantBuffer* GetCBuffer();

		protected:

			struct CameraBuffer
			{
				Math::Matrix4 modelMatrix;
				Math::Matrix4 viewMatrix;
				Math::Matrix4 projectionMatrix;
			}_CameraBuffer;

			API::ConstantBuffer* ConstantBuffer;

			Math::Vector3 position, direction;
		};
	}
}