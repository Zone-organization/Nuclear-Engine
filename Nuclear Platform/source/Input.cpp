#include <NuclearPlatform\Input.h>
#include <NuclearPlatform\Application.h>
#ifdef NUCLEAR_PLATFORM_WINDOWS32
#include <windows.h>
#endif

namespace NuclearPlatform {
	namespace Input {
		bool Mouse::buttons[NUCLEAR_PLATFORM_MAX_MOUSE_KEYS];
		static Mouse::InputMode _mode;
		bool Keyboard::IsKeyPressed(Key::Code key)
		{
			int vkey = 0;
			switch (key)
			{
			default:                   vkey = 0;             break;
			case Keyboard::Key::Code::A:          vkey = 'A';           break;
			case Keyboard::Key::Code::B:          vkey = 'B';           break;
			case Keyboard::Key::Code::C:          vkey = 'C';           break;
			case Keyboard::Key::Code::D:          vkey = 'D';           break;
			case Keyboard::Key::Code::E:          vkey = 'E';           break;
			case Keyboard::Key::Code::F:          vkey = 'F';           break;
			case Keyboard::Key::Code::G:          vkey = 'G';           break;
			case Keyboard::Key::Code::H:          vkey = 'H';           break;
			case Keyboard::Key::Code::I:          vkey = 'I';           break;
			case Keyboard::Key::Code::J:          vkey = 'J';           break;
			case Keyboard::Key::Code::K:          vkey = 'K';           break;
			case Keyboard::Key::Code::L:          vkey = 'L';           break;
			case Keyboard::Key::Code::M:          vkey = 'M';           break;
			case Keyboard::Key::Code::N:          vkey = 'N';           break;
			case Keyboard::Key::Code::O:          vkey = 'O';           break;
			case Keyboard::Key::Code::P:          vkey = 'P';           break;
			case Keyboard::Key::Code::Q:          vkey = 'Q';           break;
			case Keyboard::Key::Code::R:          vkey = 'R';           break;
			case Keyboard::Key::Code::S:          vkey = 'S';           break;
			case Keyboard::Key::Code::T:          vkey = 'T';           break;
			case Keyboard::Key::Code::U:          vkey = 'U';           break;
			case Keyboard::Key::Code::V:          vkey = 'V';           break;
			case Keyboard::Key::Code::W:          vkey = 'W';           break;
			case Keyboard::Key::Code::X:          vkey = 'X';           break;
			case Keyboard::Key::Code::Y:          vkey = 'Y';           break;
			case Keyboard::Key::Code::Z:          vkey = 'Z';           break;
			case Keyboard::Key::Code::Num0:       vkey = '0';           break;
			case Keyboard::Key::Code::Num1:       vkey = '1';           break;
			case Keyboard::Key::Code::Num2:       vkey = '2';           break;
			case Keyboard::Key::Code::Num3:       vkey = '3';           break;
			case Keyboard::Key::Code::Num4:       vkey = '4';           break;
			case Keyboard::Key::Code::Num5:       vkey = '5';           break;
			case Keyboard::Key::Code::Num6:       vkey = '6';           break;
			case Keyboard::Key::Code::Num7:       vkey = '7';           break;
			case Keyboard::Key::Code::Num8:       vkey = '8';           break;
			case Keyboard::Key::Code::Num9:       vkey = '9';           break;
			case Keyboard::Key::Code::Escape:     vkey = VK_ESCAPE;     break;
			case Keyboard::Key::Code::LControl:   vkey = VK_LCONTROL;   break;
			case Keyboard::Key::Code::LShift:     vkey = VK_LSHIFT;     break;
			case Keyboard::Key::Code::LAlt:       vkey = VK_LMENU;      break;
			case Keyboard::Key::Code::LSystem:    vkey = VK_LWIN;       break;
			case Keyboard::Key::Code::RControl:   vkey = VK_RCONTROL;   break;
			case Keyboard::Key::Code::RShift:     vkey = VK_RSHIFT;     break;
			case Keyboard::Key::Code::RAlt:       vkey = VK_RMENU;      break;
			case Keyboard::Key::Code::RSystem:    vkey = VK_RWIN;       break;
			case Keyboard::Key::Code::Menu:       vkey = VK_APPS;       break;
			case Keyboard::Key::Code::LBracket:   vkey = VK_OEM_4;      break;
			case Keyboard::Key::Code::RBracket:   vkey = VK_OEM_6;      break;
			case Keyboard::Key::Code::SemiColon:  vkey = VK_OEM_1;      break;
			case Keyboard::Key::Code::Comma:      vkey = VK_OEM_COMMA;  break;
			case Keyboard::Key::Code::Period:     vkey = VK_OEM_PERIOD; break;
			case Keyboard::Key::Code::Quote:      vkey = VK_OEM_7;      break;
			case Keyboard::Key::Code::Slash:      vkey = VK_OEM_2;      break;
			case Keyboard::Key::Code::BackSlash:  vkey = VK_OEM_5;      break;
			case Keyboard::Key::Code::Tilde:      vkey = VK_OEM_3;      break;
			case Keyboard::Key::Code::Equal:      vkey = VK_OEM_PLUS;   break;
			case Keyboard::Key::Code::Dash:       vkey = VK_OEM_MINUS;  break;
			case Keyboard::Key::Code::Space:      vkey = VK_SPACE;      break;
			case Keyboard::Key::Code::Return:     vkey = VK_RETURN;     break;
			case Keyboard::Key::Code::BackSpace:  vkey = VK_BACK;       break;
			case Keyboard::Key::Code::Tab:        vkey = VK_TAB;        break;
			case Keyboard::Key::Code::PageUp:     vkey = VK_PRIOR;      break;
			case Keyboard::Key::Code::PageDown:   vkey = VK_NEXT;       break;
			case Keyboard::Key::Code::End:        vkey = VK_END;        break;
			case Keyboard::Key::Code::Home:       vkey = VK_HOME;       break;
			case Keyboard::Key::Code::Insert:     vkey = VK_INSERT;     break;
			case Keyboard::Key::Code::Delete:     vkey = VK_DELETE;     break;
			case Keyboard::Key::Code::Add:        vkey = VK_ADD;        break;
			case Keyboard::Key::Code::Subtract:   vkey = VK_SUBTRACT;   break;
			case Keyboard::Key::Code::Multiply:   vkey = VK_MULTIPLY;   break;
			case Keyboard::Key::Code::Divide:     vkey = VK_DIVIDE;     break;
			case Keyboard::Key::Code::Left:       vkey = VK_LEFT;       break;
			case Keyboard::Key::Code::Right:      vkey = VK_RIGHT;      break;
			case Keyboard::Key::Code::Up:         vkey = VK_UP;         break;
			case Keyboard::Key::Code::Down:       vkey = VK_DOWN;       break;
			case Keyboard::Key::Code::Numpad0:    vkey = VK_NUMPAD0;    break;
			case Keyboard::Key::Code::Numpad1:    vkey = VK_NUMPAD1;    break;
			case Keyboard::Key::Code::Numpad2:    vkey = VK_NUMPAD2;    break;
			case Keyboard::Key::Code::Numpad3:    vkey = VK_NUMPAD3;    break;
			case Keyboard::Key::Code::Numpad4:    vkey = VK_NUMPAD4;    break;
			case Keyboard::Key::Code::Numpad5:    vkey = VK_NUMPAD5;    break;
			case Keyboard::Key::Code::Numpad6:    vkey = VK_NUMPAD6;    break;
			case Keyboard::Key::Code::Numpad7:    vkey = VK_NUMPAD7;    break;
			case Keyboard::Key::Code::Numpad8:    vkey = VK_NUMPAD8;    break;
			case Keyboard::Key::Code::Numpad9:    vkey = VK_NUMPAD9;    break;
			case Keyboard::Key::Code::F1:         vkey = VK_F1;         break;
			case Keyboard::Key::Code::F2:         vkey = VK_F2;         break;
			case Keyboard::Key::Code::F3:         vkey = VK_F3;         break;
			case Keyboard::Key::Code::F4:         vkey = VK_F4;         break;
			case Keyboard::Key::Code::F5:         vkey = VK_F5;         break;
			case Keyboard::Key::Code::F6:         vkey = VK_F6;         break;
			case Keyboard::Key::Code::F7:         vkey = VK_F7;         break;
			case Keyboard::Key::Code::F8:         vkey = VK_F8;         break;
			case Keyboard::Key::Code::F9:         vkey = VK_F9;         break;
			case Keyboard::Key::Code::F10:        vkey = VK_F10;        break;
			case Keyboard::Key::Code::F11:        vkey = VK_F11;        break;
			case Keyboard::Key::Code::F12:        vkey = VK_F12;        break;
			case Keyboard::Key::Code::F13:        vkey = VK_F13;        break;
			case Keyboard::Key::Code::F14:        vkey = VK_F14;        break;
			case Keyboard::Key::Code::F15:        vkey = VK_F15;        break;
			case Keyboard::Key::Code::Pause:      vkey = VK_PAUSE;      break;
			}

			return (GetAsyncKeyState(vkey) & 0x8000) != 0;
		}

