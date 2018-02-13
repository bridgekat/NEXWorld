#include "texture.h"

#include <fstream>
#include <math.h>
#include <common.h>
#include <debug.h>

#pragma pack(push)
#pragma pack(1) // Avoid structure alignment

struct BitmapInfoHeader {
	int biSize = 40, biWidth, biHeight;
	short biPlanes = 1, biBitCount = 24;
	int biCompression = 0, biSizeImage = 0, biXPelsPerMeter = 3780, biYPelsPerMeter = 3780, biClrUsed = 0, biClrImportant = 0;
};

struct BitmapFileHeader {
	short bfType = 0x4D42;
	int bfSize;
	short bfReserved1 = 0, bfReserved2 = 0;
	int bfOffBits = 54;
};

#pragma pack(pop)

inline int alignedPitch(int pitch, int align) {
	if (pitch % align == 0) return pitch;
	return pitch + align - pitch % align;
}

// Convert between BGR and RGB
void Bitmap::swapRBChannels() {
	int p = 0;
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			int ind = p + j * 3;
			unsigned char t = data[ind];
			data[ind] = data[ind + 2];
			data[ind + 2] = t;
		}
		p += pitch;
	}
}

void Bitmap::load(const std::string& filename) {
	std::ifstream bmpfile(filename.c_str(), std::ios::binary | std::ios::in);
	BitmapFileHeader bfh;
	BitmapInfoHeader bih;
	bmpfile.read((char*)&bfh, sizeof(BitmapFileHeader));
	bmpfile.read((char*)&bih, sizeof(BitmapInfoHeader));
	w = bih.biWidth;
	h = bih.biHeight;
	pitch = alignedPitch(bih.biWidth * 3, 4); // Pixel alignment (4 bytes)
	data = new unsigned char[pitch * h];
	bmpfile.read((char*)data, pitch * h);
	bmpfile.close();
	swapRBChannels();
}

void Bitmap::save(const std::string& filename) {
	BitmapFileHeader bfh;
	BitmapInfoHeader bih;
	bfh.bfSize = pitch * h + 54;
	bih.biWidth = w;
	bih.biHeight = h;
	Bitmap tmp = *this;
	tmp.swapRBChannels();
	std::ofstream ofs(filename, std::ios::out | std::ios::binary);
	ofs.write((char*)&bfh, sizeof(BitmapFileHeader));
	ofs.write((char*)&bih, sizeof(BitmapInfoHeader));
	ofs.write((char*)tmp.data, pitch * h);
	ofs.close();
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
	if (maxLevels == -1) maxLevels = (int)log2(image.width());
	TextureFormat format = alpha ? TextureFormatRGBA : TextureFormatRGB;
	glGenTextures(1, &mID);
	glBindTexture(GL_TEXTURE_2D, mID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	Build2DMipmaps(format, image.width(), image.height(), maxLevels, image.data(), image.width() * 4, TextureFormatRGBA);
}

Texture::Texture(const Bitmap& image, bool alpha, int maxLevels) {
	if (maxLevels == -1) maxLevels = (int)log2(image.w);
	TextureFormat format = alpha ? TextureFormatRGBA : TextureFormatRGB;
	glGenTextures(1, &mID);
	glBindTexture(GL_TEXTURE_2D, mID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	Build2DMipmaps(format, image.w, image.h, maxLevels, image.data, image.pitch, TextureFormatRGB);
}
