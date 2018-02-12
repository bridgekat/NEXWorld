#ifndef COMMON_H_
#define COMMON_H_

#ifdef _DEBUG
#	define NEXWORLD_DEBUG
#endif

#ifdef _WIN32
#	define NEXWORLD_TARGET_WINDOWS
#endif

// Check if arithmetic shift right is used
#if ((-1) >> 1) == -1
#	define NEXWORLD_COMPILER_RSHIFT_ARITH
#endif

constexpr const char* RootPath = "../";
constexpr const char* PluginPath = "../Plugins/";
constexpr const char* ConfigPath = "../Configs/";
constexpr const char* ConfigFilename = "Config.ini";

#endif // !COMMON_H_
