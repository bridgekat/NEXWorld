#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <string>
#include <SDL2/SDL_image.h>
#include <logger.h>
#include "opengl.h"

// RGB/RGBA texture image, pixels aligned
class TextureImage {
public:
	TextureImage(int width, int height, int bytesPerPixel):
		mWidth(width), mHeight(height), mBytesPerPixel(bytesPerPixel), mPitch(alignedPitch(mWidth * bytesPerPixel)),
		mData(new unsigned char[mHeight * mPitch]), mLoaded(true) {
		memset(mData, 0, mHeight * mPitch * sizeof(unsigned char));
	}
	TextureImage(TextureImage&& r):
		mWidth(r.mWidth), mHeight(r.mHeight), mBytesPerPixel(r.mBytesPerPixel), mPitch(r.mPitch), mLoaded(r.mLoaded) {
		std::swap(mData, r.mData);
	}
	TextureImage(const std::string& filename): mLoaded(false) { loadFromPNG(filename); }
	~TextureImage() { if (mData != nullptr) delete[] mData; }

	int width() const { return mWidth; }
	int height() const { return mHeight; }
	int pitch() const { return mPitch; }
	int bytesPerPixel() const { return mBytesPerPixel; }
	const unsigned char* data() const { return mData; }
	bool loaded() const { return mLoaded; }

	void copyFrom(const TextureImage& src, int x, int y, int srcx = 0, int srcy = 0);

	TextureImage convert(int bytesPerPixel) const;
	TextureImage enlarge(int scale) const;
	TextureImage shrink(int scale) const;
	TextureImage resample(int width, int height) const;

	static int alignedPitch(int pitch, int align = 4) {
		if (pitch % align == 0) return pitch;
		return pitch + align - pitch % align;
	}

private:
	int mWidth = 0, mHeight = 0, mPitch = 0, mBytesPerPixel = 0;
	bool mLoaded = false;
	unsigned char* mData = nullptr;

	void loadFromPNG(const std::string& filename, bool checkSize = true);
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
