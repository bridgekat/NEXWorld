#include "blocktexture.h"
#include <sstream>
#include <logger.h>
#include <math.h>

int BlockTexture::mSize, BlockTexture::mUnitSize;
Texture BlockTexture::mTexture;
std::vector<TextureImage> BlockTexture::mTexImages;
std::vector<TextureCoord> BlockTexture::mTexCoords;

void BlockTexture::postInit() {
	// Calculate required size
	mSize = 1;
	while (mSize * mSize < int(mTexImages.size())) mSize *= 2;
	mSize *= mUnitSize;
	if (mSize > Texture::maxSize()) {
		std::stringstream ss;
		ss << "Required block texture atlas size " << mSize << " exceeded the texture size limit " << Texture::maxSize();
		LogWarning(ss.str());
		mSize = Texture::maxSize();
	}

	// Merge textures
	int rowCount = mSize / mUnitSize;
	TextureImage atlas(mSize, mSize, 4);
	mTexCoords.clear();

	for (int i = 0; i < int(mTexImages.size()); i++) {
		if (mTexImages[i].width() > mUnitSize || mTexImages[i].height() > mUnitSize) {
			// Shrink texture to fit
			mTexImages[i] = mTexImages[i].resample(mUnitSize, mUnitSize);
			std::stringstream ss;
			ss << "Shrinking oversized texture with ID " << i;
			LogVerbose(ss.str());
		}
		int x = mUnitSize * (i % rowCount), y = mUnitSize * (i / rowCount);
		atlas.copyFrom(mTexImages[i], x, y);
		// Calculate texture coords
		TextureCoord curr(x / float(mSize), y / float(mSize), (x + mTexImages[i].width()) / float(mSize), (y + mTexImages[i].height()) / float(mSize));
		mTexCoords.push_back(curr);
	}

	// Upload texture atlas
	mTexture.load(atlas, true, int(log2(mUnitSize)));
}
