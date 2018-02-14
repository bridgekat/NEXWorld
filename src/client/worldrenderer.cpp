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
			VertexBuffer* vbo = new VertexBuffer(ChunkRenderer(mWorld, it->pos()).buildRender());
			Vec3i pos = it->pos();
			mChunkVBOs.set(it->pos(), vbo);
		}
	}
}

size_t WorldRenderer::render(const Vec3d& relativePos) const {
	size_t res = 0;
	Vec3i::range(-mRadius, mRadius, [this, &res, &relativePos](const Vec3i& pos) {
		if (mCenterPos.chebyshevDistance(pos) <= mRadius) {
			const VertexBuffer* curr = mChunkVBOs.get(pos);
			if (curr != nullptr) {
				Vec3f translation = Vec3d(pos) * Chunk::Size - relativePos;
				Renderer::translate(translation);
				curr->render();
				Renderer::translate(-translation);
				res++;
			}
		}
	});
	return res;
}
