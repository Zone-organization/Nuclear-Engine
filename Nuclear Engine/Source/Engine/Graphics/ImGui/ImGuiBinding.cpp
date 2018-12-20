#include "Engine\Graphics\ImGui.h"
#include "Engine\Graphics\ImGuiBinding.h"
#include <LLGL\Display.h>
#include <LLGL\Input.h>

namespace NuclearEngine
{
	namespace Graphics
	{
		static bool gWindowHasFocus = false;
		static bool gMousePressed[3] = { false, false, false };
		static bool gMouseMoved = false;
		static std::shared_ptr<LLGL::Input> gInput;

		void ImGuiBinding::InitializeWindow(LLGL::Window* window)
		{
			if (window == nullptr)
			{
				return;
			}
			ImGui::CreateContext();
			ImGuiIO& io = ImGui::GetIO();

			io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

			// init keyboard mapping
			io.KeyMap[ImGuiKey_Tab] = static_cast<int>(LLGL::Key::Tab);
			io.KeyMap[ImGuiKey_LeftArrow] = static_cast<int>(LLGL::Key::Left);
			io.KeyMap[ImGuiKey_RightArrow] = static_cast<int>(LLGL::Key::Right);
			io.KeyMap[ImGuiKey_UpArrow] = static_cast<int>(LLGL::Key::Up);
			io.KeyMap[ImGuiKey_DownArrow] = static_cast<int>(LLGL::Key::Down);
			io.KeyMap[ImGuiKey_PageUp] = static_cast<int>(LLGL::Key::PageUp);
			io.KeyMap[ImGuiKey_PageDown] = static_cast<int>(LLGL::Key::PageDown);
			io.KeyMap[ImGuiKey_Home] = static_cast<int>(LLGL::Key::Home);
			io.KeyMap[ImGuiKey_End] = static_cast<int>(LLGL::Key::End);
			io.KeyMap[ImGuiKey_Insert] = static_cast<int>(LLGL::Key::Insert);
#ifdef ANDROID
			io.KeyMap[ImGuiKey_Backspace] = static_cast<int>(LLGL::Key::Delete);
#else
			io.KeyMap[ImGuiKey_Delete] = static_cast<int>(LLGL::Key::Delete);
			io.KeyMap[ImGuiKey_Backspace] = static_cast<int>(LLGL::Key::Back);
#endif
			io.KeyMap[ImGuiKey_Space] = static_cast<int>(LLGL::Key::Space);
			io.KeyMap[ImGuiKey_Enter] = static_cast<int>(LLGL::Key::Return);
			io.KeyMap[ImGuiKey_Escape] = static_cast<int>(LLGL::Key::Escape);
			io.KeyMap[ImGuiKey_A] = static_cast<int>(LLGL::Key::A);
			io.KeyMap[ImGuiKey_C] = static_cast<int>(LLGL::Key::C);
			io.KeyMap[ImGuiKey_V] = static_cast<int>(LLGL::Key::V);
			io.KeyMap[ImGuiKey_X] = static_cast<int>(LLGL::Key::X);
			io.KeyMap[ImGuiKey_Y] = static_cast<int>(LLGL::Key::Y);
			io.KeyMap[ImGuiKey_Z] = static_cast<int>(LLGL::Key::Z);

			// init rendering
			io.DisplaySize = ImVec2(window->GetSize().width, window->GetSize().height);

			/*if (s_fontTexture) { // delete previously created texture
				delete s_fontTexture;
			}
			s_fontTexture = new sf::Texture;

			if (loadDefaultFont) {
				// this will load default font automatically
				// No need to call AddDefaultFont
				UpdateFontTexture();
			}*/

			gWindowHasFocus = window->HasFocus();
		}
		/*void Update(const sf::Vector2i& mousePos, const sf::Vector2f& displaySize, float dt)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize = ImVec2(displaySize.x, displaySize.y);
			io.DeltaTime = dt;

			if (WindowHasFocus) {
				if (io.WantSetMousePos) {
					sf::Vector2i mousePos(static_cast<int>(io.MousePos.x), static_cast<int>(io.MousePos.y));
					sf::Mouse::setPosition(mousePos);
				}
				else {
					io.MousePos = ImVec2(mousePos.x, mousePos.y);
				}
				for (unsigned int i = 0; i < 3; i++) {
					io.MouseDown[i] = s_touchDown[i] || sf::Touch::isDown(i) || s_mousePressed[i] || sf::Mouse::isButtonPressed((sf::Mouse::Button)i);
					s_mousePressed[i] = false;
					s_touchDown[i] = false;
				}
			}
		}*/

