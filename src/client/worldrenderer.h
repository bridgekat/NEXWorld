#ifndef WORLDRENDERER_H_
#define WORLDRENDERER_H_

#include <world.h>
#include <config.h>
#include "vertexarray.h"
#include <arraymap.h>

class WorldRenderer {
public:
	WorldRenderer(const World& world, int radius, const Vec3i& centerPos):
		mWorld(world), mRadius(radius), mCenterPos(centerPos), mChunkVBOs(mRadius * 2, mCenterPos) {
		mMaxUpdatesPerFrame = Config::getInt("WorldRenderer.MaxUpdatesPerFrame", 4);
	}

	void update();
	size_t render(const Vec3d& relativePos) const;

	void moveCenter(const Vec3i&) {

	}

private:
	const World& mWorld;
	int mRadius;
	Vec3i mCenterPos;
	ArrayMap<VertexBuffer*> mChunkVBOs;

	size_t mMaxUpdatesPerFrame;
};

#endif
