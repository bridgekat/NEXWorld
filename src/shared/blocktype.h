#ifndef BLOCKTYPE_H_
#define BLOCKTYPE_H_

#include <string>
#include <vector>
#include <algorithm>

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
	bool operator< (const BlockType& r) const { return mInternalName < r.mInternalName; }

	// Static part: BlockType container

	static void preInit() { mBlockTypes.clear(); }
	static void registerBlock(const BlockType& blockType) { mBlockTypes.push_back(blockType); }
	static void postInit() { std::sort(mBlockTypes.begin(), mBlockTypes.end()); }

	static int getIDByName(const std::string& internalName);
	static const BlockType& getBlockType(int id);

private:
	std::string mName, mInternalName;
	bool mSolid, mTranslucent, mOpaque;
	int mHardness;

	static std::vector<BlockType> mBlockTypes;
};

#endif
