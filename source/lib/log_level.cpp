#include "log_level.hpp"

namespace EMMLogger {
	static const char *logLevelStr[] = { "info", "warning", "error" };
	static const char *undefinedLogLevelStr = "*undefined log level";

	const char* logLevelToStr(LogLevel ll) {
		switch (ll) {
			case LogLevel::info : {
				return logLevelStr[0];
			} break;

			case LogLevel::warning : {
				return logLevelStr[1];
			} break;

			case LogLevel::error : {
				return logLevelStr[2];
			} break;

			default: {
				return undefinedLogLevelStr;
			}
		}
	}
}
