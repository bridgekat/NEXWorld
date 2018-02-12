#ifndef PLUGINAPI_H_
#define PLUGINAPI_H_

#include "common.h"

extern "C" {

	NXAPIENTRY void NXAPICALL nxLog(const char* s);

	// Plugin exported functions
	NXAPIEXPORT void NXAPICALL Init();

}

#endif
