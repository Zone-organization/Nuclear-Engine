#include "..\..\include\Scene\Scene.h"
namespace NuclearEngine
{
	void Scene::Clear()
	{
	}
	SceneNode * Scene::GetRoot()
	{
		return Root;
	}
	void Scene::Update(float dt)
	{
		Root->Update(dt);
	}
	void Scene::Render()
	{
		Root->Render();
	}
}