		bool Mouse::IsButtonPressed(Button button)
		{
			return buttons[(unsigned char)button];
		}

		void Mouse::SetPosition(int x, int y)
		{
#ifdef NUCLEAR_PLATFORM_WINDOWS32
			POINT point;
			point.x = x;
			point.y = y;

			ClientToScreen(Application::GetInternalWindow()->GetHandle(), &point);
			SetCursorPos(point.x, point.y);
#endif
		}

		void Mouse::GetPosition(int& x, int& y)
		{
#ifdef NUCLEAR_PLATFORM_WINDOWS32
			POINT pos;

			if (GetCursorPos(&pos))
			{
				ScreenToClient(Application::GetInternalWindow()->GetHandle(), &pos);
				x = pos.x;
				y = pos.y;
			}
#endif
		}

		void Mouse::SetInputMode(InputMode mode)
		{
			_mode = mode;

			if (_mode == Mouse::InputMode::Virtual)
			{
				Application::GetInternalWindow()->UpdateRectClip(true);
				SetPosition(Application::GetWidth() / 2.0, Application::GetHeight() / 2.0);
			}
		}

		Mouse::InputMode Mouse::GetInputMode()
		{
			return _mode;
		}

		void Mouse::ShowMouseCursor(bool value)
		{
#ifdef NUCLEAR_PLATFORM_WINDOWS32
			ShowCursor(value);
#endif
		}

	}
}