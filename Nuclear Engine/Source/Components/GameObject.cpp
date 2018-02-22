#include <Components\GameObject.h>
namespace NuclearEngine
{
	namespace Components
	{
		void GameObject::SetModel(Components::Model * model)
		{
			m_model = model;
		}
		Components::Model * GameObject::GetModel()
		{
			return m_model;
		}
		Transform * GameObject::GetTransform()
		{
			return &m_transform;
		}
	}
}