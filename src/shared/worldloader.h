#ifndef WORLDLOADER_H_
#define WORLDLOADER_H_

#include <set>
#include "world.h"
#include "config.h"

// World loader based on player position
class WorldLoader {
public:
	WorldLoader(const World& world, int radius, const Vec3i& center): mWorld(world), mRadius(radius), mCenterPos(center) {
		mMaxLoadsPerUpdate = Config::getInt("WorldLoader.MaxLoadsPerUpdate", 4);
		mMaxUnloadsPerUpdate = Config::getInt("WorldLoader.MaxUnloadsPerUpdate", 4);
	}

	// Returns nearest chunks that are not loaded
	// Returns set of pair(distSqr, chunkPos)
	std::set<std::pair<int, Vec3i> > getLoadSequence() const {
		std::set<std::pair<int, Vec3i> > res;
		Vec3i::range(mCenterPos - mRadius, mCenterPos + mRadius, [&](const Vec3i& curr) {
			if (!mWorld.chunkExists(curr)) {
				res.insert(std::make_pair((curr - mCenterPos).lengthSqr(), curr));
				if (res.size() > mMaxLoadsPerUpdate) res.erase(*res.rbegin());
			}
		});
		return res;
	}

	// Returns fartherest chunks that are loaded
	// Returns set of pair(distSqr, chunkPos)
	std::set<std::pair<int, Vec3i>, std::greater<> > getUnloadSequence() const {
		std::set<std::pair<int, Vec3i>, std::greater<> > res;
		mWorld.iterateChunks([&](const Chunk* curr) {
			if (!mCenterPos.inRange(mRadius, curr->pos())) {
				res.insert(std::make_pair((curr->pos() - mCenterPos).lengthSqr(), curr->pos()));
				if (res.size() > mMaxUnloadsPerUpdate) res.erase(*res.rbegin());
			}
		});
		return res;
	}

	void setRadius(int radius) { mRadius = radius; }
	void setCenter(const Vec3i & center) { mCenterPos = center; }

private:
	const World& mWorld;
	int mRadius;
	Vec3i mCenterPos;
	size_t mMaxLoadsPerUpdate, mMaxUnloadsPerUpdate;
};

#endif
