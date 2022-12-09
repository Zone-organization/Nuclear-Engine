#pragma once
#include <NE_Common.h>
#include <Math\Math.h>
#include <Graphics\Color.h>

namespace Nuclear
{
	namespace Graphics
	{
		enum CAMERA_MOVEMENT {
			CAMERA_MOVEMENT_FORWARD,
			CAMERA_MOVEMENT_BACKWARD,
			CAMERA_MOVEMENT_LEFT,
			CAMERA_MOVEMENT_RIGHT
		};

		struct CameraBuffer
		{
			Math::Matrix4 Model = Math::Matrix4(1.0f);
			Math::Matrix4 ModelInvTranspose = Math::Matrix4(1.0f);
			Math::Matrix4 ModelViewProjection = Math::Matrix4(1.0f);

			Math::Matrix4 View = Math::Matrix4(1.0f);
			Math::Matrix4 Projection = Math::Matrix4(1.0f);
		};

		// Default camera values
		const float YAW = -90.0f;
		const float PITCH = 0.0f;
		const float SPEED = 2.5f;
		const float SENSITIVTY = 0.05f;
		const float ZOOM = 45.0f;

		class NEAPI Camera
		{
		public:
			Camera(Math::Vector3 __position, Math::Vector3 _Worldup, float yaw , float pitch , float speed , float sensitivity , float Zoom );
			~Camera();

			void ProcessEye(float xoffset, float yoffset, bool constrainPitch = true);
			void ProcessMovement(CAMERA_MOVEMENT direction, float deltaTime);
			void ProcessZoom(float yoffset);

			Math::Vector3 GetFrontView();

			void Initialize(Math::Matrix4 projectionMatrix);

			void UpdateBuffer();

			void SetModelMatrix(Math::Matrix4 modelMatrix);
			void SetViewMatrix(Math::Matrix4 viewMatrix);
			void SetProjectionMatrix(Math::Matrix4 projectionMatrix);
			void SetPosition(Math::Vector3 cameraposition);

			Math::Matrix4 GetModelMatrix();
			Math::Matrix4 GetViewMatrix();
			Math::Matrix4 GetProjectionMatrix();
			Math::Vector3 GetPosition();

			// Camera options
			float MovementSpeed;
			float MouseSensitivity;
			float Zoom;

			CameraBuffer mCameraData;
		
			Graphics::Color RTClearColor;
			//Rendering::Skybox* mSkybox = nullptr;
			//bool RenderSkybox = false;
		protected:

			// Eular Angles
			float Yaw;
			float Pitch;

			Math::Vector3 Front, Up, Right, WorldUp;

			Math::Vector3 position, direction;
		};
	}
}