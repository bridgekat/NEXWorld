#include "blocktype.h"
#include <sstream>
#include "logger.h"
#include "debug.h"

#include <iostream>

std::vector<BlockType> BlockType::mBlockTypes;

void BlockType::preInit() {
	mBlockTypes.clear();
	// Insert an air block
	// 道生一，一生无……
	mBlockTypes.push_back(BlockType("Air", "", 0, false, false, false, false));
}

void BlockType::postInit() {
	// Sort all blocks except air block
	// 无生有，有生万物……
	std::sort(mBlockTypes.begin() + 1, mBlockTypes.end());
}

int BlockType::getIDByName(const std::string& internalName) {
	int first = 1, last = int(mBlockTypes.size()) - 1;
	while (first <= last) {
		int mid = (first + last) / 2;
		if (mBlockTypes[mid].mInternalName < internalName) first = mid + 1;
		else last = mid - 1;
	}
	if (first >= 1 && first < int(mBlockTypes.size()) && mBlockTypes[first].mInternalName == internalName) return first;
	return -1;
}

const BlockType& BlockType::getBlockType(int id) {
	if (id < 0 || id >= int(mBlockTypes.size())) {
		std::stringstream ss("Failed finding block type with ID ");
		ss << id << ": out of ID range";
		LogWarning(ss.str());
		Assert(false);
		// TODO: add null block
		return mBlockTypes[0];
	}
	return mBlockTypes[id];
}
