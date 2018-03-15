#pragma once
#include <NE_Common.h>
#include <Math\Math.h>
#include <API\ConstantBuffer.h>

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
			//Note: Doesn't update the constant buffer, it calculates the ModelInvTranspose, ModelViewProjection
			void UpdateMatricesOnly();

			void SetModelMatrix(Math::Matrix4 modelMatrix);
			void SetViewMatrix(Math::Matrix4 viewMatrix);
			void SetProjectionMatrix(Math::Matrix4 projectionMatrix);
			void SetPosition(Math::Vector3 cameraposition);

			Math::Matrix4 GetModelMatrix();
			Math::Matrix4 GetViewMatrix();
			Math::Matrix4 GetProjectionMatrix();
			Math::Vector3 GetPosition();

			//Note: Shader CBuffer name is "NE_Camera", Binding Index Must be Zero!!
			API::ConstantBuffer GetCBuffer();

		protected:

			struct CameraBuffer
			{
				Math::Matrix4 Model = Math::Matrix4(1.0f);
				Math::Matrix4 ModelInvTranspose = Math::Matrix4(1.0f);
				Math::Matrix4 ModelViewProjection = Math::Matrix4(1.0f);

				//Needed for some objects (as skybox & 2D Sprites & etc)
				Math::Matrix4 View = Math::Matrix4(1.0f);
				Math::Matrix4 Projection = Math::Matrix4(1.0f);
			}_CameraBuffer;

			API::ConstantBuffer ConstantBuffer;

			Math::Vector3 position, direction;
		};
	}
}