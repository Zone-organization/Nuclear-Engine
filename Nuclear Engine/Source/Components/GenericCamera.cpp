#include "Components\GenericCamera.h"
#include "Math/gtc/matrix_inverse.hpp"
namespace NuclearEngine
{
	namespace Components
	{
		GenericCamera::GenericCamera(Math::Vector3 _position)
		{
			position = _position;
			API::ConstantBuffer::Create(&ConstantBuffer, "NE_Camera", sizeof(_CameraBuffer));
		}


		GenericCamera::~GenericCamera()
		{
			API::ConstantBuffer::Delete(&ConstantBuffer);
		}
	
		void GenericCamera::Initialize(Math::Matrix4 projectionMatrix)
		{
			_CameraBuffer.Projection = projectionMatrix;
			Update();
		}

		void GenericCamera::Update()
		{
			UpdateMatricesOnly();
			ConstantBuffer.Update(&_CameraBuffer, sizeof(_CameraBuffer));
		}
		void GenericCamera::UpdateMatricesOnly()
		{
			_CameraBuffer.ModelViewProjection = _CameraBuffer.Model * _CameraBuffer.View * _CameraBuffer.Projection;
			_CameraBuffer.ModelInvTranspose = Math::inverseTranspose(_CameraBuffer.Model);
		}
		void GenericCamera::SetModelMatrix(Math::Matrix4 modelMatrix)
		{
			_CameraBuffer.Model = modelMatrix;
			Update();
		}
		void GenericCamera::SetViewMatrix(Math::Matrix4 viewMatrix)
		{
			_CameraBuffer.View = viewMatrix;
			Update();

		}
		void GenericCamera::SetProjectionMatrix(Math::Matrix4 projectionMatrix)
		{
			_CameraBuffer.Projection = projectionMatrix;
			Update();

		}
		void GenericCamera::SetPosition(Math::Vector3 cameraposition)
		{
			this->position = cameraposition;
		}
		Math::Matrix4 GenericCamera::GetModelMatrix()
		{
			return _CameraBuffer.Model;
		}
		Math::Matrix4 GenericCamera::GetViewMatrix()
		{
			return this->_CameraBuffer.Model;
		}
		Math::Matrix4 GenericCamera::GetProjectionMatrix()
		{
			return _CameraBuffer.Projection;
		}
		Math::Vector3 GenericCamera::GetPosition()
		{
			return position;
		}
		API::ConstantBuffer GenericCamera::GetCBuffer()
		{
			return ConstantBuffer;
		}
	}
}