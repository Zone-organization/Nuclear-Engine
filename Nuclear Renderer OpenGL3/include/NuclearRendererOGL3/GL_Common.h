#define GLEW_STATIC
#include <GLEW\include\glew.h>
#include <GLEW\include\wglew.h>
#include <gl\gl.h>
#include <gl\glu.h>

#ifdef NUCLEARRENDEREROPENGL3_EXPORTS
#define NROGL3API __declspec(dllexport)
#else
#define NROGL3API __declspec(dllimport)
#endif