#include "Graphics\Camera.h"
#include "Math/glm/gtc/matrix_inverse.hpp"
#include <Core/Engine.h>
#include <Diligent/Graphics/GraphicsTools/interface/CommonlyUsedStates.h>
#include <Managers\ShaderManager.h>
#include <Platform\FileSystem.h>
#include <Diligent/Graphics/GraphicsTools/interface/MapHelper.hpp>
#include <Utilities/Logger.h>

namespace Nuclear
{
	using namespace Math;

	namespace Graphics
	{
		Camera::Camera(Math::Vector3 __position, Math::Vector3 _Worldup, float yaw, float pitch, float speed, float sensitivity, float _Zoom)
			: position(__position), Front(Math::Vector3(0.0f, 0.0f, -1.0f)), MovementSpeed(speed), MouseSensitivity(sensitivity), Yaw(yaw), Pitch(pitch), WorldUp(_Worldup), Zoom(_Zoom)
		{
		
		}

		Camera::~Camera()
		{
		}

		void Camera::Initialize(Math::Matrix4 projectionMatrix)
		{
			mCameraData.Projection = projectionMatrix;
			UpdateBuffer();
		}

		void Camera::UpdateBuffer()
		{
			Math::Vector3 front;
			front.x = cos(radians(Yaw)) * cos(radians(Pitch));
			front.y = sin(radians(Pitch));
			front.z = sin(radians(Yaw)) * cos(radians(Pitch));
			Front = normalize(front);
			// Also re-calculate the Right and Up vector
			Right = normalize(Math::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			Up = normalize(Math::cross(Right, Front));

			mCameraData.View = Math::lookAt(position, position + Front, Up);

			mCameraData.ModelViewProjection = mCameraData.Projection * mCameraData.View * mCameraData.Model;
			mCameraData.ModelInvTranspose = Math::inverseTranspose(mCameraData.Model);

		}


		void Camera::SetModelMatrix(Math::Matrix4 modelMatrix)
		{
			mCameraData.Model = modelMatrix;
			UpdateBuffer();
		}
		void Camera::SetViewMatrix(Math::Matrix4 viewMatrix)
		{
			mCameraData.View = viewMatrix;
			UpdateBuffer();

		}
		void Camera::SetProjectionMatrix(Math::Matrix4 projectionMatrix)
		{
			mCameraData.Projection = projectionMatrix;
			UpdateBuffer();

		}
		void Camera::SetPosition(Math::Vector3 cameraposition)
		{
			this->position = cameraposition;
		}
		Math::Matrix4 Camera::GetModelMatrix()
		{
			return mCameraData.Model;
		}
		Math::Matrix4 Camera::GetViewMatrix()
		{
			return this->mCameraData.Model;
		}
		Math::Matrix4 Camera::GetProjectionMatrix()
		{
			return mCameraData.Projection;
		}
		Math::Vector3 Camera::GetPosition()
		{
			return position;
		}


		void Camera::ProcessEye(float xoffset, float yoffset, bool constrainPitch)
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
		void Camera::ProcessMovement(CAMERA_MOVEMENT direction, float deltaTime)
		{
			float velocity = MovementSpeed * deltaTime;
			if (direction == CAMERA_MOVEMENT_FORWARD)
				position += Front * velocity;
			if (direction == CAMERA_MOVEMENT_BACKWARD)
				position -= Front * velocity;

#ifdef COORDINATE_SYSTEM_LEFT_HANDED
			if (direction == CAMERA_MOVEMENT_LEFT)
				position += Right * velocity;
			if (direction == CAMERA_MOVEMENT_RIGHT)
				position -= Right * velocity;
#else
			if (direction == CAMERA_MOVEMENT_LEFT)
				position -= Right * velocity;
			if (direction == CAMERA_MOVEMENT_RIGHT)
				position += Right * velocity;
#endif

		}
		void Camera::ProcessZoom(float yoffset)
		{
			if (Zoom >= 1.0f && Zoom <= 45.0f)
				Zoom -= yoffset;
			if (Zoom <= 1.0f)
				Zoom = 1.0f;
			if (Zoom >= 45.0f)
				Zoom = 45.0f;
		}
		Math::Vector3 Camera::GetFrontView()
		{
			return Front;
		}
	}
}