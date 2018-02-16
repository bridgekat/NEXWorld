#ifndef BLOCK_H_
#define BLOCK_H_

#include <pluginapi.h>
#include <string>
#include <utility>
#include <vector>

class Block {
public:
	static NWblockid Grass, Rock, Dirt, Sand, Water;

	static void registerBlocks();
	static void initIDs();

private:
	static std::vector<std::pair<std::string, NWblockid*> > mIDList;

	static void registerBlock(NWblockid* id, const std::string& internalName, const std::string& name,
							  int hardness, bool solid, bool opaque, bool translucent);
};

#endif
