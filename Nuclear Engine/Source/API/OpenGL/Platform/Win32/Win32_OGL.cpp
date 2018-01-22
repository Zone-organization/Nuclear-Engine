#include "Win32_OGL.h"
#include <Core\Application.h>
#ifdef NUCLEAR_PLATFORM_WINDOWS32
#include <Core\Context.h>

#include <GLEXT\glext.h>
#include <GLEXT\wglext.h>

#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment(lib,"GLAD.lib")

namespace NuclearEngine
{
	namespace API
	{
		namespace OpenGL
		{
			namespace Internals {

				static HDC hDC = NULL;
				static HWND hWndFake;
				static HGLRC hRCFake;
				static HDC hDCFake;
				LRESULT CALLBACK MsgHandlerSimpleOpenGLClass(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam)
				{
					PAINTSTRUCT ps;
					switch (uiMsg)
					{
					case WM_PAINT:
						BeginPaint(hWnd, &ps);
						EndPaint(hWnd, &ps);
						break;

					default:
						return DefWindowProc(hWnd, uiMsg, wParam, lParam); // Default window procedure
					}
					return 0;
				}

				bool CreateDummyGLWindow()
				{
					WNDCLASSEX wc;

					HINSTANCE hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window

					wc.cbSize = sizeof(WNDCLASSEX);
					wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
					wc.lpfnWndProc = (WNDPROC)MsgHandlerSimpleOpenGLClass;
					wc.cbClsExtra = 0; wc.cbWndExtra = 0;
					wc.hInstance = hInstance;
					wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
					wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
					wc.hCursor = LoadCursor(NULL, IDC_ARROW);
					wc.hbrBackground = (HBRUSH)(COLOR_MENUBAR + 1);
					wc.lpszMenuName = NULL;
					wc.lpszClassName = L"FAKE";

					RegisterClassEx(&wc);

					hWndFake = CreateWindow(L"FAKE", L"FAKE", WS_OVERLAPPEDWINDOW | WS_MAXIMIZE | WS_CLIPCHILDREN,
						0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
						NULL, hInstance, NULL);

					hDCFake = GetDC(hWndFake);
					// First, choose false pixel format

					PIXELFORMATDESCRIPTOR pfd;
					memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
					pfd =
					{
						sizeof(PIXELFORMATDESCRIPTOR),
						1,
						PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
						PFD_TYPE_RGBA,
						32,
						0, 0, 0, 0, 0, 0,
						0,
						0,
						0,
						0, 0, 0, 0,
						32,
						8,
						0,
						PFD_MAIN_PLANE,
						0,
						0, 0, 0
					};

					int iPixelFormat = ChoosePixelFormat(hDCFake, &pfd);
					if (iPixelFormat == 0)
						return false;

					if (!SetPixelFormat(hDCFake, iPixelFormat, &pfd))
					{
						MessageBox(NULL, L"Set Pixel Format Failed!!", L"ERROR", MB_OK | MB_ICONERROR);
						return false;
					}

					// Create the false, old style context (OpenGL 2.1 and before)

					hRCFake = wglCreateContext(hDCFake);
					wglMakeCurrent(hDCFake, hRCFake);

					return true;
				}
				void DestroyDummyGLWindow()
				{
					wglMakeCurrent(NULL, NULL);
					wglDeleteContext(hRCFake);
					DestroyWindow(hWndFake);

					DeleteDC(hDCFake);
					hDCFake = NULL;
				}

				void APIENTRY glDebugOutput(GLenum source,	GLenum type, GLuint id, GLenum severity, GLsizei length,	const GLchar *message,const void *userParam)
				{
					// ignore non-significant error/warning codes
					if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

					std::string Message("[OpenGL Debug Output] [" + std::to_string(id) + "] " + message);
					switch (source)
					{
					case GL_DEBUG_SOURCE_API:             Message + "Source: API"; break;
					case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   Message + "Source: Window System"; break;
					case GL_DEBUG_SOURCE_SHADER_COMPILER: Message + "Source: Shader Compiler"; break;
					case GL_DEBUG_SOURCE_THIRD_PARTY:     Message + "Source: Third Party"; break;
					case GL_DEBUG_SOURCE_APPLICATION:     Message + "Source: Application"; break;
					case GL_DEBUG_SOURCE_OTHER:           Message + "Source: Other"; break;
					} Message + ('\n');

					switch (type)
					{
					case GL_DEBUG_TYPE_ERROR:               Message + "Type: Error"; break;
					case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: Message + "Type: Deprecated Behaviour"; break;
					case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  Message + "Type: Undefined Behaviour"; break;
					case GL_DEBUG_TYPE_PORTABILITY:         Message + "Type: Portability"; break;
					case GL_DEBUG_TYPE_PERFORMANCE:         Message + "Type: Performance"; break;
					case GL_DEBUG_TYPE_MARKER:              Message + "Type: Marker"; break;
					case GL_DEBUG_TYPE_PUSH_GROUP:          Message + "Type: Push Group"; break;
					case GL_DEBUG_TYPE_POP_GROUP:           Message + "Type: Pop Group"; break;
					case GL_DEBUG_TYPE_OTHER:               Message + "Type: Other"; break;
					} Message + ('\n');

					switch (severity)
					{
					case GL_DEBUG_SEVERITY_HIGH:         Message + "Severity: high"; break;
					case GL_DEBUG_SEVERITY_MEDIUM:       Message + "Severity: medium"; break;
					case GL_DEBUG_SEVERITY_LOW:          Message + "Severity: low"; break;
					case GL_DEBUG_SEVERITY_NOTIFICATION: Message + "Severity: notification"; break;
					} Message + ('\n');
				}
				bool Win32_OGL::Initialize()
				{
					Log->Info("[Engine] Initializing OpenGL On Windows (WGL)...\n");

					CreateDummyGLWindow();
					PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
					wglChoosePixelFormatARB = reinterpret_cast<PFNWGLCHOOSEPIXELFORMATARBPROC>(wglGetProcAddress("wglChoosePixelFormatARB"));
					if (wglChoosePixelFormatARB == nullptr) {
						Log->Error("[OpenGL] wglChoosePixelFormatARB cast failed!\n");
						return false;
					}

					PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
					wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));
					if (wglCreateContextAttribsARB == nullptr) {
						Log->Error("[OpenGL] wglCreateContextAttribsARB cast failed!\n");
						return false;
					}
					DestroyDummyGLWindow();

