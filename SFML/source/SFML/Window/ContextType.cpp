#include "SFML\Window\ContextType.h"

static bool OpenGLRenderAPI = false;

void sf::SetOpenGLContext(bool value)
{
	OpenGLRenderAPI = value;
}

bool sf::IsOpenGLContext()
{
	return OpenGLRenderAPI;
}
