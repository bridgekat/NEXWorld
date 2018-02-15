#include <plugin.h>
#include <pluginapi.h>
#include <world.h>
#include <worldloader.h>
#include <updatescheduler.h>
#include "window.h"
#include "renderer.h"
#include "worldrenderer.h"
#include "gui.h"
#include "player.h"
#include "texture.h"

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
	Renderer::disableTexture2D();
	form.render(GUI::Point2D(win.getWidth(), win.getHeight()), GUI::Point2D(0, 0));
	Renderer::enableTexture2D();
	Renderer::disableDepthOverwrite();
}

class orld {
public:
	orld() {
		LogInfo(LogInfoString);
		nwLog("gg");

		// Test config
		Config::load();

		// Test window
		Window& win = Window::getDefaultWindow("", 852, 480);
		Renderer::init();

		// Test plugins
		Plugin::loadAll(PluginPath, true);
		Plugin::preInit();
		Plugin::postInit();

		// Test game
		UpdateCounter::init();
		UpdateScheduler scheduler(30);

		// Test player
		Player player;

		// Test world
		World world(5);
		WorldLoader worldLoader(world, 5, Vec3i(0, 0, 0));
		WorldRenderer worldRenderer(world, 4, Vec3i(0, 0, 0));

		// Test texture
		TextureImage image("../Textures/untitled.png");
		TextureImage image2(32, 32, 4);
		image2.copyFrom(image.convert(4), 0, 0);
		Texture tex(image2, false, -1);

		Renderer::setClearColor(Vec3f(0.7f, 0.85f, 0.95f));
		Renderer::enableTexture2D();
		tex.bind();

		win.lockCursor();

		scheduler.sync();

		while (!win.shouldQuit()) {
			Renderer::waitForComplete();
			win.swapBuffers();

			// Render
			double interp = scheduler.delta();

			Renderer::setViewport(0, 0, win.getWidth(), win.getHeight());
			Renderer::clear();

			player.updateRotation(win);
			Camera camera = player.getRelativeCamera(float(win.getWidth()), float(win.getHeight()), 1000.0f);
			Renderer::setProjection(camera.getProjectionMatrix());
			Renderer::setModelview(camera.getModelViewMatrix());
			size_t renderedChunks = worldRenderer.render(player.interpolatedPosition(interp));
			size_t loadedChunks = 0, updatedChunks = 0;

			world.iterateChunks([&loadedChunks, &updatedChunks](const Chunk* c) {
				loadedChunks++;
				if (c->ready() && c->updatedSince(UpdateCounter::curr() - 1)) updatedChunks++;
			});

			std::stringstream ss;
			ss << loadedChunks << " chunks loaded, " << updatedChunks << " chunks updated, " << renderedChunks << " chunks rendered";
			//LogVerbose(ss.str());

			//drawExampleGUI(win);

			win.pollEvents();

			// Render update
			worldRenderer.update();

			// Game update
			scheduler.refresh();
			while (!scheduler.inSync()) {
				player.update();

				Vec3i playerChunkPos = World::toChunkPos(Vec3i(player.position()));

				world.setCacheCenter(playerChunkPos);
				worldLoader.setCenter(playerChunkPos);
				worldRenderer.setCenter(playerChunkPos);

				auto loads = worldLoader.getLoadSequence();
				for (auto& it : loads) world.addChunk(it.second);
				auto unloads = worldLoader.getUnloadSequence();
				for (auto& it : unloads) world.deleteChunk(it.second);

				scheduler.increase();
			}

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
