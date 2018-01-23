#include <Platform\Win32\Win32_Window.h>
#ifdef NUCLEAR_PLATFORM_WINDOWS_PC
#include <windowsx.h>
#include <Platform\Input.h>
#include <Core\Engine.h>
#include <iostream>

namespace NuclearEngine {

	namespace Platform {
		using namespace Input;

		namespace Internals {
			static int lastx;
			static int lasty;
			static int virtualx;
			static int virtualy;

			Win32_Window::Win32_Window()
			{

			}

			Win32_Window::Win32_Window(HANDLE handle)
			{
				this->WindPropHandle = handle;
			}

			Win32_Window::~Win32_Window()
			{
			}

			bool Win32_Window::Create(std::wstring title, int width, int height, bool fullscreenflag)
			{
				WindowRect.left = (long)0;			// Set Left Value To 0
				WindowRect.right = (long)width;		// Set Right Value To Requested Width
				WindowRect.top = (long)0;				// Set Top Value To 0
				WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

				fullscreen = false;			// Set The Global Fullscreen Flag

				Title = title.c_str();

				Width = width;
				Height = height;

				if (this->InitWindow() == FALSE)
				{
					return false;
				}
				return true;
			}

			void Win32_Window::Display()
			{
				// display the window on the screen
				ShowWindow(m_Handle, SW_SHOW);
			}

			void Win32_Window::SetTitle(std::wstring _title)
			{
				this->Title = _title.c_str();

				SetWindowText(m_Handle, Title);
				return;
			}

			void Win32_Window::UpdateRectClip(bool flag)
			{
				if (flag)
				{
					RECT clipRect;
					GetClientRect(m_Handle, &clipRect);
					ClientToScreen(m_Handle, (POINT*)&clipRect.left);
					ClientToScreen(m_Handle, (POINT*)&clipRect.right);
					ClipCursor(&clipRect);
				}
				else
				{
					ClipCursor(NULL);
				}
			}

			HWND Win32_Window::GetHandle()
			{
				return this->m_Handle;
			}

			HINSTANCE Win32_Window::GetInstance()
			{
				return hInstance;
			}

			void Win32_Window::ProcessEvents()
			{
				MSG msg;

				while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
				{
					if (msg.message == WM_QUIT)
					{
						ShouldClose = true;
					}
					else
					{
						TranslateMessage(&msg);
						DispatchMessage(&msg);
					}
				}

				if (Input::Mouse::GetInputMode() == Input::Mouse::InputMode::Virtual)
				{
					if (lastx != Width / 2 || lasty != Height / 2)
					{
						Input::Mouse::SetPosition(Width / 2, Height / 2);
					}
				}
			}

			void Win32_Window::KillWindow()								// Properly Kill The Window
			{
				if (fullscreen)										// Are We In Fullscreen Mode?
				{
					ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop
					ShowCursor(TRUE);								// Show Mouse Pointer
				}

				CleanWindow();

			}

			unsigned int Win32_Window::GetWidth()
			{
				return this->Width;
			}

			unsigned int Win32_Window::GetHeight()
			{
				return this->Height;
			}

			const wchar_t* Win32_Window::GetTitle()
			{
				return this->Title;
			}

			void Win32_Window::updateCursorImage()
			{
				//if (Input::Mouse::GetInputMode() == Input::Mouse::InputMode::Normal)
				//{
				//	/*if (window->cursor)
				//		SetCursor(window->cursor->win32.handle);
				//	else*/
				//		SetCursor(LoadCursorW(NULL, IDC_ARROW));
				//}
				//else
				//{
				//	SetCursor(NULL);
				//}
			}

