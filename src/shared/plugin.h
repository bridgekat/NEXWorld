#ifndef PLUGIN_H_
#define PLUGIN_H_

#include "library.h"

class Plugin {
public:


private:
	Library mLibrary;
	bool mLoaded, mCompatible;
};

#endif
