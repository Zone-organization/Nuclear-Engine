#include <Core/Game.h>
#include <Core/Application.h>

namespace NuclearEngine {
	namespace Core {
		Game::Game()
		{
			mGameinfo = nullptr;
		}
		Game::Game(GameInfo * yourinfo)
		{
			mGameinfo = yourinfo;
		}
		Game::~Game()
		{
		}
		GameInfo * Game::GetGameInfo()
		{
			return mGameinfo;
		}
		void Game::Initialize()
		{
		}
		
		void Game::Load()
		{
		}
		void Game::Update(float Deltatime)
		{
		}

		void Game::Render(float Deltatime)
		{
		}
		void Game::ExitRendering()
		{
		}
		void Game::Shutdown()
		{
		}
		void Game::LoadFinishUp()
		{
		}
		void Game::OnMouseScroll(double val1, double val2)
		{
		}
		void Game::OnMouseMovement(int xpos, int ypos)
		{
		}

		void Game::OnWindowResize(int width, int height)
		{
		}

		void Game::SetActiveScene(ECS::Scene * scene)
		{
			mScene = scene;
		}

		ECS::Scene * Game::GetActiveScene()
		{
			return mScene;
		}

		void Game::SetDefaultAssetManager(Managers::AssetManager* assetmanager)
		{
			mAssetManager = assetmanager;
		}

		Managers::AssetManager* Game::GetDefaultAssetManager()
		{
			return mAssetManager;
		}
	
	}
}