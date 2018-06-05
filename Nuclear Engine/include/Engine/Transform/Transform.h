#pragma once
#include <Base\NE_Common.h>
#include <Base\Math\Math.h>

namespace NuclearEngine
{
	// an entity transform
	class NEAPI Transform
	{
	public:
		Transform();
		~Transform();

		//Get the transform
		Math::Matrix4 GetTransformMatrix();

		void SetPosition(Math::Vector3 position);
		void SetRotation(Math::Quaternion rotation);
		void SetScale(Math::Vector3 scale);
		void SetScale(float scale);
		Math::Vector3 GetLocalPosition();
		Math::Quaternion GetLocalRotation();
		Math::Vector3 GetLocalScale();

		Math::Vector3 GetWorldPosition();
		Math::Quaternion GetWorldRotation();
		Math::Vector3 GetWorldScale();

		void SetTransform(Math::Matrix4 _Transform);

		void Update();

		void Update(Math::Matrix4 parent);

	private:
		Math::Matrix4 mTransform = Math::Matrix4(1.0f);
	
		Math::Vector3 mPosition = Math::Vector3(0.0f);
		Math::Quaternion mRotation = Math::Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
		Math::Vector3 mScale = Math::Vector3(.0f);

		Math::Vector3 mWorldPosition = Math::Vector3(0.0f);
		Math::Quaternion mWorldRotation = Math::Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
		Math::Vector3 mWorldScale = Math::Vector3(0.0f);


		bool mDirty;
	};
}