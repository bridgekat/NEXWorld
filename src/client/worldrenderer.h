#ifndef WORLDRENDERER_H_
#define WORLDRENDERER_H_

#include <world.h>
#include <config.h>
#include "vertexarray.h"
#include <arraymap.h>

class WorldRenderer {
public:
	WorldRenderer(World& world, int radius, const Vec3i& centerPos):
		mWorld(world), mRadius(radius), mCenterPos(centerPos), mChunkVBOs(mRadius * 2, mCenterPos) {
		mMaxUpdatesPerFrame = Config::getInt("WorldRenderer.MaxUpdatesPerFrame", 4);
	}

	void update();
	size_t render(const Vec3d& relativePos) const;

	void setCenter(const Vec3i& centerPos) {
		auto res = mChunkVBOs.setCenterPos(mCenterPos);
		for (auto& i: res) if (i.second != nullptr) i.second->destroy();
		mCenterPos = centerPos;
	}

private:
	World& mWorld;
	int mRadius;
	Vec3i mCenterPos;
	// TODO: change pointers to VertexBuffer objects
	ArrayMap<std::pair<UpdateCount, VertexBuffer*> > mChunkVBOs;

	size_t mMaxUpdatesPerFrame;
};

#endif