					hDC = GetDC(Core::Application::GetInternalWindow()->GetHandle());

					PIXELFORMATDESCRIPTOR pfd;
					memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
					pfd =
					{
						sizeof(PIXELFORMATDESCRIPTOR),
						1,
						PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    //Flags
						PFD_TYPE_RGBA,            //The kind of frameVertexBuffer. RGBA or palette.
						32,
						0, 0, 0, 0, 0, 0,
						0,
						0,
						0,
						0, 0, 0, 0,
						32,
						8,
						0,
						PFD_MAIN_PLANE,
						0,
						0, 0, 0
					};

					const int iPixelFormatAttribList[] =
					{
						WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
						WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
						WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
						WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
						WGL_COLOR_BITS_ARB, 32,
						WGL_DEPTH_BITS_ARB, 24,
						WGL_STENCIL_BITS_ARB, 8,
						0 // End of attributes list
					};
					
					int iContextAttribs[] =
					{
						WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
						WGL_CONTEXT_MINOR_VERSION_ARB, 3,
						WGL_CONTEXT_FLAGS_ARB,
						WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB |
						WGL_CONTEXT_DEBUG_BIT_ARB,
						WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
						0
					};

					int iPixelFormat, iNumFormats;
					
					wglChoosePixelFormatARB(hDC, iPixelFormatAttribList, NULL, 1, &iPixelFormat, (UINT*)&iNumFormats);

					// PFD seems to be only redundant parameter now
					if (!SetPixelFormat(hDC, iPixelFormat, &pfd))
						return false;

					HGLRC hRC = wglCreateContextAttribsARB(hDC, 0, iContextAttribs);
					// If everything went OK
					if (hRC) wglMakeCurrent(hDC, hRC);
		
					if (!gladLoadGL())
					{
						Log->Error("[OpenGL] GLAD Initialization failed!\n");
						return false;
					}

					Log->Info("[OpenGL] (WGL & GLAD) Successfully Initialized\n");
					Log->Info("[OpenGL] Version: ");
					Log->Info((char*)glGetString(GL_VERSION));
					Log->Info("\n[OpenGL] Vendor: ");
					Log->Info((char*)glGetString(GL_VENDOR));
					Log->Info("\n[OpenGL] Renderer: ");
					Log->Info((char*)glGetString(GL_RENDERER));
					Log->Info("\n[OpenGL] Shading: ");
					Log->Info((char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
					Log->Info("\n");
					if (GLAD_GL_EXT_texture_filter_anisotropic)
					{
						glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &Core::ContextDesc::MaxAnisotropicLevel);
					}
					else
					{
						Core::ContextDesc::MaxAnisotropicLevel = 0.0f;
						Log->Warning("[OpenGL] Anisotropic filtering isn't supported on this hardware, therefore any ansiotropic filtering will fall back to trilinear filtering...\n");
					}	

					if (!GLAD_GL_ARB_debug_output)
					{
						Log->Warning("[OpenGL] Synchronous Debug Output (DebugLayer) isn't supported on this hardware, therefore Dubug Output will rely on glGetError which is limited...\n");
					}
					else {
						Core::ContextDesc::SupprtDebugLayer = true;
						
						glEnable(GL_DEBUG_OUTPUT);
						glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
						glDebugMessageCallback(glDebugOutput, nullptr);
						glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
						
					}

					return true;										// Initialization Went OK
				}
				void Win32_OGL::Shutdown()
				{
					wglMakeCurrent(NULL, NULL);
				}
				void Win32_OGL::SwapBuffer()
				{
					SwapBuffers(hDC);
				}
			}
		}

	}

}
#endif