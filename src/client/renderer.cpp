#include "renderer.h"
#include <sstream>

int Renderer::matrixMode = 0;
Mat4f Renderer::mProjection(1.0f), Renderer::mModelview(1.0f);

void Renderer::init() {
	glShadeModel(GL_SMOOTH);
	glDisable(GL_DITHER);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	setClearColor(Vec3f(0.0f, 0.0f, 0.0f));
	setClearDepth(1.0f);
	clear();

	enableDepthTest();
}

void Renderer::checkError() {
	GLenum err = glGetError();
	if (err) {
		std::stringstream ss("OpenGL error ");
		ss << err;
		LogWarning(ss.str());
	}
}
