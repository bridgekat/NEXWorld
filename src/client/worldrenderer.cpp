#include "worldrenderer.h"
#include <limits.h>
#include "chunkrenderer.h"

void WorldRenderer::update() {
	std::vector<const Chunk*> res = mWorld.filterChunks([&](const Chunk* curr) {
		int res = INT_MAX;
		if (curr->ready() && curr->updated()) res = (mCenterPos - curr->pos()).lengthSqr();
		return res;
	}, mMaxUpdatesPerFrame);
	for (auto& it: res) {
		if (mCenterPos.chebyshevDistance(it->pos()) <= mRadius) {
			mChunkVBOs[it->pos()] = ChunkRenderer(mWorld, it->pos()).buildRender();
		}
	}
}

size_t WorldRenderer::render(const Vec3d& relativePos) const {
	size_t res = 0;
	for (auto& it: mChunkVBOs) {
		if (mCenterPos.chebyshevDistance(it.first) <= mRadius) {
			Vec3f translation = Vec3d(it.first) * Chunk::Size - relativePos;
			Renderer::translate(translation);
			it.second.render();
			Renderer::translate(-translation);
			res++;
		}
	}
	return res;
}
