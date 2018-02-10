#include "renderer.h"

int Renderer::matrixMode = 0;

void Renderer::init()
{
	if (glewInit() != GLEW_OK) LogFatal("Failed to initialize GLEW!");
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DITHER);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}
