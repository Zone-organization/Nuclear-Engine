#pragma once
#include <NE_Common.h>
#include <Components\ShaderStructs.h>
#include <Graphics\Color.h>
#include <Graphics/RenderTarget.h>

namespace Nuclear
{
	namespace Components
	{
		enum CAMERA_MOVEMENT {
			CAMERA_MOVEMENT_FORWARD,
			CAMERA_MOVEMENT_BACKWARD,
			CAMERA_MOVEMENT_LEFT,
			CAMERA_MOVEMENT_RIGHT
		};

		// Default camera values
		const float YAW = -90.0f;
		const float PITCH = 0.0f;
		const float SPEED = 2.5f;
		const float SENSITIVTY = 0.05f;
		const float ZOOM = 45.0f;


		class NEAPI CameraComponent
		{
		public:
			CameraComponent();
			CameraComponent(Math::Matrix4 projectionMatrix, float yaw = YAW, float pitch = PITCH, float speed = SPEED, float sensitivity = SENSITIVTY, float Zoom = ZOOM);
			~CameraComponent();

			void ProcessEye(float xoffset, float yoffset, bool constrainPitch = true);
			void ProcessMovement(CAMERA_MOVEMENT direction, float deltaTime);
			void ProcessZoom(float yoffset);

			Math::Vector3 GetFrontView();

			void UpdateBuffer();

			void SetModelMatrix(Math::Matrix4 modelMatrix);
			void SetViewMatrix(Math::Matrix4 viewMatrix);
			void SetProjectionMatrix(Math::Matrix4 projectionMatrix);

			Math::Matrix4 GetModelMatrix();
			Math::Matrix4 GetViewMatrix();
			Math::Matrix4 GetProjectionMatrix();

			void SetPosition(Math::Vector3 cameraposition);
			Math::Vector3 GetPosition();

			Graphics::RenderTarget& GetRenderTarget();
			Graphics::RenderTarget& GetDepthRenderTarget();


			// Camera options
			float MovementSpeed;
			float MouseSensitivity;
			float Zoom;

			CameraBuffer mCameraData;

			Graphics::Color mRTClearColor;

			//Rendering::Skybox* mSkybox = nullptr;
			//bool RenderSkybox = false;
		protected:

			// Eular Angles
			float Yaw;
			float Pitch;

			Math::Vector3 Front, Up, Right;
			Math::Vector3 WorldUp = Math::Vector3(0.0f, 1.0f, 0.0f);

			Math::Vector3 Position, direction;

			Graphics::RenderTarget mRT;
			Graphics::RenderTarget mDepthRT;
		};
	}
}