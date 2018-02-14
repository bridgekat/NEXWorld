#ifndef WORLD_H_
#define WORLD_H_

#include <vector>
#include <unordered_map>
#include <functional>
#include "common.h"
#include "vec.h"
#include "chunk.h"
#include "logger.h"
#include "arraymap.h"

class World {
public:
	World(int cacheRadius = 0): mChunks(), mCachedChunkPtrs(cacheRadius * 2, Vec3i(0, 0, 0)) {}

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

	void iterateChunks(const std::function<void(const Chunk*)>& func) const { for (const Chunk* c: mChunks) func(c); }
	// Returns the first `count` chunks with the least weight
	std::vector<const Chunk*> filterChunks(const std::function<int(const Chunk*)>& getWeight, size_t count) const;

	BlockData getBlock(const Vec3i& pos) const {
		const Chunk* p = getChunkPtr(toChunkPos(pos));
		if (p == nullptr || !p->ready()) return BlockData(0);
		return p->getBlock(toBlockPos(pos));
	}

	// Be careful! Return value might be nullptr.
	const Chunk* getChunkPtr(const Vec3i& chunkPos) const;
	// Optional optimization
	void setCacheCenter(const Vec3i& centerPos) { mCachedChunkPtrs.move(centerPos); }

private:
	std::vector<Chunk*> mChunks;
	mutable ArrayMap<Chunk*> mCachedChunkPtrs;

	int binarySearch(const Vec3i& chunkPos) const;
	bool searchSuccessful(size_t result, const Vec3i& chunkPos) const {
		return result < mChunks.size() && mChunks[result]->pos() == chunkPos;
	}
	void insertEntry(size_t index) {
		mChunks.resize(mChunks.size() + 1);
		for (size_t i = mChunks.size() - 1; i > index; i--) mChunks[i] = mChunks[i - 1];
	}
	void eraseEntry(size_t index) {
		for (size_t i = index + 1; i < mChunks.size(); i++) mChunks[i - 1] = mChunks[i];
		mChunks.resize(mChunks.size() - 1);
	}
	Chunk* getChunkPtr(const Vec3i& chunkPos);
};

#endif
