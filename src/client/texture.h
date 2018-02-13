#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>
#include <SDL2/SDL_image.h>
#include <logger.h>
#include "opengl.h"

// RGBA texture image
class TextureImage {
public:
	TextureImage() {}
	TextureImage(const TextureImage&) = delete;
	TextureImage(const std::string& filename): TextureImage() { loadFromPNG(filename); }
	~TextureImage() { if (mData != nullptr) delete[] mData; }

	int width() const { return mWidth; }
	int height() const { return mHeight; }
	int pitch() const { return mPitch; }
	int bytesPerPixel() const { return mBytesPerPixel; }
	const unsigned char* data() const { return mData; }
	bool loaded() const { return mLoaded; }

private:
	int mWidth = 0, mHeight = 0, mPitch = 0, mBytesPerPixel = 0;
	bool mLoaded = false;
	unsigned char* mData = nullptr;

	void loadFromPNG(const std::string& filename);
};

class Texture {
public:
	Texture(const TextureImage& image, bool alpha = false, int maxLevels = -1);

	TextureID id() const { return mID; }
	void bind() const { glBindTexture(GL_TEXTURE_2D, mID); }

private:
	TextureID mID = 0;
};

#endif
