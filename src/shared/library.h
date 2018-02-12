#ifndef LIBRARY_H_
#define LIBRARY_H_

#include <string>
#include <functional>

#include "common.h"
#include "logger.h"

#ifdef NEXWORLD_TARGET_WINDOWS
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

class Library {
public:
	Library() = default;
	Library(const Library&) = delete;
	Library(Library&& library): Library() {
		std::swap(library.mHandle, mHandle);
		std::swap(library.mLoaded, mLoaded);
	}
	explicit Library(const std::string& filename): Library() { load(filename); }
	~Library() { if (loaded()) freeLibrary(mHandle); }
	Library& operator=(const Library&) = delete;

	template<class T> T* get(const std::string& name) { return (T*)getFunc<T*>(mHandle, name); }
	operator bool() const { return loaded(); }
	bool loaded() const { return mLoaded; }

	void load(const std::string& filename) {
		if (loaded()) unload();
		mHandle = loadLibrary(filename, mLoaded);
		if (!loaded()) LogWarning("Failed to load \"" + filename + "\" as dynamic library");
	}

	void unload() {
		freeLibrary(mHandle);
		mLoaded = false;
	}

private:

#ifdef NEXWORLD_TARGET_WINDOWS

	using HandleType = HMODULE;

	static HandleType loadLibrary(const std::string& filename, bool& success) {
		HandleType handle = LoadLibraryA(filename.c_str());
		success = handle != nullptr;
		return handle;
	}

	template<class T> static T* getFunc(HandleType handle, const std::string& name) {
		assert(handle != nullptr);
		return reinterpret_cast<T*>(GetProcAddress(handle, name.c_str()));
	}

	static void freeLibrary(HandleType handle) {
		FreeLibrary(handle);
	}

#else

	using HandleType = void*;

	static HandleType loadLibrary(const std::string& filename, bool& success) {
		HandleType handle = dlopen(filename.c_str(), RTLD_LAZY);
		success = handle != nullptr;
		return handle;
	}

	template<class T> static T* getFunc(HandleType handle, const std::string& name) {
		assert(handle != nullptr);
		return reinterpret_cast<T*>(dlsym(handle, name.c_str()));
	}

	static void freeLibrary(HandleType handle) {
		dlclose(handle);
	}

#endif

	HandleType mHandle;
	bool mLoaded = false;
};

#endif
