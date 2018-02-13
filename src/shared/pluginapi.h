#ifndef PLUGINAPI_H_
#define PLUGINAPI_H_

#include "common.h"

extern "C" {

	NWAPIENTRY void NWAPICALL nwLog(const char* s);

	// Plugin exported functions
	NWAPIEXPORT int NWAPICALL Init();

}

#endif
