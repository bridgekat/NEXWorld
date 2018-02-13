#include "plugin.h"
#include "filesystem.h"

std::vector<Plugin> Plugin::mPlugins;

typedef int NWAPICALL InitFunction();
typedef void NWAPICALL UnloadFunction();

Plugin::Plugin(const std::string& filename) {
	mLibrary.load(filename);
	if (!mLibrary.loaded()) {
		LogWarning("Could not load plugin \"" + filename + "\"");
		return;
	}

	InitFunction* init = nullptr;
	init = mLibrary.get<InitFunction>("Init");
	if (init == nullptr) {
		LogWarning("Could not load plugin \"" + filename + "\": Init() entry not found");
		return;
	}

	int res = init();
	if (res == 0) mLoaded = true;
	else {
		std::stringstream ss("Could not load plugin \"" + filename + "\": Init() returns ");
		ss << res;
		LogWarning(ss.str());
	}
}

Plugin::~Plugin() {
	if (!mLoaded) return;
	UnloadFunction* unload = nullptr;
	unload = mLibrary.get<UnloadFunction>("Unload");
	if (unload != nullptr) unload();
	mLoaded = false;
}

void Plugin::loadAll(const std::string& path) {
	Filesystem::inDirectory(path, [](const std::string& curr) {
		if (curr.substr(curr.length() - std::string(DylibSuffix).length()) == DylibSuffix) {
			mPlugins.push_back(Plugin(curr));
		}
	});
}
