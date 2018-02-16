#include <pluginapi.h>
#include "block.h"
#include "worldgen.h"
#include "blockrenderer.h"

NWplugindata data;

const NWplugindata* NWAPICALL Info() {
	data.pluginType = NWPluginTypeShared;
	data.internalName = "infinideas.nexworldgame";
	data.pluginName = "NEXWorldGame";
	data.authorName = "INFINIDEAS";
	return &data;
}

int NWAPICALL PreInit() {
	nwLog("PreInit!");
	nwRegisterChunkGenerator(&WorldGen::generator);
	Block::registerBlocks();
	BlockRenderer::registerTextures();
	return 0;
}

int NWAPICALL PostInit() {
	nwLog("PostInit!");
	Block::initIDs();
	BlockRenderer::registerBlockModels();
	return 0;
}
