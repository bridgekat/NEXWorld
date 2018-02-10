#ifndef WINDOW_H_
#define WINDOW_H_

#include <string>
#include <SDL2/SDL.h>

class Window {
public:
	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void makeCurrentDraw() const { SDL_GL_MakeCurrent(mWindow, mContext); }
	void swapBuffers() const { SDL_GL_SwapWindow(mWindow); }

	static const Uint8* getKeyBoardState() { return SDL_GetKeyboardState(nullptr); }

	int getWidth() const { return mWidth; }
	int getHeight() const { return mHeight; }

	void pollEvents() {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				mShouldQuit = true;
				break;
			case SDL_WINDOWEVENT:
				switch (e.window.event) {
				case SDL_WINDOWEVENT_RESIZED:
				case SDL_WINDOWEVENT_SIZE_CHANGED:
					mWidth = e.window.data1;
					mHeight = e.window.data2;
					break;
				}
				break;
			}
		}
	}

	static Window& getInstance(const std::string& title="", int width=0, int height=0) {
		static Window win(title, width, height);
		return win;
	}

	bool shouldQuit() const { return mShouldQuit; }

private:
	SDL_Window* mWindow = nullptr;
	std::string mTitle;
	int mWidth, mHeight;
	bool mShouldQuit = false;

	Window(const std::string& title, int width, int height);
	~Window();

	SDL_GLContext mContext;
};

#endif