			BOOL Win32_Window::InitWindow()
			{

				hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
				wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;	// Redraw On Size, And Own DC For Window.
				wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
				wc.cbClsExtra = 0;									// No Extra Window Data
				wc.cbWndExtra = 0;									// No Extra Window Data
				wc.hInstance = hInstance;							// Set The Instance
				wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
				wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
				wc.hbrBackground = NULL;									// No Background Required For GL
				wc.lpszMenuName = NULL;									// We Don't Want A Menu
				wc.lpszClassName = L"Platform";								// Set The Class Name


				if (!RegisterClass(&wc))									// Attempt To Register The Window Class
				{
					MessageBox(NULL, L"Failed To Register The Window Class.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
					return FALSE;											// Return FALSE
				}

				if (fullscreen)												// Attempt Fullscreen Mode?
				{
					DEVMODE dmScreenSettings;								// Device Mode
					memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
					dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
					dmScreenSettings.dmPelsWidth = Width;				// Selected Screen Width
					dmScreenSettings.dmPelsHeight = Height;				// Selected Screen Height
					dmScreenSettings.dmBitsPerPel = 32;					// Selected Bits Per Pixel
					dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

					// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
					if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
					{
						// If The Mode Fails, Offer Two Options.  Quit Or Bind Windowed Mode.
						if (MessageBox(NULL, L"The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Bind Windowed Mode Instead?", L"Info", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
						{
							fullscreen = FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
						}
						else
						{
							// Pop Up A Message Box Letting User Know The Program Is Closing.
							MessageBox(NULL, L"Engine Will Now Close.", L"ERROR", MB_OK | MB_ICONSTOP);
							return FALSE;									// Return FALSE
						}
					}
				}

				if (fullscreen)												// Are We Still In Fullscreen Mode?
				{
					dwExStyle = WS_EX_APPWINDOW;								// Window Extended Style
					dwStyle = WS_POPUP;										// Windows Style
				}
				else
				{
					dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
					dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;							// Windows Style
				}

				AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

																				// Create The Window
				if (!(m_Handle = CreateWindowEx(dwExStyle,							// Extended Style For The Window
					L"Platform",							// Class Name
					Title,								// Window Title
					dwStyle |							// Defined Window Style
					WS_CLIPSIBLINGS |					// Required Window Style
					WS_CLIPCHILDREN,					// Required Window Style
					0, 0,								// Window Position
					WindowRect.right - WindowRect.left,	// Calculate Window Width
					WindowRect.bottom - WindowRect.top,	// Calculate Window Height
					NULL,								// No Parent Window
					NULL,								// No Menu
					hInstance,							// Instance
					NULL)))								// Dont Pass Anything To WM_CREATE
				{
					KillWindow();								// Reset The Display
					MessageBox(NULL, L"Window Creation Error.", L"ERROR", MB_OK | MB_ICONEXCLAMATION);
					return FALSE;								// Return FALSE
				}

				SetProp(m_Handle, L"PlatformProp", WindPropHandle);
				return TRUE;									// Success
			}

			BOOL Win32_Window::CleanWindow()
			{
				if (m_Handle && !DestroyWindow(m_Handle))					// Are We Able To Destroy The Window?
				{
					m_Handle = NULL;										// Set hWnd To NULL
				}

				if (!UnregisterClass(L"Platform", hInstance))			// Are We Able To Unregister Class
				{
					hInstance = NULL;									// Set hInstance To NULL
				}

				return TRUE;
			}

			LRESULT CALLBACK Win32_Window::WndProc(HWND	hWnd, UINT	uMsg, WPARAM	wParam, LPARAM	lParam)
			{
				Win32_Window window = GetProp(hWnd, L"PlatformProp");
				PAINTSTRUCT ps;

				switch (uMsg)									// Check For Windows Messages
				{
				case WM_PAINT:
					BeginPaint(hWnd, &ps);
					EndPaint(hWnd, &ps);
					break;
				case WM_SYSCOMMAND:							// Intercept System Commands
				{
					switch (wParam)							// Check System Calls
					{
					case SC_SCREENSAVE:					// Screensaver Trying To Start?
					case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
						return 0;							// Prevent From Happening
					}
					break;
				}

				case WM_CLOSE:								// Did We Receive A Close Message?
				{
					window.ShouldClose = TRUE;
					PostQuitMessage(0);						// Send A Quit Message
					return 0;								// Jump Back
				}

				case WM_LBUTTONDOWN:
				{
					Mouse::buttons[(unsigned char)Mouse::Button::Left] = true;
					return 0;								// Jump Back
				}

				case WM_LBUTTONUP:
				{
					Mouse::buttons[(unsigned char)Mouse::Button::Left] = false;
					return 0;								// Jump Back
				}

				case WM_MBUTTONDOWN:
				{
					Mouse::buttons[(unsigned char)Mouse::Button::Middle] = true;
					return 0;								// Jump Back
				}

				case WM_MBUTTONUP:
				{
					Mouse::buttons[(unsigned char)Mouse::Button::Middle] = false;
					return 0;								// Jump Back

				}

				case WM_RBUTTONDOWN:
				{
					Mouse::buttons[(unsigned char)Mouse::Button::Right] = true;
					return 0;								// Jump Back
				}

				case WM_RBUTTONUP:
				{
					Mouse::buttons[(unsigned char)Mouse::Button::Right] = false;
					return 0;								// Jump Back
				}

				case WM_XBUTTONDOWN:
				{
					int buttonID = GET_XBUTTON_WPARAM(wParam);

					if (buttonID == 1)
						Mouse::buttons[(unsigned char)Mouse::Button::X1] = true;
					else if (buttonID == 2)
						Mouse::buttons[(unsigned char)Mouse::Button::X2] = true;
					return 0;								// Jump Back
				}

				case WM_XBUTTONUP:
				{
					int buttonID = GET_XBUTTON_WPARAM(wParam);

					if (buttonID == 1)
						Mouse::buttons[(unsigned char)Mouse::Button::X1] = false;
					else if (buttonID == 2)
						Mouse::buttons[(unsigned char)Mouse::Button::X2] = false;
					return 0;								// Jump Back
				}

				case WM_MOUSEMOVE:								// Resize The OpenGL Window
				{

					int x = GET_X_LPARAM(lParam);
					int y = GET_Y_LPARAM(lParam);

					if (Input::Mouse::GetInputMode() == Input::Mouse::InputMode::Virtual)
					{
						int dx = x - lastx;
						int dy = y - lasty;

						// get mouse coordinates from Windows
						Core::Engine::GetGame()->OnMouseMovement(virtualx + dx, virtualy + dy);
					}
					else
					{
						Core::Engine::GetGame()->OnMouseMovement(x,y);
					}
					lastx = x;
					lasty = y;

					return 0;								// Jump Back
				}
				case WM_SIZE:
				{
					Core::Engine::GetGame()->OnWindowResize(LOWORD(lParam), HIWORD(lParam));

					return 0;
				}
				/*	case WM_SETCURSOR:
					{
						if (LOWORD(lParam) == HTCLIENT)
						{
							window.updateCursorImage();
							return TRUE;
						}

						break;
					}*/

				case WM_SETFOCUS:
				{
					if (Input::Mouse::GetInputMode() == Input::Mouse::InputMode::Virtual)
					{
						Input::Mouse::SetInputMode(Input::Mouse::InputMode::Virtual);
					}
					return 0;
				}

				case WM_KILLFOCUS:
				{
					if (Input::Mouse::GetInputMode() == Input::Mouse::InputMode::Virtual)
					{
						Input::Mouse::SetInputMode(Input::Mouse::InputMode::Normal);
					}
					return 0;
				}

				case WM_MOVE:
				{
					if (Input::Mouse::GetInputMode() == Input::Mouse::InputMode::Virtual)
					{
						window.UpdateRectClip(true);
					}

					//TODO: Implement GetWindowPos...
					return 0;
				}

				}

				// Pass All Unhandled Messages To DefWindowProc
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
		}
	}
}
#endif