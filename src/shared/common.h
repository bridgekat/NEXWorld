#ifndef COMMON_H_
#define COMMON_H_

// Compiler
#ifdef _MSC_VER
#	define NEXWORLD_COMPILER_MSVC
#endif

// Debugging
#ifdef _DEBUG
#	define NEXWORLD_DEBUG
#endif

// Target
#ifdef _WIN32
#	define NEXWORLD_TARGET_WINDOWS
#elif defined __MACOSX__ || (defined __APPLE__ && defined __GNUC__)
#	define NEXWORLD_TARGET_MACOSX
#	define NEXWORLD_TARGET_POSIX
#else
#	define NEXWORLD_TARGET_LINUX
#	define NEXWORLD_TARGET_POSIX
#endif

// Check if arithmetic shift right is used
#if ((-1) >> 1) == -1
#	define NEXWORLD_COMPILER_RSHIFT_ARITH
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

constexpr const char* RootPath = "../";
constexpr const char* PluginPath = "Plugins/";
constexpr const char* ConfigPath = "../Configs/";
constexpr const char* ConfigFilename = "Config.ini";

#if defined(NEXWORLD_TARGET_WINDOWS)
constexpr const char* DylibSuffix = ".dll";
#elif defined(NEXWORLD_TARGET_MACOSX)
constexpr const char* DylibSuffix = ".so";
#elif defined(NEXWORLD_TARGET_LINUX)
constexpr const char* DylibSuffix = ".so";
#endif

constexpr const char* LogInfoString = "NEXWorld Logs -- Logging powered by Infiniloggers!";

#endif // !COMMON_H_
