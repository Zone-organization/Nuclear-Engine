#pragma once
#include "Common.h"
#include <fstream>
#include <sstream>

API::Texture *block_solid;
API::Texture *block;

// GameLevel holds all Tiles as part of a Breakout level and 
// hosts functionality to Load/render levels from the harddisk.
class GameLevel
{
public:
	// Level state
	std::vector<Components::Sprite> Bricks;
	// Constructor
	GameLevel() { }
	// Loads level from file
	void      Load(const char *file, uint levelWidth, uint levelHeight);
	// Render level
	void      Draw(Renderers::SpriteRenderer *Renderer);
	// Check if the level is completed (all non-solid tiles are destroyed)
	bool IsCompleted();
private:
	// Initialize level from tile data
	void      init(std::vector<std::vector<uint>> tileData, uint levelWidth,uint levelHeight);
};

class BreakOut : public Core::Game
{
protected:
	Core::GameInfo Info;
	API::Shader *RendererShader;
	Renderers::SpriteRenderer *Renderer;
	Components::Sprite* sprite;
	GameLevel lvl;
	Components::FlyCamera *Camera;
public:
	BreakOut()
	{
		Core::GameInfo Info;
		Info.Name = "Breakout";
		Info.Developer = "Zone Organization";
		Info.Version = "0.001";

		this->gameinfo = &Info;
	}
	void Load()
	{
		Camera = new Components::FlyCamera();
		Camera->Initialize(Math::Orthographic(0.0f, static_cast<float>(_Width_),static_cast<float>(_Height_), 0.0f, -1.0f, 1.0f));


		RendererShader = new API::Shader("SpriteRenderer",
			&API::CompileShader(Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/SpriteRenderer.vs.hlsl").c_str(), ShaderType::Vertex, ShaderLanguage::HLSL),
			&API::CompileShader(Core::FileSystem::LoadFileToString("Assets/NuclearEngine/Shaders/Renderer/SpriteRenderer.ps.hlsl").c_str(), ShaderType::Pixel, ShaderLanguage::HLSL));
		
		Renderer = new Renderers::SpriteRenderer(RendererShader,Camera);


		Texture_Desc TexDesc;
		TexDesc.Filter = TextureFilter::Linear2D;
		TexDesc.Wrap = TextureWrap::Repeat;
		TexDesc.Format = TextureFormat::R8G8B8A8;
		TexDesc.Type = TextureType::Texture2D;
		block_solid = new API::Texture(ResourceManager::LoadTextureFromFile("Assets/Breakout/Textures/block_solid.png", TexDesc), TexDesc);
		block = new API::Texture(ResourceManager::LoadTextureFromFile("Assets/Breakout/Textures/block.png", TexDesc), TexDesc);

		lvl.Load("Assets/Breakout/Levels/lvl1.lvl",_Width_,_Height_ * 0.5);
		sprite = new Components::Sprite(block_solid, Math::Vector2(200.0f, 200.0f), (300, 400),  API::Color(0.0f, 1.0f, 0.0f, 1.0f));
		Core::Context::SetPrimitiveType(PrimitiveType::TriangleList);
	}

	void PreRender(float deltatime) override
	{
	/*	if (Input::Keyboard::IsKeyPressed(Input::Keyboard::Key::W))
			Camera->ProcessMovement(Components::Camera_Movement::FORWARD, deltatime);
		if (Input::Keyboard::IsKeyPressed(Input::Keyboard::Key::A))
			Camera->ProcessMovement(Components::Camera_Movement::LEFT, deltatime);
		if (Input::Keyboard::IsKeyPressed(Input::Keyboard::Key::S))
			Camera->ProcessMovement(Components::Camera_Movement::BACKWARD, deltatime);
		if (Input::Keyboard::IsKeyPressed(Input::Keyboard::Key::D))
			Camera->ProcessMovement(Components::Camera_Movement::RIGHT, deltatime);
*/
		Camera->Update();
	}

	void Render()
	{
		Core::Context::Begin();

		//Change Background Color to Blue in RGBA format
		Core::Context::ClearColor(API::Color(0.1f, 0.1f, 0.1f, 1.0f));
		//Renderer->DrawSprite(sprite);
		//Renderer->DrawSprite(block_solid, Math::Vector2(200.0f, 200.0f), (300, 400) ,0.0f,API::Color(0.0f,1.0f,0.0f,1.0f));
		lvl.Draw(Renderer);
		Core::Context::End();
	}
	void ExitRendering()	// Exit Rendering
	{		
		delete Camera;
	}
};


void GameLevel::Load(const char * file, uint levelWidth, uint levelHeight)
{ // Clear old data
	this->Bricks.clear();
	// Load from file
	uint tileCode;
	GameLevel level;
	std::string line;
	std::ifstream fstream(file);
	std::vector<std::vector<uint>> tileData;
	if (fstream)
	{
		while (std::getline(fstream, line)) // Read each line from level file
		{
			std::istringstream sstream(line);
			std::vector<uint> row;
			while (sstream >> tileCode) // Read each word seperated by spaces
				row.push_back(tileCode);
			tileData.push_back(row);
		}
		if (tileData.size() > 0)
			this->init(tileData, levelWidth, levelHeight);
	}
}

void GameLevel::Draw(Renderers::SpriteRenderer * Renderer)
{
	for (Components::Sprite &tile : this->Bricks)
		if (!tile.Destroyed)
			Renderer->DrawSprite(&tile);
}

bool GameLevel::IsCompleted()
{
	for (Components::Sprite &tile : this->Bricks)
		if (!tile.IsSolid && !tile.Destroyed)
			return false;
	return true;
}

void GameLevel::init(std::vector<std::vector<uint>> tileData, uint levelWidth, uint levelHeight)
{  // Calculate dimensions
	uint height = tileData.size();
	uint width = tileData[0].size(); // Note we can index vector at [0] since this function is only called if height > 0
	float unit_width = levelWidth / static_cast<float>(width), unit_height = levelHeight / height;
	// Initialize level tiles based on tileData		
	for (uint y = 0; y < height; ++y)
	{
		for (uint x = 0; x < width; ++x)
		{
			// Check block type from level data (2D level array)
			if (tileData[y][x] == 1) // Solid
			{
				Math::Vector2 pos(unit_width * x, unit_height * y);
				Math::Vector2 size(unit_width, unit_height);
				Components::Sprite obj(block_solid, pos, size, API::Color(0.8f, 0.8f, 0.7f));
				obj.IsSolid = true;
				this->Bricks.push_back(obj);
			}
			else if (tileData[y][x] > 1)	// Non-solid; now determine its color based on level data
			{
				API::Color color = API::Color(1.0f); // original: white
				if (tileData[y][x] == 2)
					color = API::Color(0.2f, 0.6f, 1.0f);
				else if (tileData[y][x] == 3)
					color = API::Color(0.0f, 0.7f, 0.0f);
				else if (tileData[y][x] == 4)
					color = API::Color(0.8f, 0.8f, 0.4f);
				else if (tileData[y][x] == 5)
					color = API::Color(1.0f, 0.5f, 0.0f);

				Math::Vector2 pos(unit_width * x, unit_height * y);
				Math::Vector2 size(unit_width, unit_height);
				this->Bricks.push_back(Components::Sprite(block, pos, size, color));
			}
		}
	}

}
