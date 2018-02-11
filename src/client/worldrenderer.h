#ifndef WORLDRENDERER_H_
#define WORLDRENDERER_H_

#include "world.h"
#include "vertexarray.h"

class WorldRenderer {
public:


private:
	std::unordered_map<Vec3i, VertexBuffer> mChunkVBOs;
};

#endif