		void ImGuiBinding::Update(LLGL::Window* window, float dt)
		{
			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize = ImVec2(window->GetSize().width, window->GetSize().height);
			io.DeltaTime = dt;

			if (gWindowHasFocus) {
				if (io.WantSetMousePos) 
				{
					window->SetMousePosition({ static_cast<int>(io.MousePos.x), static_cast<int>(io.MousePos.y) });
				}
				else 
				{
					io.MousePos = ImVec2(static_cast<float>(gInput->GetMousePosition().x), static_cast<float>(gInput->GetMousePosition().y));
				}
				/*for (unsigned int i = 0; i < 3; i++) {
					io.MouseDown[i] = s_touchDown[i] || sf::Touch::isDown(i) || s_mousePressed[i] || sf::Mouse::isButtonPressed((sf::Mouse::Button)i);
					s_mousePressed[i] = false;
					s_touchDown[i] = false;
				}*/
			}

			// Hide OS mouse cursor if imgui is drawing it
			if (ImGui::GetIO().MouseDrawCursor)
				LLGL::Display::ShowCursor(false);

		}


		/*void ProcessEvent(const sf::Event & event)
		{
			if (s_windowHasFocus) {
				ImGuiIO& io = ImGui::GetIO();

				switch (event.type) {
				case sf::Event::MouseMoved:
					s_mouseMoved = true;
					break;
				case sf::Event::MouseButtonPressed: // fall-through
				case sf::Event::MouseButtonReleased:
				{
					int button = event.mouseButton.button;
					if (event.type == sf::Event::MouseButtonPressed &&
						button >= 0 && button < 3) {
						s_mousePressed[event.mouseButton.button] = true;
					}
				}
				break;
				case sf::Event::TouchBegan: // fall-through
				case sf::Event::TouchEnded:
				{
					s_mouseMoved = false;
					int button = event.touch.finger;
					if (event.type == sf::Event::TouchBegan &&
						button >= 0 && button < 3) {
						s_touchDown[event.touch.finger] = true;
					}
				}
				break;
				case sf::Event::MouseWheelMoved:
					io.MouseWheel += static_cast<float>(event.mouseWheel.delta);
					break;
				case sf::Event::KeyPressed: // fall-through
				case sf::Event::KeyReleased:
					io.KeysDown[event.key.code] = (event.type == sf::Event::KeyPressed);
					io.KeyCtrl = event.key.control;
					io.KeyShift = event.key.shift;
					io.KeyAlt = event.key.alt;
					break;
				case sf::Event::TextEntered:
					if (event.text.unicode > 0 && event.text.unicode < 0x10000) {
						io.AddInputCharacter(static_cast<ImWchar>(event.text.unicode));
					}
					break;
				case sf::Event::JoystickConnected:
					if (s_joystickId == NULL_JOYSTICK_ID) {
						s_joystickId = event.joystickConnect.joystickId;
					}
					break;
				case sf::Event::JoystickDisconnected:
					if (s_joystickId == event.joystickConnect.joystickId) { // used gamepad was disconnected
						s_joystickId = getConnectedJoystickId();
					}
					break;
				default:
					break;
				}
			}

			switch (event.type) {
			case sf::Event::LostFocus:
				s_windowHasFocus = false;
				break;
			case sf::Event::GainedFocus:
				s_windowHasFocus = true;
				break;
			default:
				break;
			}

		}
	*/
}
}
