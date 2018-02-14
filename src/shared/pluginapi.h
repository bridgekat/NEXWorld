#ifndef PLUGINAPI_H_
#define PLUGINAPI_H_

// NEARLY "NEWorld-compatible" APIs 23333333

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

// NEXWorld constants

enum {
	NWPluginTypeClientOnly,
	NWPluginTypeServerOnly,
	NWPluginTypeShared
};

const int NWChunkSize = 32;
const int NWAirID = 0;

// NEXWorld structures

struct NWplugindata {
	const char* pluginName;
	const char* authorName;
	const char* internalName;
	int pluginType;
};

struct NWvec3i {
	int x, y, z;
};

struct NWblockdata {
	unsigned int id : 12;
	unsigned int state : 12;
	unsigned int blocklight : 4;
	unsigned int skylight : 4;
};

struct NWblocktype {
	const char* name;
	const char* internalName;
	bool solid;
	bool translucent;
	bool opaque;
	int hardness;
};

// Shared APIs

typedef void NWAPICALL NWchunkgenerator(const NWvec3i*, NWblockdata*);

NWAPIENTRY void NWAPICALL nwLog(const char* s);
NWAPIENTRY int NWAPICALL nwRegisterChunkGenerator(NWchunkgenerator* const generator);

// Client-only APIs

#ifdef NEXWORLD_PLUGIN_CLIENT_SIDE

typedef size_t NWtextureid;
typedef void(*NWblockrenderfunc)(void* cthis, NWblockdata data, int x, int y, int z);

struct NWblocktexture {
	NWtextureid right, left, top, bottom, front, back;
};

NWAPIENTRY NWtextureid NWAPICALL nwRegisterTexture(const char* filename);
NWAPIENTRY void NWAPICALL nwSetBlockRenderFunc(size_t id, NWblockrenderfunc func);
NWAPIENTRY void NWAPICALL nwUseDefaultBlockRenderFunc(size_t id, void* data);

#endif

// Plugin exported functions

NWAPIEXPORT const NWplugindata* NWAPICALL Info();
NWAPIEXPORT int NWAPICALL PreInit();
NWAPIEXPORT int NWAPICALL PostInit();
NWAPIEXPORT void NWAPICALL Unload();

#ifdef __cplusplus
}
#endif

#endif
