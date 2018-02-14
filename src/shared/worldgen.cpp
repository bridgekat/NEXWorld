#include "worldgen.h"
#include "chunk.h"

std::function<void(const Vec3i&, BlockData*)> WorldGen::generator = [](const Vec3i&, BlockData* blocks) {
	for (int x = 0; x < Chunk::Size; x++)
		for (int z = 0; z < Chunk::Size; z++)
			for (int y = 0; y < Chunk::Size; y++)
				blocks[x*Chunk::Size*Chunk::Size + y * Chunk::Size + z] = BlockData(0, 0);
};
