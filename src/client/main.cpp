#include <world.h>
#include <worldloader.h>
#include "window.h"
#include "renderer.h"
#include "worldrenderer.h"
#include "gui.h"
#include "player.h"

void drawExampleGUI(const Window& win) {
	GUI::Form form;
	GUI::Area area(GUI::Position(GUI::Point2D(0.5f, 0.0f), GUI::Point2D(-250, 0)), GUI::Position(GUI::Point2D(0.5f, 1.0f), GUI::Point2D(+250, 0)));
	GUI::Button button0(GUI::Position(GUI::Point2D(0.0f, 0.0f), GUI::Point2D(+40, +40)), GUI::Position(GUI::Point2D(1.0f, 0.0f), GUI::Point2D(-40, +240)));
	GUI::Button button1(GUI::Position(GUI::Point2D(0.0f, 0.0f), GUI::Point2D(+20, +300)), GUI::Position(GUI::Point2D(1.0f, 0.0f), GUI::Point2D(-20, +350)));
	GUI::Button button2(GUI::Position(GUI::Point2D(0.0f, 0.0f), GUI::Point2D(+20, +360)), GUI::Position(GUI::Point2D(0.5f, 0.0f), GUI::Point2D(-5, +410)));
	GUI::Button button3(GUI::Position(GUI::Point2D(0.5f, 0.0f), GUI::Point2D(+5, +360)), GUI::Position(GUI::Point2D(1.0f, 0.0f), GUI::Point2D(-20, +410)));
	form.addChild(&area);
	area.addChild({ &button0, &button1, &button2, &button3 });

	Renderer::restoreProjection();
	Renderer::restoreModelview();
	Renderer::applyOrtho(0.0f, float(win.getWidth()), 0.0f, float(win.getHeight()), -1.0f, 1.0f);
	Renderer::enableDepthOverwrite();
	form.render(GUI::Point2D(win.getWidth(), win.getHeight()), GUI::Point2D(0, 0));
	Renderer::disableDepthOverwrite();
}

class orld {
public:
	orld() {
		// Meaningless helloworld
		LogVerbose("Hello, world!");

		// Test config
		Config::load();

		// Test window
		Window& win = Window::getDefaultWindow("Test", 852, 480);
		Renderer::init();

		// Test player
		Player player;

		// Test world
		World world;
		WorldLoader loader(world, 16, Vec3i(1, 1, 1));
		WorldRenderer worldRenderer(world, 16, Vec3i(1, 1, 1));

		world.addChunk(Vec3i(0, -1, 0));

		win.lockCursor();

		while (!win.shouldQuit()) {
			Renderer::waitForComplete();
			win.swapBuffers();

			Renderer::setViewport(0, 0, win.getWidth(), win.getHeight());
			Renderer::clear();

			Camera camera = player.getRelativeCamera(float(win.getWidth()), float(win.getHeight()), 1000.0f);
			Renderer::setProjection(camera.getProjectionMatrix());
			Renderer::setModelview(camera.getModelViewMatrix());
			worldRenderer.render(player.position());

			//drawExampleGUI(win);

			win.pollEvents();

			player.update(win);
			worldRenderer.update();
			world.clearUpdated();
			std::set<std::pair<int, Vec3i> > res = loader.getLoadSequence();
			for (auto& it: res) world.addChunk(it.second);

			if (Window::isKeyPressed(SDL_SCANCODE_ESCAPE)) break;
		}

		win.unlockCursor();

		Config::save();
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
