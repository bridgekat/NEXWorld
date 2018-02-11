#ifndef CHUNK_H_
#define CHUNK_H_

#include "debug.h"
#include "vec.h"
#include "blockdata.h"
#include "worldgen.h"

class Chunk {
public:
	static constexpr int SizeLog2 = 4;
	static constexpr int Size = 1 << SizeLog2;

	Chunk(const Vec3i& pos): mPos(pos), mReady(false) {}

	Vec3i pos() const { return mPos; }
	bool ready() const { return mReady; }

	void genTerrain() {
		// TODO: invoke WorldGen::generator
		Vec3i::range(0, Size, [&](const Vec3i& pos) {
			if (mPos.y >= 0 || (pos - Vec3i(7, 7, 7)).lengthSqr() > 6 * 6) mBlocks[getBlockIndex(pos)] = BlockData(0);
			else mBlocks[getBlockIndex(pos)] = BlockData(1);
		});
		mReady = true;
	}

	BlockData getBlock(const Vec3i& pos) const {
		Assert(mReady);
		return mBlocks[getBlockIndex(pos)];
	}

private:
	Vec3i mPos;
	bool mReady;
	BlockData mBlocks[Size * Size * Size];

	static size_t getBlockIndex(const Vec3i& pos) {
		Assert(pos.x >= 0 && pos.x < Size && pos.y >= 0 && pos.y < Size && pos.z >= 0 && pos.z < Size);
		return pos.y * Size * Size + pos.x * Size + pos.z;
	}
};

#endif
