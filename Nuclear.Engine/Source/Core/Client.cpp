#include <Core/Client.h>
#include <Core/Scene.h>
#include <Assets/AssetManager.h>
namespace Nuclear {
	namespace Core {
		Client::Client(const ClientInfo& yourinfo)
		{
			minfo = yourinfo;
		}
		Client::~Client()
		{
		}
		ClientInfo Client::GetClientInfo()
		{
			return minfo;
		}
		void Client::Initialize()
		{
		}

		void Client::Load()
		{
		}
		void Client::Update(float Deltatime)
		{
		}

		void Client::Render(float Deltatime)
		{
		}
		void Client::ExitRendering()
		{
		}
		void Client::Shutdown()
		{
		}
		void Client::LoadFinishUp()
		{
		}
		Core::Scene& Client::GetScene()
		{
			return Core::Scene::GetInstance();
		}
		Assets::AssetManager& Client::GetAssetManager()
		{
			return Assets::AssetManager::GetInstance();
		}
		void Client::OnMouseScroll(double val1, double val2)
		{
		}
		void Client::OnMouseMovement(int xpos, int ypos)
		{
		}

		void Client::OnWindowResize(int width, int height)
		{
		}
	}
}