#include "pluginapi.h"
#include "logger.h"

extern "C" {

	NXAPIEXPORT void NXAPICALL nxLog(const char* s) {
		LogInfo("[Plugin]" + std::string(s));
	}

}
