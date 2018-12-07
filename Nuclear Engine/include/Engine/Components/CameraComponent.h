#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>
#include <LLGL/Buffer.h>
#include <Engine\ECS\Entity.h>

namespace NuclearEngine
{
	namespace Components
	{
		enum Camera_Movement {
			FORWARD,
			BACKWARD,
			LEFT,
			RIGHT
		};

		// Default camera values
		const float YAW = -90.0f;
		const float PITCH = 0.0f;
		const float SPEED = 2.5f;
		const float SENSITIVTY = 0.05f;
		const float ZOOM = 45.0f;

		class NEAPI CameraComponent : public ECS::Component<CameraComponent>
		{
		public:
			CameraComponent();
			CameraComponent(Math::Vector3 __position, Math::Vector3 _Worldup = Math::Vector3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, float speed = SPEED, float sensitivity = SENSITIVTY, float Zoom = ZOOM);
			~CameraComponent();

			void ProcessEye(float xoffset, float yoffset, bool constrainPitch = true);
			void ProcessMovement(Camera_Movement direction, float deltaTime);
			void ProcessZoom(float yoffset);

			Math::Vector3 GetFrontView();

			void Initialize(Math::Matrix4 projectionMatrix);
			void Update();
			//Note: Doesn't update the constant buffer, it calculates the ModelInvTranspose, ModelViewProjection
			void UpdateMatricesOnly();

			void SetModelMatrix(Math::Matrix4 modelMatrix);
			void SetViewMatrix(Math::Matrix4 viewMatrix);
			void SetProjectionMatrix(Math::Matrix4 projectionMatrix);
			void SetPosition(Math::Vector3 cameraposition);

			void SetActive();

			Math::Matrix4 GetModelMatrix();
			Math::Matrix4 GetViewMatrix();
			Math::Matrix4 GetProjectionMatrix();
			Math::Vector3 GetPosition();

			//Note: Shader CBuffer name is "NE_Camera", Binding Index Must be Zero!!
			LLGL::Buffer* GetCBuffer();

		protected:
			// Eular Angles
			float Yaw;
			float Pitch;
			// Camera options
			float MovementSpeed;
			float MouseSensitivity;
			float Zoom;

			Math::Vector3 Front, Up, Right, WorldUp;

			struct CameraBuffer
			{
				Math::Matrix4 Model = Math::Matrix4(1.0f);
				Math::Matrix4 ModelInvTranspose = Math::Matrix4(1.0f);
				Math::Matrix4 ModelViewProjection = Math::Matrix4(1.0f);

				//Needed for some objects (as skybox & 2D Sprites & etc)
				Math::Matrix4 View = Math::Matrix4(1.0f);
				Math::Matrix4 Projection = Math::Matrix4(1.0f);
			}_CameraBuffer;

			LLGL::Buffer* ConstantBuffer;

			Math::Vector3 position, direction;
		};

		typedef CameraComponent Camera;
	}
}