#include "filesystem.h"
#include <sstream>
#include "common.h"
#include "logger.h"

#ifdef NEXWORLD_TARGET_WINDOWS
#include <Windows.h>
#include <io.h>
#include <direct.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/dir.h>
#endif

bool Filesystem::fileExists(const std::string& path) {
#ifdef NEXWORLD_TARGET_WINDOWS
	return _access(path.c_str(), 0) == 0;
#else
	return access(path.c_str(), 0) == 0;
#endif
}

void Filesystem::createDirectory(const std::string& path) {
#ifdef NEXWORLD_TARGET_WINDOWS
	_mkdir(path.c_str());
#else
	mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
}

void Filesystem::inDirectory(const std::string& path, std::function<void(std::string)> callback) {
#ifdef NEXWORLD_TARGET_WINDOWS
	WIN32_FIND_DATA ffd;
	HANDLE hFind = FindFirstFileA((path + "*").c_str(), &ffd);
	if (hFind == INVALID_HANDLE_VALUE) {
		std::stringstream ss;
		ss << "Listing directory \"" + path + "\" failed: error " << GetLastError();
		LogWarning(ss.str());
		return;
	}
	do {
		if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
		callback(path + std::string(ffd.cFileName));
	} while (FindNextFileA(hFind, &ffd) != 0);
	FindClose(hFind);
#else
	auto pDir = opendir(path.c_str());
	if (pDir == nullptr) {
		std::stringstream ss;
		ss << "Listing directory \"" + path + "\" failed";
		LogWarning(ss.str());
		return;
	}
	struct dirent *ent;
	while ((ent = readdir(pDir)) != nullptr) {
		if (ent->d_type & DT_DIR) continue;
		callback(path + ent->d_name);
	}
#endif
}
