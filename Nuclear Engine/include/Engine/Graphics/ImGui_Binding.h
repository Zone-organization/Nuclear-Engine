#pragma once
namespace sf
{
	class Event;
	class RenderTarget;
	class RenderWindow;
	class Sprite;
	class Texture;
	class Window;
}

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace ImGui_Binding
		{
			void Init(sf::Window& window);
			void ProcessEvent(const sf::Event& event);
		}
	}
}
