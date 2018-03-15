#include "Components\FlyCamera.h"
#include "Core\Engine.h"

namespace NuclearEngine
{
	using namespace Math;

	namespace Components
	{
		FlyCamera::FlyCamera(Math::Vector3 __position, Math::Vector3 _Worldup, float yaw, float pitch, float speed, float sensitivity, float _Zoom)
			: Front(Math::Vector3(0.0f, 0.0f, -1.0f)), MovementSpeed(speed), MouseSensitivity(sensitivity), Yaw(yaw), Pitch(pitch), WorldUp(_Worldup), Zoom(_Zoom)
		{
			
			GenericCamera::GenericCamera(__position);
		}
		FlyCamera::~FlyCamera()
		{
		}
		void FlyCamera::ProcessEye(float xoffset, float yoffset, bool constrainPitch)
		{
			xoffset *= MouseSensitivity;
			yoffset *= MouseSensitivity;

			Yaw += xoffset;
			Pitch += yoffset;

			// Make sure that when pitch is out of bounds, screen doesn't get flipped
			if (constrainPitch)
			{
				if (Pitch > 89.0f)
					Pitch = 89.0f;
				if (Pitch < -89.0f)
					Pitch = -89.0f;
			}
		}
		void FlyCamera::ProcessMovement(Camera_Movement direction, float deltaTime)
		{
			float velocity = MovementSpeed * deltaTime;
			if (direction == FORWARD)
				position += Front * velocity;
			if (direction == BACKWARD)
				position -= Front * velocity;
			if (direction == LEFT)
				position -= Right * velocity;
			if (direction == RIGHT)
				position += Right * velocity;
		}
		void FlyCamera::ProcessZoom(float yoffset)
		{
			if (Zoom >= 1.0f && Zoom <= 45.0f)
				Zoom -= yoffset;
			if (Zoom <= 1.0f)
				Zoom = 1.0f;
			if (Zoom >= 45.0f)
				Zoom = 45.0f;
		}
		Math::Vector3 FlyCamera::GetFrontView()
		{
			return Front;
		}

		void FlyCamera::Update()
		{
			Vector3 front;
			front.x = cos(radians(Yaw)) * cos(radians(Pitch));
			front.y = sin(radians(Pitch));
			front.z = sin(radians(Yaw)) * cos(radians(Pitch));
			Front = Normalize(front);
			// Also re-calculate the Right and Up vector
			Right = Normalize(Math::Cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			Up = Normalize(Math::Cross(Right, Front));

			_CameraBuffer.View = Math::LookAt(position, position + Front, Up);
			GenericCamera::Update();
		}
	}
}