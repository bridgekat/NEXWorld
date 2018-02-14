#ifndef BLOCKTYPE_H_
#define BLOCKTYPE_H_

#include <string>

class BlockType {
public:
	BlockType() = default;
	BlockType(const std::string& name, const std::string& internalName, bool solid, bool translucent, bool opaque, int hardness):
		mName(name), mInternalName(internalName), mSolid(solid), mTranslucent(translucent), mOpaque(opaque), mHardness(hardness) {}

	std::string name() { return mName; }
	std::string internalName() { return mInternalName; }
	bool solid() { return mSolid; }
	bool translucent() { return mTranslucent; }
	bool opaque() { return mOpaque; }
	int hardness() { return mHardness; }

private:
	std::string mName, mInternalName;
	bool mSolid, mTranslucent, mOpaque;
	int mHardness;
};

#endif
