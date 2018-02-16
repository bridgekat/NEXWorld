#ifndef BLOCKTYPE_H_
#define BLOCKTYPE_H_

#include <string>
#include <vector>
#include <algorithm>

class BlockType {
public:
	BlockType() = default;
	BlockType(const std::string& name, const std::string& internalName, int hardness, bool solid, bool opaque, bool visible, bool translucent):
		mName(name), mInternalName(internalName), mHardness(hardness), mSolid(solid), mOpaque(opaque), mVisible(visible), mTranslucent(translucent) {}

	std::string name() const { return mName; }
	std::string internalName() const { return mInternalName; }
	bool solid() const { return mSolid; }
	bool translucent() const { return mTranslucent; }
	bool opaque() const { return mOpaque; }
	int hardness() const { return mHardness; }
	bool visible() const { return mVisible; }
	bool operator< (const BlockType& r) const { return mInternalName < r.mInternalName; }

	// Static part: BlockType container

	static void preInit();
	static void registerBlock(const BlockType& blockType) { mBlockTypes.push_back(blockType); }
	static void postInit();

	static int count() { return int(mBlockTypes.size()); }
	static int getIDByName(const std::string& internalName);
	static const BlockType& getBlockType(int id);

private:
	std::string mName, mInternalName;
	bool mSolid, mTranslucent, mOpaque, mVisible;
	int mHardness;

	static std::vector<BlockType> mBlockTypes;
};

#endif
