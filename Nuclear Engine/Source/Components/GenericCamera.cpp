#include "Components\GenericCamera.h"


namespace NuclearEngine
{
	namespace Components
	{
		GenericCamera::GenericCamera(Math::Vector3 _position)
		{
			position = _position;
		}


		GenericCamera::~GenericCamera()
		{
		}
	
		void GenericCamera::Initialize(Math::Matrix4 projectionMatrix)
		{
			UniformBuffer = new API::UniformBuffer("NE_Camera", sizeof(_CameraBuffer));

			_CameraBuffer.projectionMatrix = projectionMatrix;
		}

		void GenericCamera::Update()
		{
			UniformBuffer->Update(&_CameraBuffer, sizeof(_CameraBuffer), 0, ShaderType::Vertex);

		}
		void GenericCamera::SetModelMatrix(Math::Matrix4 modelMatrix)
		{
			_CameraBuffer.modelMatrix = modelMatrix;
			Update();
		}
		void GenericCamera::SetViewMatrix(Math::Matrix4 viewMatrix)
		{
			_CameraBuffer.viewMatrix = viewMatrix;
			Update();

		}
		void GenericCamera::SetProjectionMatrix(Math::Matrix4 projectionMatrix)
		{
			_CameraBuffer.projectionMatrix = projectionMatrix;
			Update();

		}
		void GenericCamera::SetPosition(Math::Vector3 cameraposition)
		{
			this->position = cameraposition;
		}
		Math::Matrix4 GenericCamera::GetModelMatrix()
		{
			return _CameraBuffer.modelMatrix;
		}
		Math::Matrix4 GenericCamera::GetViewMatrix()
		{
			return this->_CameraBuffer.viewMatrix;
		}
		Math::Matrix4 GenericCamera::GetProjectionMatrix()
		{
			return _CameraBuffer.projectionMatrix;
		}
		Math::Vector3 GenericCamera::GetPosition()
		{
			return position;
		}
		API::UniformBuffer* GenericCamera::GetCBuffer()
		{
			return UniformBuffer;
		}
	}
}