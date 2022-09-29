#include "worldrenderer.h"
#include <limits.h>
#include "chunkrenderer.h"
#include "vertexarray.h"

void WorldRenderer::update() {
	std::vector<const Chunk*> res = mWorld.filterChunks([&](const Chunk* curr) {
		int res = INT_MAX;
		if (mCenterPos.inRange(mRadius, curr->pos()) && curr->ready() && curr->updatedSince(mChunkVBOs.get(curr->pos()).first)) {
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
		if (mCenterPos.inRange(mRadius, it->pos()) && it->ready() && it->updatedSince(mChunkVBOs.get(it->pos()).first)) {
			auto buildResult = ChunkRenderer(mWorld, it->pos()).buildRender();
			if (buildResult.first) {
				VertexBuffer* prev = mChunkVBOs.get(it->pos()).second;
				if (prev != nullptr) prev->destroy();
				if (!buildResult.second.empty()) {
					VertexBuffer* p = new VertexBuffer(std::move(buildResult.second));
					mChunkVBOs.set(it->pos(), std::make_pair(UpdateCounter::curr(), p));
				} else mChunkVBOs.set(it->pos(), std::make_pair(UpdateCounter::curr(), nullptr));
			}
		}
	}
}

const float Vertices[36 * 3] = {
	// Right
	 1.0f, 1.0f, 1.0f,
	 1.0f,-1.0f, 1.0f,
	 1.0f, 1.0f,-1.0f,
	 1.0f,-1.0f, 1.0f,
	 1.0f,-1.0f,-1.0f,
	 1.0f, 1.0f,-1.0f,
	// Left
	-1.0f, 1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	// Top
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f, 1.0f,
	 1.0f, 1.0f,-1.0f,
	// Bottom
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	 1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	 1.0f,-1.0f,-1.0f,
	 1.0f,-1.0f, 1.0f,
	// Front
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	 1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	 1.0f,-1.0f, 1.0f,
	 1.0f, 1.0f, 1.0f,
	// Back
	 1.0f, 1.0f,-1.0f,
	 1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	 1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f
};

size_t WorldRenderer::render(const Vec3d& relativePos) const {
	if (OpenGL::coreProfile()) {
		// Draw skybox
		Renderer::shader().setUniform3f("CameraPosition", relativePos.x, relativePos.y, relativePos.z);
		Renderer::shader().setUniform1i("SkyBoxMode", 1);
		Renderer::disableDepthTest();
		Renderer::disableCullFace();
		VertexArray va(36, VertexFormat(2, 3, 0, 3));
		for (int i = 0; i < 36; i++) va.addVertex(Vertices + i * 3);
		VertexBuffer(va, false).render();
		Renderer::shader().setUniform1i("SkyBoxMode", 0);
		Renderer::enableDepthTest();
		Renderer::enableCullFace();
	}
	size_t res = 0;
	Vec3i::range(mCenterPos - mRadius, mCenterPos + mRadius, [this, &res, &relativePos](const Vec3i& pos) {
		if (mCenterPos.inRange(mRadius, pos)) {
			const VertexBuffer* curr = mChunkVBOs.get(pos).second;
			if (curr != nullptr && !curr->empty()) {
				Vec3f translation = Vec3d(pos) * Chunk::Size - relativePos;
				if (OpenGL::coreProfile()) Renderer::shader().setUniform3f("Translation", translation.x, translation.y, translation.z);
				Renderer::translate(translation);
				curr->render();
				Renderer::translate(-translation);
				res++;
			}
		}
	});
	return res;
}
