#include "pluginapi.h"
#include "logger.h"

extern "C" {

	NWAPIEXPORT void NWAPICALL nwLog(const char* s) {
		LogInfo("[Plugin]" + std::string(s));
	}

}
