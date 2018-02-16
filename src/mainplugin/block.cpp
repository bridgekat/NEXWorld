#include "block.h"
#include <sstream>

std::vector<std::pair<std::string, NWblockid*> > Block::mIDList;

NWblockid Block::Grass, Block::Rock, Block::Dirt, Block::Sand, Block::Water;

void Block::registerBlocks() {
	// Add entries here!
	registerBlock(&Grass, "grass", "Grass", 3, true, true, false);
	registerBlock(&Rock, "rock", "Rock", 10, true, true, false);
	registerBlock(&Dirt, "dirt", "Dirt", 3, true, true, false);
	registerBlock(&Sand, "sand", "Sand", 2, true, true, false);
	registerBlock(&Water, "water", "Water", 0, false, false, true);
}

void Block::initIDs() {
	for (size_t i = 0; i < mIDList.size(); i++) {
		*mIDList[i].second = nwGetBlockIDByName(mIDList[i].first.c_str());
		if (*mIDList[i].second == -1) {
			nwLog(("Failed to add block: " + mIDList[i].first).c_str());
			*mIDList[i].second = NWAirID;
			continue;
		}
		std::stringstream ss;
		ss << "Retrieved block ID " << *mIDList[i].second << " for block " << mIDList[i].first;
		nwLog(ss.str().c_str());
	}
}

void Block::registerBlock(NWblockid* id, const std::string& internalName, const std::string& name,
						  int hardness, bool solid, bool opaque, bool translucent) {
	NWblocktype block;
	block.internalName = internalName.c_str();
	block.name = name.c_str();
	block.hardness = hardness;
	block.solid = solid;
	block.opaque = opaque;
	block.visible = true;
	block.translucent = translucent;
	nwRegisterBlockType(&block);
	mIDList.push_back(std::make_pair(internalName, id));
}
