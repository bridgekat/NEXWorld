#ifndef LOGGER_H_
#define LOGGER_H_

#include <iostream>
#include <string>
#include <sstream>

#define LOG(MESSAGE, OUT_STREAM, LEVEL) \
    OUT_STREAM << static_cast<std::ostringstream&>(std::ostringstream().flush() << \
    LEVEL << MESSAGE).str() << std::endl;

inline void LogVerbose(const std::string& message) { LOG(message, std::cout, "[Verbose]"); }
inline void LogInfo(const std::string& message) { LOG(message, std::cout, "[Info]"); }
inline void LogWarning(const std::string& message) { LOG(message, std::cout, "[Warning]"); }
inline void LogError(const std::string& message) { LOG(message, std::cout, "[Error]"); }
inline void LogFatal(const std::string& message) { LOG(message, std::cout, "[Fatal]"); }

#undef LOG

#endif
