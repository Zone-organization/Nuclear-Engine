#include "Transform/Transform.h"
#define GLM_ENABLE_EXPERIMENTAL
#include <Math/gtx/quaternion.hpp>
namespace NuclearEngine
{
	Transform::Transform()
	{
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

	Math::Matrix4 Transform::GetTransformMatrix()
	{
		return mTransform;
	}
	Math::Vector3 Transform::GetWorldPosition()
	{
		Math::Matrix4 transform = GetTransformMatrix();
		Math::Vector4 pos = transform * Math::Vector4(mPosition, 1.0f);
		return Math::Vector3(pos.x, pos.y, pos.z);
	}

	Math::Quaternion Transform::GetWorldRotation()
	{
		return mWorldRotation;
	}

	Math::Vector3 Transform::GetWorldScale()
	{
		Math::Matrix4 transform = GetTransformMatrix();
		Math::Vector3 scale = Math::Vector3(transform[0][0], transform[1][1], transform[2][2]);
		if (scale.x < 0.0f) scale.x *= -1.0f;
		if (scale.y < 0.0f) scale.y *= -1.0f;
		if (scale.z < 0.0f) scale.z *= -1.0f;
		return scale;
	}
	void Transform::Update()
	{
		if (mDirty)
		{
			// first scale, then rotate, then translation
			mTransform = Math::Translate(mTransform, mPosition);
			mTransform = Math::Scale(mTransform, mScale);
			mTransform *= Math::toMat4(mRotation);
			mDirty = false;
		}
	}
	void Transform::Update(Math::Matrix4 parent)
	{
		//Todo: find a better way to update the transform since we double check if dirty!
		Update();
		if (mDirty)
		{
			mTransform = parent * mTransform;
		}
	}
}