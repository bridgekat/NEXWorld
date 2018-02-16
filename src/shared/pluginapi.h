#ifndef PLUGINAPI_H_
#define PLUGINAPI_H_

#ifdef __cplusplus
extern "C" {
#endif

///// Preprocessor /////

// Platform
#ifdef _MSC_VER
#	define NEXWORLD_COMPILER_MSVC
#endif

#ifdef _WIN32
#	define NEXWORLD_TARGET_WINDOWS
#elif defined __MACOSX__ || (defined __APPLE__ && defined __GNUC__)
#	define NEXWORLD_TARGET_MACOSX
#	define NEXWORLD_TARGET_POSIX
#else
#	define NEXWORLD_TARGET_LINUX
#	define NEXWORLD_TARGET_POSIX
#endif

// NWAPICALL
#ifdef NEXWORLD_COMPILER_MSVC
#	define NWAPICALL __cdecl
#elif defined(__i386__) || defined(__i386)
#	define NWAPICALL __attribute__((__cdecl__))
#else
#	undef NWAPICALL
#	define NWAPICALL
#endif

// NWAPIENTRY
#ifdef NEXWORLD_TARGET_WINDOWS
#	ifdef NEXWORLD_COMPILER_MSVC
#		define NWAPIIMPORT __declspec(dllimport)
#		define NWAPIEXPORT __declspec(dllexport)
#	else
#		define NWAPIIMPORT __attribute__((dllimport))
#		define NWAPIEXPORT __attribute__((dllexport))
#	endif
#else
#	define NWAPIIMPORT __attribute__((visibility("default")))
#	define NWAPIEXPORT __attribute__((visibility("default")))
#endif

#ifdef NEXWORLD_PLUGIN_API_IMPL
#	define NWAPIENTRY NWAPIEXPORT
#else
#	define NWAPIENTRY NWAPIIMPORT
#endif

///// Constants /////

enum {
	NWPluginTypeClientOnly,
	NWPluginTypeServerOnly,
	NWPluginTypeShared
};

const int NWChunkSize = 32;
const int NWAirID = 0;

///// Structures /////

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
	int hardness;
	bool solid;
	bool opaque;
	bool visible;
	bool translucent;
};

typedef unsigned int NWblockid;
typedef void NWAPICALL NWchunkgenerator(const NWvec3i*, NWblockdata*);

#ifdef NEXWORLD_PLUGIN_API_CLIENT

typedef unsigned int NWtextureid;

struct NWvertex {
	NWtextureid tex;
	float u, v, nx, ny, nz, x, y, z;
};

#endif

///// APIs /////

// Pre-init

NWAPIENTRY void NWAPICALL nwLog(const char* s);
NWAPIENTRY void NWAPICALL nwRegisterBlockType(const NWblocktype* blocktype);
NWAPIENTRY int NWAPICALL nwRegisterChunkGenerator(NWchunkgenerator* const generator);
#ifdef NEXWORLD_PLUGIN_API_CLIENT
NWAPIENTRY NWtextureid NWAPICALL nwRegisterTexture(const char* filename);
#endif

// Post-init

NWAPIENTRY int NWAPICALL nwGetBlockIDByName(const char* internalname);
#ifdef NEXWORLD_PLUGIN_API_CLIENT
NWAPIENTRY void NWAPICALL nwRegisterBlockModel(NWblockid id, int count, const NWvertex* vertices,
		bool rightFull, bool leftFull, bool topFull, bool bottomFull, bool frontFull, bool backFull);
#endif

// In-game

///// Plugin exported functions /////

NWAPIEXPORT const NWplugindata* NWAPICALL Info();
NWAPIEXPORT int NWAPICALL PreInit();
NWAPIEXPORT int NWAPICALL PostInit();
NWAPIEXPORT void NWAPICALL Unload();

#ifdef __cplusplus
}
#endif

#endif
