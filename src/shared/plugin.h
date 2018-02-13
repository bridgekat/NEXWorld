#ifndef PLUGIN_H_
#define PLUGIN_H_

#include <vector>
#include <utility>
#include "library.h"

class Plugin {
public:
	Plugin(const std::string& filename);
	Plugin(Plugin&& r): mLibrary(std::move(r.mLibrary)), mLoaded(r.mLoaded) {
		r.mLoaded = false;
	}
	~Plugin();

	static void loadAll(const std::string& path = PluginPath);

private:
	Library mLibrary;
	bool mLoaded = false;

	static std::vector<Plugin> mPlugins;
};

#endif
