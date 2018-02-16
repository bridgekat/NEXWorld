#include "pluginapi.h"
#include <sstream>
#include "logger.h"
#include "worldgen.h"
#include "chunk.h"
#include "blocktype.h"

// PluginAPI backend

namespace PluginAPI {
	std::string currPluginName;

	BlockData convertBlockData(const NWblockdata& src) { return BlockData(src.id, src.state); }
	NWblockdata convertBlockData(const BlockData& src) { return NWblockdata{ src.id, src.state, src.blocklight, src.skylight }; }
	BlockType convertBlockType(const NWblocktype& src) { return BlockType(src.name, src.internalName, src.hardness, src.solid, src.opaque, src.visible, src.translucent); }
}

extern "C" {

	void NWAPICALL nwLog(const char* s) {
		LogInfo("[" + PluginAPI::currPluginName + "]" + std::string(s));
	}

	int NWAPICALL nwRegisterChunkGenerator(NWchunkgenerator* const generator) {
		LogInfo("[Plugin]" + PluginAPI::currPluginName + " registered chunk generator");
		WorldGen::generator = [generator](const Vec3i& pos, BlockData* blocks) {
			NWblockdata* tmp = new NWblockdata[Chunk::Size * Chunk::Size * Chunk::Size];
			NWvec3i tmpPos{ pos.x, pos.y, pos.z };
			generator(&tmpPos, tmp);
			for (int x = 0; x < Chunk::Size; x++)
				for (int y = 0; y < Chunk::Size; y++)
					for (int z = 0; z < Chunk::Size; z++)
						blocks[Chunk::getBlockIndex(Vec3i(x, y, z))] = PluginAPI::convertBlockData(tmp[x * NWChunkSize * NWChunkSize + y * NWChunkSize + z]);
			delete[] tmp;
		};
		return 0;
	}

	void NWAPICALL nwRegisterBlockType(const NWblocktype* blockType) {
		BlockType block = PluginAPI::convertBlockType(*blockType);
		std::stringstream ss;
		ss << "[Plugin]" + PluginAPI::currPluginName << " registered block " << block.name() << "[" << block.internalName() << "] with hardness " << block.hardness() << ", attributes:";
		if (block.solid()) ss << " solid";
		if (block.translucent()) ss << " translucent";
		if (block.opaque()) ss << " opaque";
		LogInfo(ss.str());
		BlockType::registerBlock(block);
	}

	int NWAPICALL nwGetBlockIDByName(const char* name) {
		return BlockType::getIDByName(std::string(name));
	}

}
