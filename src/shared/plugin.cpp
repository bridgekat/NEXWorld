#include "plugin.h"
#include "pluginapi.h"
#include "filesystem.h"

namespace PluginAPI {
	// 这个有点丑了，我知道233 不过也就这一处会这么写了。。。
	extern std::string currPluginName;
}

std::vector<Plugin> Plugin::mPlugins;

typedef const NWplugindata* NWAPICALL InfoFunction();
typedef int NWAPICALL PreInitFunction();
typedef int NWAPICALL PostInitFunction();
typedef void NWAPICALL UnloadFunction();

Plugin::Plugin(const std::string& filename, bool client) {
	mLibrary.load(filename);
	if (!mLibrary.loaded()) {
		LogWarning("Could not load plugin \"" + filename + "\"");
		return;
	}

	InfoFunction* info = nullptr;
	info = mLibrary.get<InfoFunction>("Info");
	if (info == nullptr) {
		LogWarning("Could not load plugin \"" + filename + "\": Info() entry not found");
		return;
	}

	const NWplugindata* res = info();
	if (!client && res->pluginType == NWPluginTypeClientOnly) {
		LogWarning("Could not load plugin \"" + filename + "\": client only plugin");
		return;
	}

	mInternalName = res->internalName;
	mName = res->pluginName;
	mAuthor = res->authorName;

	LogInfo("Loaded plugin \"" + mName + "\"[" + mInternalName + "] by " + mAuthor);

	mLoaded = true;
}

Plugin::~Plugin() {
	if (!mLoaded) return;
	callFunc<UnloadFunction>("Unload");
	mLoaded = false;
}

template <class T> void Plugin::callFunc(const std::string& s) {
	PluginAPI::currPluginName = mName;
	T* func = nullptr;
	func = mLibrary.get<T>(s);
	if (func != nullptr) func(); // Function can be not implemented
}

template <class T> void Plugin::callFuncAll(const std::string& func) {
	for (auto& curr: mPlugins) curr.callFunc<T>(func);
}

void Plugin::loadAll(const std::string& path, bool client) {
	LogInfo("Loading plugins from \"" + path + "\", mode: " + (client ? "client" : "server"));
	Filesystem::inDirectory(path, [&client](const std::string& curr) {
		if (curr.substr(curr.length() - std::string(DylibSuffix).length()) == DylibSuffix) {
			mPlugins.push_back(Plugin(curr, client));
		}
	});
}
void Plugin::preInit() { callFuncAll<PreInitFunction>("PreInit"); }
void Plugin::postInit() { callFuncAll<PostInitFunction>("PostInit"); }
