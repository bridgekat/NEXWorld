#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>
#include <logger.h>
#include "opengl.h"

class Bitmap {
public:
	int w = 0, h = 0, pitch = 0;
	unsigned char* data = nullptr;

	Bitmap() {}
	Bitmap(const Bitmap& rhs) { (*this) = rhs; }
	explicit Bitmap(const std::string& filename) { load(filename); }
	~Bitmap() { if (data != 0) delete[] data; }

	Bitmap& operator=(const Bitmap& rhs) {
		w = rhs.w;
		h = rhs.h;
		pitch = rhs.pitch;
		if (data != nullptr) delete[] data;
		data = new unsigned char[h * pitch];
		memcpy(data, rhs.data, h * pitch);
		return *this;
	}

	void swapRBChannels();
	void load(const std::string& filename);
	void save(const std::string& filename);
};

// RGBA texture image
class TextureImage {
public:
	TextureImage() {}
	TextureImage(const TextureImage&) = delete;
	TextureImage(const std::string& filename): TextureImage() { loadFromPNG(filename); }
	~TextureImage() { if (mData != nullptr) delete[] mData; }

	int width() const { return mWidth; }
	int height() const { return mHeight; }
	const unsigned char* data() const { return mData; }
	bool loaded() const { return mLoaded; }

private:
	int mWidth = 0, mHeight = 0, mPitch = 0;
	bool mLoaded = false;
	unsigned char* mData = nullptr;

	void loadFromPNG(const std::string& filename);
};

class Texture {
public:
	Texture(const TextureImage& image, bool alpha = false, int maxLevels = -1);
	Texture(const Bitmap& image, bool alpha = false, int maxLevels = -1);

	TextureID id() const { return mID; }
	void bind() const { glBindTexture(GL_TEXTURE_2D, mID); }

private:
	TextureID mID = 0;
};

#endif
