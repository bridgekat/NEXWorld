#ifndef BLOCKRENDERER_H_
#define BLOCKRENDERER_H_

#include <pluginapi.h>
#include "block.h"

class BlockRenderer {
public:
	static NWtextureid GrassTop, GrassSide, Dirt, Rock, Sand, Water;

	static void registerTextures();
	static void bindFullBlockModel(NWblockid id, NWtextureid tex[6]);
	static void registerBlockModels();
};

#endif
