#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
#include <unordered_map>
#include <functional>
#include "common.h"
#include "vec.h"
#include "chunk.h"
#include "logger.h"

class World {
public:
#ifndef NEXWORLD_COMPILER_RSHIFT_ARITH
	static int toChunkPos(int pos) {
		int res = pos / Chunk::Size;
		return res >= 0 ? res : res + Chunk::Size;
	}
#else // Use arithmetic shift right
	static int toChunkPos(int pos) { return pos >> Chunk::SizeLog2; }
#endif
	static int toBlockPos(int pos) { return pos & (Chunk::Size - 1); }
	static Vec3i toChunkPos(const Vec3i& pos) { return Vec3i(toChunkPos(pos.x), toChunkPos(pos.y), toChunkPos(pos.z)); }
	static Vec3i toBlockPos(const Vec3i& pos) { return Vec3i(toBlockPos(pos.x), toBlockPos(pos.y), toBlockPos(pos.z)); }

	void addChunk(const Vec3i& chunkPos);
	void deleteChunk(const Vec3i& chunkPos);

	bool chunkExists(const Vec3i& chunkPos) const { return getChunkPtr(chunkPos) != nullptr; }
	bool chunkReady(const Vec3i& chunkPos) const { return chunkExists(chunkPos) && getChunkPtr(chunkPos)->ready(); }
	// Returns the first `count` chunks with the least weight
	std::vector<const Chunk*> filterChunks(const std::function<int(const Chunk*)>& getWeight, size_t count) const;
	void clearUpdated() {
		for (auto& it: mChunks) it.second->clearUpdated();
	}

	BlockData getBlock(const Vec3i& pos) const {
		const Chunk* p = getChunkPtr(toChunkPos(pos));
		if (p == nullptr || !p->ready()) return BlockData(0);
		return p->getBlock(toBlockPos(pos));
	}

	// Be careful! Return value might be nullptr.
	const Chunk* getChunkPtr(const Vec3i& chunkPos) const;

private:
	std::unordered_map<Vec3i, Chunk*> mChunks;

	Chunk* getChunkPtr(const Vec3i& chunkPos);
};

#endif
