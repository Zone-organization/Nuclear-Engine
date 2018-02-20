#include "..\..\include\Scene\GameObject.h"

namespace NuclearEngine
{
	GameObject::GameObject(Components::Model * model)
	{
		m_model = model;
	}
	void GameObject::SetModel(Components::Model * model)
	{
		m_model = model;
	}
	void GameObject::Render()
	{
		m_model->Draw();
	}
}