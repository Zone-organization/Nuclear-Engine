#include "..\..\include\Components\Transform.h"
namespace NuclearEngine
{
	namespace Components
	{
		Transform::Transform()
		{
		}
		Transform::~Transform()
		{
		}

		void Transform::SetPosition(Math::Vector3 position)
		{
			m_Position = position;
			m_dirty = true;
		}

		void Transform::SetRotation(Math::Vector4 rotation)
		{
			m_Rotation = rotation;
			m_dirty = true;
		}

		void Transform::SetScale(Math::Vector3 scale)
		{
			m_Scale = scale;
			m_dirty = true;
		}

		void Transform::SetScale(float scale)
		{
			m_Scale = Math::Vector3(scale);
			m_dirty = true;
		}

		Math::Vector3 Transform::GetLocalPosition()
		{
			return m_Position;
		}

		Math::Vector4 Transform::GetLocalRotation()
		{
			return m_Rotation;
		}

		Math::Vector3 Transform::GetLocalScale()
		{
			return m_Scale;
		}

		Math::Matrix4 Transform::GetTransform()
		{
			return m_Transform;
		}
		Math::Vector3 Transform::GetWorldPosition()
		{
			Math::Matrix4 transform = GetTransform();
			Math::Vector4 pos = transform * Math::Vector4(m_Position, 1.0f);
			return pos.xyz;
		}

		Math::Vector3 Transform::GetWorldScale()
		{
			Math::Matrix4 transform = GetTransform();
			Math::Vector3 scale = Math::Vector3(transform[0][0], transform[1][1], transform[2][2]);
			if (scale.x < 0.0f) scale.x *= -1.0f;
			if (scale.y < 0.0f) scale.y *= -1.0f;
			if (scale.z < 0.0f) scale.z *= -1.0f;
			return scale;
		}
		void Transform::Update()
		{
			// we only do this if the node itself or its parent is flagged as dirty
			if (m_dirty)
			{
				// first scale, then rotate, then translation
				m_Transform = Math::Translate(m_Position);
				m_Transform = Math::Scale(m_Transform, m_Scale);
				//m_Transform = Math::Rotate(m_Transform, m_Rotation.xyz, m_Rotation.w);

				m_dirty = false;
			}
		}
		void Transform::Update(Math::Matrix4 parent)
		{
			//Todo: find a better way to update the transform since we double check if dirty!
			Update();
			if (m_dirty)
			{
				m_Transform = parent * m_Transform;
			}
		}
	}
}