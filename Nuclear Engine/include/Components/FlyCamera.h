#pragma once
#include <Components\GenericCamera.h>

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
		const float SPEED = 0.5f;
		const float SENSITIVTY = 0.1f;
		const float ZOOM = 45.0f;

		class NEAPI FlyCamera  : public GenericCamera
		{
		public:
			FlyCamera(Math::Vector3 __position = Math::Vector3(0.0f, 0.0f, 0.0f) , Math::Vector3 _Worldup = Math::Vector3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH, float speed = SPEED, float sensitivity = SENSITIVTY, float Zoom = ZOOM);
			~FlyCamera();

			void ProcessEye(float xoffset, float yoffset, bool constrainPitch = true);
			void ProcessMovement(Camera_Movement direction, float deltaTime);
			void ProcessZoom(float yoffset);

			Math::Vector3 GetFrontView();

			//void Initialize(Math::Matrix4 projectionMatrix) override;
			void Update();
		protected:
			// Eular Angles
			float Yaw;
			float Pitch;
			// Camera options
			float MovementSpeed;
			float MouseSensitivity;
			float Zoom;

			Math::Vector3 Front, Up , Right, WorldUp;
		};
	}
}