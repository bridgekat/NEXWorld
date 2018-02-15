#ifndef BLOCKTEXTURE_H_
#define BLOCKTEXTURE_H_

#include <vector>
#include <config.h>
#include <blocktype.h>
#include <debug.h>
#include <sstream>
#include "texture.h"

class TextureCoord {
public:
	float u0, v0, u1, v1;

	TextureCoord() = default;
	TextureCoord(float u0_, float v0_, float u1_, float v1_):
		u0(u0_), v0(v0_), u1(u1_), v1(v1_) {}
};

class BlockTexture {
public:
	static void preInit() {
		mUnitSize = Config::getInt("BlockTexture.UnitSize", 32);
		mTexCoords.clear();
		mTexImages.clear();
	}

	static unsigned int registerTexture(const std::string& filename) {
		mTexImages.push_back(TextureImage(filename).convert(4));
		return static_cast<unsigned int>(mTexImages.size() - 1);
	}

	static void postInit();

	static TextureCoord& getTextureCoord(unsigned int id) {
		if (id >= mTexCoords.size()) {
			std::stringstream ss("Unable to locate block texture with ID ");
			ss << id;
			LogWarning(ss.str());
			// TODO: add a null block texture
			return mTexCoords[0];
		}
		return mTexCoords[id];
	}

	static void bind() { mTexture.bind(); }
	static void unbind() { Texture::unbind(); }

private:
	static int mSize, mUnitSize;
	static Texture mTexture;
	static std::vector<TextureImage> mTexImages;
	static std::vector<TextureCoord> mTexCoords;
};

#endif
