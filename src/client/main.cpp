#include <main.h>
#include <world.h>
#include <worldloader.h>
#include "window.h"
#include "renderer.h"
#include "chunkrenderer.h"
#include "gui.h"

void drawExampleGUI(const Window& win) {
	GUI::Form form;
	GUI::Area area(GUI::Position(GUI::Point2D(0.5f, 0.0f), GUI::Point2D(-250, 0)), GUI::Position(GUI::Point2D(0.5f, 1.0f), GUI::Point2D(+250, 0)));
	GUI::Button button0(GUI::Position(GUI::Point2D(0.0f, 0.0f), GUI::Point2D(+40, +40)), GUI::Position(GUI::Point2D(1.0f, 0.0f), GUI::Point2D(-40, +240)));
	GUI::Button button1(GUI::Position(GUI::Point2D(0.0f, 0.0f), GUI::Point2D(+20, +300)), GUI::Position(GUI::Point2D(1.0f, 0.0f), GUI::Point2D(-20, +350)));
	GUI::Button button2(GUI::Position(GUI::Point2D(0.0f, 0.0f), GUI::Point2D(+20, +360)), GUI::Position(GUI::Point2D(0.5f, 0.0f), GUI::Point2D(-5, +410)));
	GUI::Button button3(GUI::Position(GUI::Point2D(0.5f, 0.0f), GUI::Point2D(+5, +360)), GUI::Position(GUI::Point2D(1.0f, 0.0f), GUI::Point2D(-20, +410)));
	form.addChild(&area);
	area.addChild({ &button0, &button1, &button2, &button3 });

	Renderer::restoreProj();
	Renderer::restoreModl();
	Renderer::applyOrtho(0.0f, float(win.getWidth()), 0.0f, float(win.getHeight()), -1.0f, 1.0f);
	Renderer::enableDepthOverwrite();
	form.render(GUI::Point2D(win.getWidth(), win.getHeight()), GUI::Point2D(0, 0));
	Renderer::disableDepthOverwrite();
}

class orld {
public:
	orld() {
		// Meaningless helloworld
		LogVerbose("Hello world from client!");
		helloworld();

		// Test window
		Window& win = Window::getInstance("Test", 852, 480);

		Renderer::init();

		// Test world
		World world;
		WorldLoader loader(world, 4, Vec3i(1, 1, 1));

		world.addChunk(Vec3i(0, -1, 0));
		VertexBuffer vb = ChunkRenderer(world, Vec3i(0, -1, 0)).buildRender();

		int cnt = 0;

		while (!win.shouldQuit()) {
			Renderer::waitForComplete();
			win.swapBuffers();

			Renderer::setViewport(0, 0, win.getWidth(), win.getHeight());
			Renderer::clear();

			Renderer::restoreProj();
			Renderer::restoreModl();
			Renderer::applyPerspective(70.0f, float(win.getWidth()) / float(win.getHeight()), 0.1f, 100.0f);
			Renderer::translate(Vec3f(0.0f, 0.0f, -32.0f));
			Renderer::rotate(float(cnt), Vec3f(1.0f, 0.0f, 0.0f));
			Renderer::rotate(float(cnt), Vec3f(0.0f, 1.0f, 0.0f));
			vb.render();

			//drawExampleGUI(win);

			win.pollEvents();

			cnt++;
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
