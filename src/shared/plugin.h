#ifndef PLUGIN_H_
#define PLUGIN_H_

#include <vector>
#include <utility>
#include <string>
#include "library.h"

class Plugin {
public:
	Plugin(const std::string& filename, bool client = false);
	Plugin(Plugin&& r): mLibrary(std::move(r.mLibrary)), mLoaded(r.mLoaded),
		mInternalName(std::move(r.mInternalName)), mName(std::move(r.mName)), mAuthor(std::move(r.mAuthor)) {
		r.mLoaded = false;
	}
	~Plugin();

	std::string internalName() { return mInternalName; }
	std::string name() { return mName; }
	std::string author() { return mAuthor; }

	static void loadAll(const std::string& path = PluginPath, bool client = false);
	static void preInit();
	static void postInit();

private:
	Library mLibrary;
	bool mLoaded = false;
	std::string mInternalName, mName, mAuthor;

	static std::vector<Plugin> mPlugins;

	template <class T> void callFunc(const std::string& func);
	template <class T> static void callFuncAll(const std::string& func);
};

#endif
