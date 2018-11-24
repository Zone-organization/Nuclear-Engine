#include "Engine\Graphics\ImGui.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Touch.hpp>
#include <SFML/Window/Window.hpp>
#include "Engine\Graphics\ImGui_Binding.h"

namespace
{
	// data
	static bool s_windowHasFocus = false;
	static bool s_mousePressed[3] = { false, false, false };
	static bool s_touchDown[3] = { false, false, false };
	static bool s_mouseMoved = false;
	static sf::Vector2i s_touchPos;

	static const unsigned int NULL_JOYSTICK_ID = sf::Joystick::Count;
	static unsigned int s_joystickId = NULL_JOYSTICK_ID;

	static const unsigned int NULL_JOYSTICK_BUTTON = sf::Joystick::ButtonCount;
	static unsigned int s_joystickMapping[ImGuiNavInput_COUNT];

	struct StickInfo {
		sf::Joystick::Axis xAxis;
		sf::Joystick::Axis yAxis;

		bool xInverted;
		bool yInverted;

		float threshold;
	};

	StickInfo s_dPadInfo;
	StickInfo s_lStickInfo;

	// Default mapping is XInput gamepad mapping
	//void initDefaultJoystickMapping();

	// Returns first id of connected joystick
	unsigned int getConnectedJoystickId()
	{
		for (unsigned int i = 0; i < (unsigned int)sf::Joystick::Count; ++i) {
			if (sf::Joystick::isConnected(i)) return i;
		}

		return NULL_JOYSTICK_ID;
	}

	void updateJoystickActionState(ImGuiIO& io, ImGuiNavInput_ action);
	void updateJoystickDPadState(ImGuiIO& io);
	void updateJoystickLStickState(ImGuiIO& io);
}

namespace NuclearEngine
{
	namespace Graphics
	{
		namespace ImGui_Binding
		{
			void Init(sf::Window& window)
			{
				ImGui::CreateContext();
				ImGuiIO& io = ImGui::GetIO();

				io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

				// init keyboard mapping
				io.KeyMap[ImGuiKey_Tab] = sf::Keyboard::Tab;
				io.KeyMap[ImGuiKey_LeftArrow] = sf::Keyboard::Left;
				io.KeyMap[ImGuiKey_RightArrow] = sf::Keyboard::Right;
				io.KeyMap[ImGuiKey_UpArrow] = sf::Keyboard::Up;
				io.KeyMap[ImGuiKey_DownArrow] = sf::Keyboard::Down;
				io.KeyMap[ImGuiKey_PageUp] = sf::Keyboard::PageUp;
				io.KeyMap[ImGuiKey_PageDown] = sf::Keyboard::PageDown;
				io.KeyMap[ImGuiKey_Home] = sf::Keyboard::Home;
				io.KeyMap[ImGuiKey_End] = sf::Keyboard::End;
				io.KeyMap[ImGuiKey_Insert] = sf::Keyboard::Insert;
#ifdef ANDROID
				io.KeyMap[ImGuiKey_Backspace] = sf::Keyboard::Delete;
#else
				io.KeyMap[ImGuiKey_Delete] = sf::Keyboard::Delete;
				io.KeyMap[ImGuiKey_Backspace] = sf::Keyboard::BackSpace;
#endif
				io.KeyMap[ImGuiKey_Space] = sf::Keyboard::Space;
				io.KeyMap[ImGuiKey_Enter] = sf::Keyboard::Return;
				io.KeyMap[ImGuiKey_Escape] = sf::Keyboard::Escape;
				io.KeyMap[ImGuiKey_A] = sf::Keyboard::A;
				io.KeyMap[ImGuiKey_C] = sf::Keyboard::C;
				io.KeyMap[ImGuiKey_V] = sf::Keyboard::V;
				io.KeyMap[ImGuiKey_X] = sf::Keyboard::X;
				io.KeyMap[ImGuiKey_Y] = sf::Keyboard::Y;
				io.KeyMap[ImGuiKey_Z] = sf::Keyboard::Z;

				io.BackendFlags |= ImGuiBackendFlags_HasGamepad;
				s_joystickId = getConnectedJoystickId();

				for (unsigned int i = 0; i < ImGuiNavInput_COUNT; i++) {
					s_joystickMapping[i] = NULL_JOYSTICK_BUTTON;
				}

				///initDefaultJoystickMapping();

				// init rendering
				/*io.DisplaySize = static_cast<sf::Vector2f>(target.getSize());

				if (s_fontTexture) { // delete previously created texture
					delete s_fontTexture;
				}
				s_fontTexture = new sf::Texture;

				if (loadDefaultFont) {
					// this will load default font automatically
					// No need to call AddDefaultFont
					UpdateFontTexture();
				}*/

				s_windowHasFocus = window.hasFocus();
			}
			void ProcessEvent(const sf::Event & event)
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
		
		}
	}
}
