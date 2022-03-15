#include "Engine\ECS/Transform.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <Base/Math/gtx/quaternion.hpp>
namespace NuclearEngine
{
	namespace ECS
	{
		Transform::Transform()
		{
			mTransformMatrix = Math::Matrix4(1.0f);
			mPosition = Math::Vector3(0.0f);
			mRotation = Math::Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
			mScale = Math::Vector3(0.0f);
			mDirty = true;
			mWorldPosition = Math::Vector3(0.0f);
			mWorldRotation = Math::Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
			mWorldScale = Math::Vector3(0.0f);
		}

		Transform::Transform(Math::Matrix4 Transform)
		{
			mDirty = true;
			mTransformMatrix = Transform;
		}

		Transform::Transform(Math::Vector3 position, Math::Quaternion rotation)
		{
			mTransformMatrix = Math::Matrix4(1.0f);
			mPosition = position;
			mRotation = rotation;
			mScale = Math::Vector3(0.0f);
			mDirty = true;
			mWorldPosition = Math::Vector3(0.0f);
			mWorldRotation = Math::Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
			mWorldScale = Math::Vector3(0.0f);
		}

		Transform::~Transform()
		{
		}

		void Transform::SetPosition(Math::Vector3 position)
		{
			mPosition = position;
			mDirty = true;
		}

		void Transform::SetRotation(Math::Quaternion rotation)
		{
			mRotation = rotation;
			mDirty = true;
		}

		void Transform::SetScale(Math::Vector3 scale)
		{
			mScale = scale;
			mDirty = true;
		}

		void Transform::SetScale(float scale)
		{
			mScale = Math::Vector3(scale);
			mDirty = true;
		}

		Math::Vector3 Transform::GetLocalPosition()
		{
			return mPosition;
		}

		Math::Quaternion Transform::GetLocalRotation()
		{
			return mRotation;
		}

		Math::Vector3 Transform::GetLocalScale()
		{
			return mScale;
		}

		Math::Matrix4 Transform::GetTransform()
		{
			return mTransformMatrix;
		}
		Math::Vector3 Transform::GetWorldPosition()
		{
			Math::Matrix4 transform = GetTransform();
			Math::Vector4 pos = transform * Math::Vector4(mPosition, 1.0f);
			mWorldPosition = Math::Vector3(pos.x, pos.y, pos.z);
			return mWorldPosition;
		}

		Math::Quaternion Transform::GetWorldRotation()
		{
			return mWorldRotation;
		}

		Math::Vector3 Transform::GetWorldScale()
		{
			Math::Matrix4 transform = GetTransform();
			Math::Vector3 scale = Math::Vector3(transform[0][0], transform[1][1], transform[2][2]);
			if (scale.x < 0.0f) scale.x *= -1.0f;
			if (scale.y < 0.0f) scale.y *= -1.0f;
			if (scale.z < 0.0f) scale.z *= -1.0f;
			mWorldScale = scale;
			return mWorldScale;
		}
		void Transform::SetTransform(Math::Matrix4 _Transform)
		{
			mTransformMatrix = _Transform;
		}
		void Transform::Update()
		{
			if (mDirty)
			{
				mTransformMatrix = Math::translate(mTransformMatrix, mPosition);
				mTransformMatrix = Math::scale(mTransformMatrix, mScale);
				mTransformMatrix *= Math::toMat4(mRotation);
				mWorldPosition = GetWorldPosition();
				mWorldScale = GetWorldScale();
				mDirty = false;
			}
		}
		float* Transform::GetWorldPositionPtr()
		{
			return (float*)&mWorldPosition;
		}
		/*void Transform::Update(Math::Matrix4 parent)
		{
			Update();
			if (mDirty)
			{
				mTransformMatrix = parent * mTransformMatrix;
			}
		}*/
	}
}