#ifndef EMM_LIB_HEADER
#define EMM_LIB_HEADER

#include <string>
#include <fstream>
#include <ctime>

namespace EMMLogger {
	enum class LogLevel { info = 0,	warning, error };

	const char* logLevelToStr(LogLevel ll);

	class Logger {
		std::ofstream logOutStream;
		LogLevel logLevel;

	public:
		Logger(const std::string &fileName, LogLevel defaultLogLevel);
		Logger(Logger &l) = delete;
		Logger& operator=(Logger &l) = delete;

		void setLogLevel(LogLevel ll);

		void log(LogLevel ll, std::time_t time, const std::string &mess);
		void log(std::time_t time, const std::string &mes);

		~Logger();
	};
}

#endif
