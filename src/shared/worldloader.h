#ifndef WORLDLOADER_H_
#define WORLDLOADER_H_

#include "world.h"
#include <set>

class WorldLoader {
public:
	WorldLoader(int radius, const Vec3i& center, const World& world): mRadius(radius), mCenter(center), mWorld(world) {}

	// Returns nearest chunks that are not loaded
	// Returns set of pair(distSqr, chunkPos)
	std::set<std::pair<int, Vec3i> > getLoadSequence(int count) const {
		std::set<std::pair<int, Vec3i> > res;
		Vec3i::range(mCenter - Vec3i(mRadius), mCenter + Vec3i(mRadius), [&] (const Vec3i& curr) {
			if (!mWorld.chunkExists(curr)) {
				res.insert(std::make_pair((curr - mCenter).lengthSqr(), curr));
				if (res.size() > count) res.erase(*res.rbegin());
			}
		});
		return res;
	}

	void setRadius(int radius) { mRadius = radius; }
	void setCenter(const Vec3i & center) { mCenter = center; }

private:
	int mRadius;
	Vec3i mCenter;
	const World & mWorld;
};

#endif
