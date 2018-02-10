#include <iostream>

#include "window.h"
#include "renderer.h"
#include <main.h>

class orld {
public:
	orld() {
		// Meaningless helloworld
		std::cout << "Hello world" << std::endl;
		helloworld();

		// Test window
		Window& win = Window::getInstance("Test", 852, 480);

		Renderer::init();

		while (!win.shouldQuit()) {
			Renderer::waitForComplete();
			win.swapBuffers();

			Renderer::setViewport(0, 0, win.getWidth(), win.getHeight());
			Renderer::clear();

			Renderer::restoreProj();
			Renderer::restoreModl();
			Renderer::applyPerspective(70.0f, float(win.getWidth()) / float(win.getHeight()), 0.1f, 100.0f);
			Renderer::translate(Vec3f(0.0f, 0.0f, -5.0f));

			VertexArray va(6, VertexFormat(0, 3, 0, 3));

			va.setColor({ 0.7f, 0.85f, 0.95f });
			va.addVertex({ -1.0f, -1.0f, 0.0f });
			va.addVertex({ 1.0f, -1.0f, 0.0f });
			va.addVertex({ 1.0f, 1.0f, 0.0f });
			va.addVertex({ -1.0f, -1.0f, 0.0f });
			va.addVertex({ 1.0f, 1.0f, 0.0f });
			va.addVertex({ -1.0f, 1.0f, 0.0f });

			VertexBuffer vb(va);

			vb.render();

			win.pollEvents();
		}
	}
};

// It's not convenient to #define SDL_MAIN_HANDLED wherever #include <SDL/SDL2.h> appears...
#ifdef main
#undef main
#endif

int main() {
	new orld; // Although, actually, it is NEXWorld...
	return 0;
}
