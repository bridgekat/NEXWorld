#include "world.h"

#include <sstream>
#include <set>
#include "debug.h"
#include "logger.h"

void World::addChunk(const Vec3i& pos) {
	size_t ind = binarySearch(pos);
	if (searchSuccessful(ind, pos)) {
		std::stringstream ss;
		ss << "Adding chunk at (" << pos.x << ", " << pos.y << ", " << pos.z << "): chunk already exists";
		LogWarning(ss.str());
		return;
	}
	insertEntry(ind);
	mChunks[ind] = new Chunk(pos);
	mChunks[ind]->genTerrain();
	mCachedChunkPtrs.set(pos, mChunks[ind]);
}

void World::deleteChunk(const Vec3i& pos) {
	size_t ind = binarySearch(pos);
	if (!searchSuccessful(ind, pos)) {
		std::stringstream ss;
		ss << "Deleting chunk at (" << pos.x << ", " << pos.y << ", " << pos.z << "): chunk not exists";
		LogWarning(ss.str());
		return;
	}
	delete mChunks[ind];
	eraseEntry(ind);
	mCachedChunkPtrs.set(pos, nullptr);
}

std::vector<const Chunk*> World::filterChunks(const std::function<int(const Chunk*)>& getWeight, size_t count) const {
	std::set<std::pair<int, const Chunk*> > s;
	for (auto& c: mChunks) {
		s.insert(std::make_pair(getWeight(c), c));
		if (s.size() > count) s.erase(*s.rbegin());
	}
	std::vector<const Chunk*> res;
	for (auto& it: s) res.push_back(it.second);
	return res;
}

int World::binarySearch(const Vec3i& chunkPos) const {
	int first = 0, last = int(mChunks.size()) - 1;
	while (first <= last) {
		int mid = (first + last) / 2;
		if (mChunks[mid]->pos() < chunkPos) first = mid + 1;
		else last = mid - 1;
	}
	return first;
}

const Chunk* World::getChunkPtr(const Vec3i& pos) const {
	const Chunk* c = mCachedChunkPtrs.get(pos);
	if (c != nullptr) return c;

	size_t ind = binarySearch(pos);
	if (!searchSuccessful(ind, pos)) return nullptr;

	mCachedChunkPtrs.set(pos, mChunks[ind]);
	return mChunks[ind];
}

Chunk* World::getChunkPtr(const Vec3i& pos) {
	Chunk* c = mCachedChunkPtrs.get(pos);
	if (c != nullptr) return c;

	size_t ind = binarySearch(pos);
	if (!searchSuccessful(ind, pos)) return nullptr;

	mCachedChunkPtrs.set(pos, mChunks[ind]);
	return mChunks[ind];
}
