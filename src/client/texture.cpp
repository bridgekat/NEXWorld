#include "texture.h"

#include <fstream>
#include <math.h>
#include <common.h>
#include <debug.h>

inline int alignedPitch(int pitch, int align = 4) {
	if (pitch % align == 0) return pitch;
	return pitch + align - pitch % align;
}

void TextureImage::loadFromPNG(const std::string& filename) {
	SDL_Surface* surface = IMG_LoadPNG_RW(SDL_RWFromFile(filename.c_str(), "rb"));

	if (surface == nullptr) {
		LogWarning("Failed to load file \"" + filename + "\" as PNG image: " + IMG_GetError());
		return;
	}

	if (surface->format->BytesPerPixel != 3 && surface->format->BytesPerPixel != 4) {
		LogWarning("Failed to load file \"" + filename + "\" as PNG image: unsupported format (only RGB/RGBA is supported)");
		return;
	}

	mWidth = surface->w;
	mHeight = surface->h;
	mBytesPerPixel = surface->format->BytesPerPixel;
	mPitch = alignedPitch(mWidth * mBytesPerPixel);

	mData = new unsigned char[mHeight * mPitch];

	for (int i = 0; i < mHeight; i++) {
		memcpy(mData + i * mPitch, reinterpret_cast<unsigned char*>(surface->pixels) + i * surface->pitch, mWidth * mBytesPerPixel);
	}

	SDL_FreeSurface(surface);
	mLoaded = true;
}

void Build2DMipmaps(TextureFormat format, int w, int h, int level, const unsigned char* src, int srcPitch, TextureFormat srcFormat) {
	int cc = (srcFormat == TextureFormatRGB ? 3 : 4);
	int sum = 0, scale = 1, pitch;

	pitch = alignedPitch(w * cc, 4);

	unsigned char *cur = new unsigned char[pitch * h];
	memset(cur, 0, pitch * h);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, level);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, level);
	glTexEnvf(GL_TEXTURE_FILTER_CONTROL, GL_TEXTURE_LOD_BIAS, 0.0f);

	for (int i = 0; i <= level; i++) {
		int curW = w / scale, curH = h / scale;
		int curPitch = alignedPitch(curW * cc, 4);

		for (int y = 0; y < curH; y++)
			for (int x = 0; x < curW; x++) {
				for (int col = 0; col < cc; col++) {
					sum = 0;
					for (int yy = 0; yy < scale; yy++) for (int xx = 0; xx < scale; xx++)
							sum += src[(y * scale + yy) * srcPitch + (x * scale + xx) * cc + col];
					cur[y * curPitch + x * cc + col] = (unsigned char)(sum / (scale * scale));
				}
			}

		glTexImage2D(GL_TEXTURE_2D, i, format, curW, curH, 0, srcFormat, GL_UNSIGNED_BYTE, cur);

		scale *= 2;
	}

	delete[] cur;
}

Texture::Texture(const TextureImage& image, bool alpha, int maxLevels) {
	Assert(image.bytesPerPixel() == 3 || image.bytesPerPixel() == 4);
	if (maxLevels < 0) maxLevels = (int)log2(image.width());
	TextureFormat srcFormat = image.bytesPerPixel() == 4 ? TextureFormatRGBA : TextureFormatRGB;
	TextureFormat dstFormat = alpha ? TextureFormatRGBA : TextureFormatRGB;
	glGenTextures(1, &mID);
	glBindTexture(GL_TEXTURE_2D, mID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	Build2DMipmaps(dstFormat, image.width(), image.height(), maxLevels, image.data(), image.pitch(), srcFormat);
}
