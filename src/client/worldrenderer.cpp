#include "worldrenderer.h"
#include <limits.h>
#include "chunkrenderer.h"

void WorldRenderer::update() {
	std::vector<const Chunk*> res = mWorld.filterChunks([&](const Chunk* curr) {
		int res = INT_MAX;
		if (mCenterPos.inRange(mRadius, curr->pos()) && curr->ready() && curr->updated()) {
			bool f = true;
			for (int x = -1; x <= 1; x++)
				for (int y = -1; y <= 1; y++)
					for (int z = -1; z <= 1; z++)
						if (!mWorld.chunkExists(curr->pos() + Vec3i(x, y, z))) {
							f = false;
							break;
						}
			if (f) res = (mCenterPos - curr->pos()).lengthSqr();
		}
		return res;
	}, mMaxUpdatesPerFrame);
	for (auto& it: res) {
		if (mCenterPos.inRange(mRadius, it->pos()) && it->ready() && it->updated()) {
			auto buildResult = ChunkRenderer(mWorld, it->pos()).buildRender();
			if (buildResult.first) {
				if (!buildResult.second.empty()) {
					VertexBuffer* p = new VertexBuffer(std::move(buildResult.second));
					VertexBuffer* prev = mChunkVBOs.get(it->pos());
					mChunkVBOs.set(it->pos(), p);
					if (prev != nullptr) prev->destroy();
				}
				mWorld.clearUpdated(it->pos());
			}
		}
	}
}

size_t WorldRenderer::render(const Vec3d& relativePos) const {
	size_t res = 0;
	Vec3i::range(mCenterPos - mRadius, mCenterPos + mRadius, [this, &res, &relativePos](const Vec3i& pos) {
		if (mCenterPos.inRange(mRadius, pos)) {
			const VertexBuffer* curr = mChunkVBOs.get(pos);
			if (curr != nullptr && !curr->empty()) {
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
