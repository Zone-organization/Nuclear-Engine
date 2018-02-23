#include <Components\GameObject.h>
namespace NuclearEngine
{
	namespace Components
	{
		inline Components::Model * GameObject::GetModel()
		{
			return m_model;
		}
		inline void GameObject::SetTransform(Transform transform)
		{
			m_transform = transform;
		}
		inline Transform * GameObject::GetTransformComponent()
		{
			return &m_transform;
		}
		inline void GameObject::SetModel(Components::Model * model)
		{
			m_model = model;
		}
	}
}