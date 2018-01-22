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
		void Game::StartupLoad()
		{
		}
		void Game::Load()
		{
		}
		void Game::PreRender(float tick_deltatime)
		{
		}
		void Game::PostRender()
		{
		}
		void Game::Render()
		{
		}
		void Game::ExitRendering()
		{
		}
		void Game::Shutdown()
		{
		}
		void Game::OnMouseMovement(double xpos, double ypos)
		{
		}

		void Game::OnWindowResize(int width, int height)
		{
		}
	
	}
}