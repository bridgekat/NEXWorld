#include <pluginapi.h>
#include "worldgen.h"

NWplugindata data;

NWAPIEXPORT const NWplugindata* NWAPICALL Info() {
	data.pluginType = NWPluginTypeShared;
	data.internalName = "infinideas.nexworld.main";
	data.pluginName = "NEXWorldGame";
	data.authorName = "INFINIDEAS";
	return &data;
}

NWAPIEXPORT int NWAPICALL PreInit() {
	nwLog("PreInit!");
	nwRegisterChunkGenerator(WorldGen::generator);
	return 0;
}
