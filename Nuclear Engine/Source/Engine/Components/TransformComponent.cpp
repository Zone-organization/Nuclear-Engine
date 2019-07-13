#include "Engine\Components/TransformComponent.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <Base/Math/gtx/quaternion.hpp>
namespace NuclearEngine
{
	namespace Components
	{
		TransformComponent::TransformComponent()
		{
			mTransform = Math::Matrix4(1.0f);
			mPosition = Math::Vector3(0.0f);
			mRotation = Math::Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
			mScale = Math::Vector3(0.0f);

			mWorldPosition = Math::Vector3(0.0f);
			mWorldRotation = Math::Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
			mWorldScale = Math::Vector3(0.0f);
		}

		TransformComponent::TransformComponent(Math::Matrix4 Transform)
		{
			mTransform = Transform;
		}

		TransformComponent::TransformComponent(Math::Vector3 position, Math::Quaternion rotation)
		{
			mTransform = Math::Matrix4(1.0f);
			mPosition = position;
			mRotation = rotation;
			mScale = Math::Vector3(0.0f);

			mWorldPosition = Math::Vector3(0.0f);
			mWorldRotation = Math::Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
			mWorldScale = Math::Vector3(0.0f);
		}

		TransformComponent::~TransformComponent()
		{
		}

		void TransformComponent::SetPosition(Math::Vector3 position)
		{
			mPosition = position;
			mDirty = true;
		}

		void TransformComponent::SetRotation(Math::Quaternion rotation)
		{
			mRotation = rotation;
			mDirty = true;
		}

		void TransformComponent::SetScale(Math::Vector3 scale)
		{
			mScale = scale;
			mDirty = true;
		}

		void TransformComponent::SetScale(float scale)
		{
			mScale = Math::Vector3(scale);
			mDirty = true;
		}

		Math::Vector3 TransformComponent::GetLocalPosition()
		{
			return mPosition;
		}

		Math::Quaternion TransformComponent::GetLocalRotation()
		{
			return mRotation;
		}

		Math::Vector3 TransformComponent::GetLocalScale()
		{
			return mScale;
		}

		Math::Matrix4 TransformComponent::GetTransformMatrix()
		{
			return mTransform;
		}
		Math::Vector3 TransformComponent::GetWorldPosition()
		{
			Math::Matrix4 transform = GetTransformMatrix();
			Math::Vector4 pos = transform * Math::Vector4(mPosition, 1.0f);
			return Math::Vector3(pos.x, pos.y, pos.z);
		}

		Math::Quaternion TransformComponent::GetWorldRotation()
		{
			return mWorldRotation;
		}

		Math::Vector3 TransformComponent::GetWorldScale()
		{
			Math::Matrix4 transform = GetTransformMatrix();
			Math::Vector3 scale = Math::Vector3(transform[0][0], transform[1][1], transform[2][2]);
			if (scale.x < 0.0f) scale.x *= -1.0f;
			if (scale.y < 0.0f) scale.y *= -1.0f;
			if (scale.z < 0.0f) scale.z *= -1.0f;
			return scale;
		}
		void TransformComponent::SetTransform(Math::Matrix4 _Transform)
		{
			mTransform = _Transform;
		}
		void TransformComponent::Update()
		{
			if (mDirty)
			{
				mTransform = Math::translate(mTransform, mPosition);
				mTransform = Math::scale(mTransform, mScale);
				mTransform *= Math::toMat4(mRotation);
				mDirty = false;
			}
		}
		void TransformComponent::Update(Math::Matrix4 parent)
		{
			Update();
			if (mDirty)
			{
				mTransform = parent * mTransform;
			}
		}
	}
}