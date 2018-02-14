#ifndef CHUNK_H_
#define CHUNK_H_

#include "debug.h"
#include "vec.h"
#include "blockdata.h"
#include "worldgen.h"
#include "updatecounter.h"

class Chunk {
public:
	static constexpr int SizeLog2 = 5;
	static constexpr int Size = 1 << SizeLog2;

	Chunk(const Vec3i& pos): mPos(pos), mReady(false), mUpdated(false) {}

	Vec3i pos() const { return mPos; }
	bool ready() const { return mReady; }
	bool updatedSince(UpdateCount updateCount) const { return mUpdated > updateCount; }
	void setUpdated() { mUpdated = UpdateCounter::curr(); }

	void genTerrain() {
		WorldGen::generator(mPos, mBlocks);
		mReady = true;
		mUpdated = UpdateCounter::curr();
	}

	BlockData getBlock(const Vec3i& pos) const {
		Assert(mReady);
		return mBlocks[getBlockIndex(pos)];
	}

	static size_t getBlockIndex(const Vec3i& pos) {
		Assert(pos.x >= 0 && pos.x < Size && pos.y >= 0 && pos.y < Size && pos.z >= 0 && pos.z < Size);
		return pos.y * Size * Size + pos.x * Size + pos.z;
	}

private:
	Vec3i mPos;
	bool mReady;
	UpdateCount mUpdated;
	BlockData mBlocks[Size * Size * Size];
};

#endif
