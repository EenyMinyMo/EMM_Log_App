#ifndef EMM_LIB_LL_HEADER
#define EMM_LIB_LL_HEADER

namespace EMMLogger {
	enum class LogLevel { info = 0,	warning, error };

	const char* logLevelToStr(LogLevel ll);
}

#endif
