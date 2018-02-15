#include "blocktype.h"
#include <sstream>
#include "logger.h"
#include "debug.h"

std::vector<BlockType> BlockType::mBlockTypes;

int BlockType::getIDByName(const std::string& internalName) {
	int first = 0, last = int(mBlockTypes.size()) - 1;
	while (first <= last) {
		int mid = (first + last) / 2;
		if (mBlockTypes[mid].mInternalName < internalName) first = mid + 1;
		else last = mid - 1;
	}
	return mBlockTypes[first].mInternalName == internalName ? first : -1;
}

const BlockType& BlockType::getBlockType(int id) {
	if (id < 0 || id >= int(mBlockTypes.size())) {
		std::stringstream ss("Failed finding block type with ID ");
		ss << id << ": out of ID range";
		LogWarning(ss.str());
		Assert(false);
		return BlockType();
	}
	return mBlockTypes[id];
}
