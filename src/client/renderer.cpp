#include "renderer.h"
#include <sstream>
#include <common.h>

int Renderer::matrixMode = 0;
Mat4f Renderer::mProjection(1.0f), Renderer::mModelview(1.0f);
ShaderProgram Renderer::mFinal;

void Renderer::init() {
	glShadeModel(GL_SMOOTH);
	glDisable(GL_DITHER);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glDepthFunc(GL_LEQUAL);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (!OpenGL::coreProfile()) {
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.001f);
	} else {
		mFinal.loadShadersFromFile(std::string(ShaderPath) + "Final.vsh", std::string(ShaderPath) + "Final.fsh");
		mFinal.bind();
		mFinal.setUniform1i("Texture", 0);
	}

	setClearColor(Vec3f(0.0f, 0.0f, 0.0f));
	setClearDepth(1.0f);
	clear();

	enableCullFace();
	enableDepthTest();
}

void Renderer::checkError() {
	GLenum err = glGetError();
	if (err) {
		std::stringstream ss;
		ss << "OpenGL error " << err;
		LogWarning(ss.str());
	}
}
