#include "stella/graphics/display.h"

#include <glad/glad.h>
#include <SDL2/SDL.h>

#include <sstream>
#include <iostream>

namespace stella {
namespace graphics {
double MouseX, MouseY;

Display::Display(GLuint width, GLuint height, const std::string &title)
    : Width(width), Height(height), Title(title) {
  // GLFW initialization
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    std::cout << "It was not possible to initialize SDL2" << std::endl;
	
	//this->Window = SDL_CreateWindow(this->Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->Width, this->Height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
	this->Window = SDL_CreateWindow(this->Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, this->Width, this->Height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	SDL_ShowCursor(SDL_DISABLE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_CreateContext(this->Window);

  this->Running = true;

  // Set initial value for Frame
  this->Frame = 1; // Setting as 1 to avoid division by 0
  this->LastFPSCheck = this->LastTime = (float)SDL_GetTicks()/1000.0f;
  this->LastFrame = 0;

  if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
  }   

  // OpenGL Viewport settings
  glViewport(0, 0, this->Width, this->Height);
  glEnable(GL_BLEND);
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // Set default Clear Color
  this->ClearColor = glm::vec3(0.0f, 0.0f, 0.0f);

  // Init debug GUI
  this->DGUI.Init(this->Window);
}

Display::~Display() {
  SDL_DestroyWindow(this->Window);
  SDL_Quit();
}

GLuint Display::GetWidth() {
  int width, height;
  SDL_GetWindowSize(this->Window, &width, &height);
  return width;
}

GLuint Display::GetHeight() {
  int width, height;
  SDL_GetWindowSize(this->Window, &width, &height);
  return height;
}

bool Display::IsRunning() { return this->Running; }

void Display::Update() {
  this->getDT();
  this->Frame++;
  if (this->Frame >= 10000000)
    this->Frame = 0;

   //Print FPS 
  //if (this->Frame % 120 == 0) {
     //std::cout << this->getFPS() << std::endl;
  //}

  this->updateInput();
  //this->DGUI.Update();
  SDL_GL_SwapWindow(this->Window);
}

void Display::SetClearColor(int r, int g, int b) {
  this->ClearColor.x = r / 255.0f;
  this->ClearColor.y = g / 255.0f;
  this->ClearColor.z = b / 255.0f;
}

void Display::SetClearColor(GLfloat x, GLfloat y, GLfloat z) {
  if (x > 1.0f)
    x = 1.0f;
  if (y > 1.0f)
    y = 1.0f;
  if (z > 1.0f)
    z = 1.0f;

  this->ClearColor.x = x;
  this->ClearColor.y = y;
  this->ClearColor.z = z;
}

void Display::Clear() {
  glClearColor(this->ClearColor.x, this->ClearColor.y, this->ClearColor.z, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Display::updateInput() {
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    //this->DGUI.GetInput(event);
    if (event.type == SDL_QUIT)
      this->Running = false;
    if (event.type == SDL_KEYDOWN)
    {
      switch (event.key.keysym.sym)
      {
      case SDLK_ESCAPE:
        this->Running = false;
        break;
      default:
        break;
      }
    }
    if (event.type == SDL_WINDOWEVENT) {
      switch (event.window.event) {
        case SDL_WINDOWEVENT_RESIZED:
          glViewport(0, 0, event.window.data1, event.window.data2);
          break;
        case SDL_WINDOWEVENT_SIZE_CHANGED:
          glViewport(0, 0, event.window.data1, event.window.data2);
          break;
      }
    }
    if (event.type == SDL_MOUSEMOTION) {
      MouseX = event.motion.x;
      MouseY = event.motion.y;
    }
  }
}

bool Display::IsKeyDown(int key) {
  const Uint8* keys = SDL_GetKeyboardState(NULL);
  return keys[key];
}

void Display::getDT() {
  GLfloat currentTime = (float)SDL_GetTicks()/1000.0f;
  this->DT = currentTime - this->LastTime;
  this->LastTime = currentTime;
}

void Display::GetMousePos(double &mx, double &my) {
  mx = MouseX;
  my = MouseY;
}

GLfloat Display::getFPS() {
  GLuint currentFrame = this->Frame;
  GLuint deltaFrame = currentFrame - this->LastFrame;
  this->LastFrame = currentFrame;

  GLfloat currentTime = (float)SDL_GetTicks()/1000.0f;
  GLfloat deltaTime = currentTime - this->LastFPSCheck;
  this->LastFPSCheck = currentTime;

  return deltaFrame / deltaTime;
}

// Adjust viewport proportions on fullscreen to match 16:9 proportions
void Display::checkViewportProportions() {
  int width, height;
  int vpcoords[4];
  glGetIntegerv(GL_VIEWPORT, vpcoords);

  width = vpcoords[2];
  height = vpcoords[3];

  // 16/9 = 1.77777. Therefore, we check if the new proportions are greater or
  // lower than that
  if (width / (float)height > 1.78f) { // Height is max and width is adjusted
    int newwidth = height * 1.77777f;
    int left = width - newwidth;
    std::cout << newwidth << std::endl;
    glViewport(left / 2, 0, newwidth, height);
  } else if (width / (float)height <
             1.77f) { // Width is max and height is adjusted
    int newheight = (int)width / 1.77f;
    int left = height - newheight;
    std::cout << newheight << std::endl;
    glViewport(0, left / 2, width, newheight);
  }
}
} // namespace graphics
} // namespace stella
