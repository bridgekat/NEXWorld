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

// NXAPICALL
#ifdef NEXWORLD_COMPILER_MSVC
#	define NXAPICALL __cdecl
#elif defined(__i386__) || defined(__i386)
#	define NXAPICALL __attribute__((__cdecl__))
#else
#	undef NXAPICALL
#	define NXAPICALL
#endif

// NXAPIENTRY
#ifdef NEXWORLD_TARGET_WINDOWS
#	ifdef NEXWORLD_COMPILER_MSVC
#		define NXAPIIMPORT __declspec(dllimport)
#		define NXAPIEXPORT __declspec(dllexport)
#	else
#		define NXAPIIMPORT __attribute__((dllimport))
#		define NXAPIEXPORT __attribute__((dllexport))
#	endif
#else
#	define NXAPIIMPORT __attribute__((visibility("default")))
#	define NXAPIEXPORT __attribute__((visibility("default")))
#endif

#ifdef NEXWORLD_PLUGIN
#define NXAPIENTRY NXAPIIMPORT
#else
#define NXAPIENTRY NXAPIEXPORT
#endif

constexpr const char* RootPath = "../";
constexpr const char* PluginPath = "../Plugins/";
constexpr const char* ConfigPath = "../Configs/";
constexpr const char* ConfigFilename = "Config.ini";

#if defined(NEXWORLD_TARGET_WINDOWS)
constexpr const char* DylibSuffix = "dll";
#elif defined(NEXWORLD_TARGET_MACOSX)
constexpr const char* DylibSuffix = "dylib";
#elif defined(NEXWORLD_TARGET_LINUX)
constexpr const char* DylibSuffix = "so";
#endif

constexpr const char* LogInfoString = "NEXWorld Logs -- Logging powered by Infiniloggers!";

#endif // !COMMON_H_
