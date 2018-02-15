#ifndef BLOCKTEXTURE_H_
#define BLOCKTEXTURE_H_

#include <vector>
#include <config.h>
#include "texture.h"

class BlockTexture {
public:
	void preInit() {
		mUnitSize = Config::getInt("BlockTexture.UnitSize", 64);
		mTexImages.clear();
	}



private:
	static int mSize, mUnitSize;
	static std::vector<TextureImage> mTexImages;
};

#endif
