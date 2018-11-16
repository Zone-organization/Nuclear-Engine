#include "Engine/Components\CameraComponent.h"
#include "Base/Math/gtc/matrix_inverse.hpp"
#include <Core\Engine.h>
#include <Engine/ECS\Scene.h>

namespace NuclearEngine
{
	using namespace Math;

	namespace Components
	{
		CameraComponent::CameraComponent()
			: CameraComponent(Math::Vector3(0.0f, 0.0f, 0.0f))
		{
		}
		CameraComponent::CameraComponent(Math::Vector3 __position, Math::Vector3 _Worldup, float yaw, float pitch, float speed, float sensitivity, float _Zoom)
			: Front(Math::Vector3(0.0f, 0.0f, -1.0f)), MovementSpeed(speed), MouseSensitivity(sensitivity), Yaw(yaw), Pitch(pitch), WorldUp(_Worldup), Zoom(_Zoom)
		{

			position = __position;
			Graphics::API::ConstantBuffer::Create(&ConstantBuffer, "NE_Camera", sizeof(_CameraBuffer));
		}
	
		CameraComponent::~CameraComponent()
		{
			Graphics::API::ConstantBuffer::Delete(&ConstantBuffer);			
		}
	
		void CameraComponent::Initialize(Math::Matrix4 projectionMatrix)
		{
			_CameraBuffer.Projection = projectionMatrix;
			Update();
		}

		void CameraComponent::Update()
		{
			Vector3 front;
			front.x = cos(radians(Yaw)) * cos(radians(Pitch));
			front.y = sin(radians(Pitch));
			front.z = sin(radians(Yaw)) * cos(radians(Pitch));
			Front = normalize(front);
			// Also re-calculate the Right and Up vector
			Right = normalize(Math::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
			Up = normalize(Math::cross(Right, Front));

			_CameraBuffer.View = Math::lookAt(position, position + Front, Up);

			UpdateMatricesOnly();
			ConstantBuffer.Update(&_CameraBuffer, sizeof(_CameraBuffer));
		}
		void CameraComponent::UpdateMatricesOnly()
		{
			_CameraBuffer.ModelViewProjection = _CameraBuffer.Projection * _CameraBuffer.View * _CameraBuffer.Model;
			_CameraBuffer.ModelInvTranspose = Math::inverseTranspose(_CameraBuffer.Model);
		}
		void CameraComponent::SetModelMatrix(Math::Matrix4 modelMatrix)
		{
			_CameraBuffer.Model = modelMatrix;
			Update();
		}
		void CameraComponent::SetViewMatrix(Math::Matrix4 viewMatrix)
		{
			_CameraBuffer.View = viewMatrix;
			Update();

		}
		void CameraComponent::SetProjectionMatrix(Math::Matrix4 projectionMatrix)
		{
			_CameraBuffer.Projection = projectionMatrix;
			Update();

		}
		void CameraComponent::SetPosition(Math::Vector3 cameraposition)
		{
			this->position = cameraposition;
		}
		void CameraComponent::SetActive()
		{
			//Ugly hack prone to errors dont look
			Core::Engine::GetGame()->GetActiveScene()->SetActiveCamera(this);
		}
		Math::Matrix4 CameraComponent::GetModelMatrix()
		{
			return _CameraBuffer.Model;
		}
		Math::Matrix4 CameraComponent::GetViewMatrix()
		{
			return this->_CameraBuffer.Model;
		}
		Math::Matrix4 CameraComponent::GetProjectionMatrix()
		{
			return _CameraBuffer.Projection;
		}
		Math::Vector3 CameraComponent::GetPosition()
		{
			return position;
		}
		Graphics::API::ConstantBuffer CameraComponent::GetCBuffer()
		{
			return ConstantBuffer;
		}

		void CameraComponent::ProcessEye(float xoffset, float yoffset, bool constrainPitch)
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
		void CameraComponent::ProcessMovement(Camera_Movement direction, float deltaTime)
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