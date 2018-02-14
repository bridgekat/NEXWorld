#ifndef CHUNKRENDERER_H_
#define CHUNKRENDERER_H_

#include <utility>
#include "world.h"
#include "renderer.h"

class ChunkRenderer {
public:
	ChunkRenderer(const World& world, const Vec3i& pos): mWorld(world), mPos(pos) {}

	// Returns (success, vbo)
	std::pair<bool, VertexBuffer> buildRender();

private:
	const World& mWorld;
	Vec3i mPos;
};

#endif
