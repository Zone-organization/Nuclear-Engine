#pragma once
#include <NE_Common.h>
#include <Math\Math.h>

namespace NuclearEngine
{
	// an entity transform
	class NEAPI Transform
	{
	public:
		Transform();
		~Transform();

		//Get the transform
		Math::Matrix4 GetTransform();

		void SetPosition(Math::Vector3 position);
		void SetRotation(Math::Vector4 rotation);
		void SetScale(Math::Vector3 scale);
		void SetScale(float scale);
		Math::Vector3 GetLocalPosition();
		Math::Vector4 GetLocalRotation();
		Math::Vector3 GetLocalScale();
		Math::Vector3 GetWorldPosition();
		Math::Vector3 GetWorldScale();

		void Update();

		void Update(Math::Matrix4 parent);

	private:
		Math::Matrix4 m_Transform;
		Math::Vector3 m_Position = Math::Vector3(0.0f);
		Math::Vector4 m_Rotation = Math::Vector4(0.0f, 0.0f, 0.0f, 1.0f); // axis-angle for now; test w/ quaternions soon!
		Math::Vector3 m_Scale = Math::Vector3(1.0f);
		bool m_dirty;
	};
}