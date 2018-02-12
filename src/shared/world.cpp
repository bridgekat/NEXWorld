#include "world.h"

#include <sstream>
#include <set>
#include "debug.h"
#include "logger.h"

void World::addChunk(const Vec3i& pos) {
	auto it = mChunks.find(pos);
	if (it != mChunks.end()) {
		std::stringstream ss;
		ss << "Adding chunk at (" << pos.x << ", " << pos.y << ", " << pos.z << "): chunk already exists";
		LogWarning(ss.str());
		return;
	}
	mChunks[pos] = new Chunk(pos);
	mChunks[pos]->genTerrain();
}

void World::deleteChunk(const Vec3i& pos) {
	auto it = mChunks.find(pos);
	if (it == mChunks.end()) {
		std::stringstream ss;
		ss << "Deleting chunk at (" << pos.x << ", " << pos.y << ", " << pos.z << "): chunk not exists";
		LogWarning(ss.str());
		return;
	}
	delete it->second;
	mChunks.erase(it);
}

std::vector<const Chunk*> World::filterChunks(const std::function<int(const Chunk*)>& getWeight, size_t count) const {
	std::set<std::pair<int, const Chunk*> > s;
	for (auto& it: mChunks) {
		s.insert(std::make_pair(getWeight(it.second), it.second));
		if (s.size() > count) s.erase(*s.rbegin());
	}
	std::vector<const Chunk*> res;
	for (auto& it: s) res.push_back(it.second);
	return res;
}

Chunk* World::getChunkPtr(const Vec3i& pos) {
	auto it = mChunks.find(pos);
	if (it == mChunks.end()) return nullptr;
	return it->second;
}

const Chunk* World::getChunkPtr(const Vec3i& pos) const {
	auto it = mChunks.find(pos);
	if (it == mChunks.end()) return nullptr;
	return it->second;
}
