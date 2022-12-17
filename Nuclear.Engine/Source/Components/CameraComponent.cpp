#include "Components\CameraComponent.h"
#include "Math/glm/gtc/matrix_inverse.hpp"

namespace Nuclear
{
	using namespace Math;

	namespace Components
	{
		CameraComponent::CameraComponent()
			: CameraComponent(Math::perspective(Math::radians(45.0f), 800.f/600.f, 0.1f, 100.0f))
		{
		}
		CameraComponent::~CameraComponent()
		{
		}

		CameraComponent::CameraComponent(Math::Matrix4 projectionMatrix, float yaw, float pitch, float speed, float sensitivity, float _Zoom)
			: Front(Math::Vector3(0.0f, 0.0f, -1.0f)), MovementSpeed(speed), MouseSensitivity(sensitivity), Yaw(yaw), Pitch(pitch), WorldUp(Math::Vector3(0.0f, 1.0f, 0.0f)), Zoom(_Zoom)
		{
			mCameraData.Projection = projectionMatrix;
			UpdateBuffer();
		}

		void CameraComponent::UpdateBuffer()
		{
			Math::Vector3 front;
			front.x = cos(radians(Yaw)) * cos(radians(Pitch));
			front.y = sin(radians(Pitch));
			front.z = sin(radians(Yaw)) * cos(radians(Pitch));
			Front = normalize(front);
			// Also re-calculate the Right and Up vector
			Right = normalize(Math::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			Up = normalize(Math::cross(Right, Front));

			mCameraData.View = Math::lookAt(Position, Position + Front, Up);

			mCameraData.ModelViewProjection = mCameraData.Projection * mCameraData.View * mCameraData.Model;
			mCameraData.ModelInvTranspose = Math::inverseTranspose(mCameraData.Model);

		}

		void CameraComponent::SetModelMatrix(Math::Matrix4 modelMatrix)
		{
			mCameraData.Model = modelMatrix;
			UpdateBuffer();
		}
		void CameraComponent::SetViewMatrix(Math::Matrix4 viewMatrix)
		{
			mCameraData.View = viewMatrix;
			UpdateBuffer();

		}
		void CameraComponent::SetProjectionMatrix(Math::Matrix4 projectionMatrix)
		{
			mCameraData.Projection = projectionMatrix;
			UpdateBuffer();

		}

		Math::Matrix4 CameraComponent::GetModelMatrix()
		{
			return mCameraData.Model;
		}
		Math::Matrix4 CameraComponent::GetViewMatrix()
		{
			return this->mCameraData.Model;
		}
		Math::Matrix4 CameraComponent::GetProjectionMatrix()
		{
			return mCameraData.Projection;
		}

		void CameraComponent::SetPosition(Math::Vector3 cameraposition)
		{
			Position = cameraposition;
		}

		Math::Vector3 CameraComponent::GetPosition()
		{
			return Position;
		}
	
		Graphics::RenderTarget& CameraComponent::GetRenderTarget()
		{
			return mRT;
		}

		Graphics::RenderTarget& CameraComponent::GetDepthRenderTarget()
		{
			return mDepthRT;
		}


		void CameraComponent::ProcessEye(float xoffset, float yoffset, bool constrainPitch)
		{
			xoffset *= MouseSensitivity;
			yoffset *= MouseSensitivity;

			Pitch += yoffset;

#ifdef COORDINATE_SYSTEM_LEFT_HANDED
			Yaw -= xoffset;
#else
			Yaw += xoffset;
#endif

			// Make sure that when pitch is out of bounds, screen doesn't get flipped
			if (constrainPitch)
			{
				if (Pitch > 89.0f)
					Pitch = 89.0f;
				if (Pitch < -89.0f)
					Pitch = -89.0f;
			}
		}
		void CameraComponent::ProcessMovement(CAMERA_MOVEMENT direction, float deltaTime)
		{
			float velocity = MovementSpeed * deltaTime;
			if (direction == CAMERA_MOVEMENT_FORWARD)
				Position += Front * velocity;
			if (direction == CAMERA_MOVEMENT_BACKWARD)
				Position -= Front * velocity;

#ifdef COORDINATE_SYSTEM_LEFT_HANDED
			if (direction == CAMERA_MOVEMENT_LEFT)
				Position += Right * velocity;
			if (direction == CAMERA_MOVEMENT_RIGHT)
				Position -= Right * velocity;
#else
			if (direction == CAMERA_MOVEMENT_LEFT)
				Position -= Right * velocity;
			if (direction == CAMERA_MOVEMENT_RIGHT)
				Position += Right * velocity;
#endif

		}
		void CameraComponent::ProcessZoom(float yoffset)
		{
			if (Zoom >= 1.0f && Zoom <= 45.0f)
				Zoom -= yoffset;
			if (Zoom <= 1.0f)
				Zoom = 1.0f;
			if (Zoom >= 45.0f)
				Zoom = 45.0f;
		}
		Math::Vector3 CameraComponent::GetFrontView()
		{
			return Front;
		}
	}
}