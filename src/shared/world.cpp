#include "world.h"

#include <sstream>
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
