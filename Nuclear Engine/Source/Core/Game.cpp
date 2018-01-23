#include "..\..\include\Core\Game.h"


namespace NuclearEngine {
	namespace Core {
		Game::Game()
		{
			gameinfo = nullptr;
		}
		Game::Game(GameInfo * yourinfo)
		{
			gameinfo = yourinfo;
		}
		Game::~Game()
		{
		}
		GameInfo * Game::GetInfo()
		{
			return gameinfo;
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
		void Game::OnMouseMovement(float xpos, float ypos)
		{
		}

		void Game::OnWindowResize(int width, int height)
		{
		}
	
	}
}