#include "pluginapi.h"
#include "logger.h"
#include "worldgen.h"
#include "chunk.h"
#include "blocktype.h"

// PluginAPI backend

namespace PluginAPI {
	std::string currPluginName;

	BlockData convertBlockData(const NWblockdata& src) { return BlockData(src.id, src.state); }
	NWblockdata convertBlockData(const BlockData& src) { return NWblockdata{ src.id, src.state, 0, 0 }; }
	BlockType convertBlockType(const NWblocktype& src) { return BlockType(src.name, src.internalName, src.solid, src.translucent, src.opaque, src.hardness); }
}

extern "C" {

	NWAPIEXPORT void NWAPICALL nwLog(const char* s) {
		LogInfo("[" + PluginAPI::currPluginName + "]" + std::string(s));
	}

	NWAPIEXPORT int NWAPICALL nwRegisterChunkGenerator(NWchunkgenerator* const generator) {
		LogInfo("[Plugin]" + PluginAPI::currPluginName + " Registered chunk generator");
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

}
