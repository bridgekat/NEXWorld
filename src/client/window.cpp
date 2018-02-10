#include "window.h"
#include <sstream>
#include "opengl.h"
#include <logger.h>
#include <debug.h>
#include <common.h>

// OpenGL debug callback
void GLEWAPIENTRY glDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* msg, const void* data) {
	if (severity != GL_DEBUG_SEVERITY_NOTIFICATION) {
		std::stringstream ss("OpenGL debug: ");
		ss << std::string(msg);
		LogVerbose(ss.str());
	}
}

Window::Window(const std::string& title, int width, int height):
	mTitle(title), mWidth(width), mHeight(height) {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
	SDL_GL_SetSwapInterval(0);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

#ifdef NEXWORLD_DEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	mWindow = SDL_CreateWindow(mTitle.c_str(), 100, 100, mWidth, mHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if (mWindow == nullptr) {
		LogFatal("Failed to create SDL window!");
		Assert(false);
	}

	mContext = SDL_GL_CreateContext(mWindow);
	makeCurrentDraw();

	if (glewInit() != GLEW_OK) {
		LogFatal("Failed to initialize GLEW!");
		Assert(false);
	}

#ifdef NEXWORLD_DEBUG
	if (GLEW_ARB_debug_output) {
		LogInfo("GL_ARB_debug_output enabled.");
		glDebugMessageCallbackARB(&glDebugCallback, nullptr);
	}
#endif
}

Window::~Window() {
	SDL_DestroyWindow(mWindow);
	SDL_GL_DeleteContext(mContext);
	SDL_Quit();
}
