#ifndef WORLDLOADER_H_
#define WORLDLOADER_H_

#include <set>
#include "world.h"
#include "config.h"

class WorldLoader {
public:
	WorldLoader(const World& world, int radius, const Vec3i& center): mWorld(world), mRadius(radius), mCenter(center) {
		mMaxLoadsPerUpdate = Config::getInt("WorldLoader.MaxLoadsPerUpdate", 4);
	}

	// Returns nearest chunks that are not loaded
	// Returns set of pair(distSqr, chunkPos)
	std::set<std::pair<int, Vec3i> > getLoadSequence() const {
		std::set<std::pair<int, Vec3i> > res;
		Vec3i::range(mCenter - mRadius, mCenter + mRadius, [&](const Vec3i& curr) {
			if (!mWorld.chunkExists(curr)) {
				res.insert(std::make_pair((curr - mCenter).lengthSqr(), curr));
				if (res.size() > mMaxLoadsPerUpdate) res.erase(*res.rbegin());
			}
		});
		return res;
	}

	void setRadius(int radius) { mRadius = radius; }
	void setCenter(const Vec3i & center) { mCenter = center; }

private:
	const World& mWorld;
	int mRadius;
	Vec3i mCenter;
	size_t mMaxLoadsPerUpdate;
};

#endif
