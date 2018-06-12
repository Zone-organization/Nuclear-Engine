#pragma once

#include <string>
#include <array>

#include <SDL2/SDL_timer.h>
#include <glm/glm.hpp>

#include "stella/graphics/debug_gui.h"

// Forward declarations
struct SDL_Window;
typedef unsigned int GLuint;
typedef float GLfloat;

namespace stella {
namespace graphics {
class Display {
public:
  Display(GLuint width, GLuint height, const std::string &title);
  ~Display();
  bool IsRunning();
  void Update();
  void Clear();
  void SetClearColor(int r, int g, int b);
  void SetClearColor(GLfloat x, GLfloat y, GLfloat z);
  GLuint GetWidth();
  GLuint GetHeight();
  GLfloat GetTime() { return (GLfloat)SDL_GetTicks(); }
  GLuint GetFrame() { return Frame; }
  GLfloat GetDT() { return DT; }
  GLfloat getFPS();
  void GetMousePos(double &mx, double &my);
	bool IsKeyDown(int key);

private:
  GLuint Width, Height, Frame, LastFrame;

  GLfloat LastTime, LastFPSCheck, DT;
  std::string Title;
  SDL_Window *Window;
  bool Running;
  glm::vec3 ClearColor;

  DebugGui DGUI;

  void updateInput();
  void getDT();
  void checkViewportProportions();
};
} // namespace graphics
} // namespace stella
