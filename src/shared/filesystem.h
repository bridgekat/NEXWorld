#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <string>
#include <functional>

class Filesystem {
public:
	static bool fileExists(const std::string& path);
	static void createDirectory(const std::string& path);
	static void inDirectory(std::string path, std::function<void(std::string)> callback);
};

#endif
