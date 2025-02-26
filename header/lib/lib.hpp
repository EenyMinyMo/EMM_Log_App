#ifndef EMM_LIB_HEADER
#define EMM_LIB_HEADER

#include <string>
#include <fstream>
#include <stdexcept>
#include <ctime>

namespace EMMLogger {

enum class LogLevel { info = 0,	warning = 1, error = 2 };

const char* logLevelToStr(LogLevel ll);

class Logger {
	std::ofstream logOutStream;
	LogLevel logLevel;

public:
	Logger(const char *fileName, LogLevel defaultLogLevel);
	Logger(const Logger &l) = delete;
	Logger& operator=(const Logger &l) = delete;

	void setLogLevel(LogLevel ll);

	void log(LogLevel ll, std::time_t time, const std::string &mess);
	void log(std::time_t time, const std::string &mes);

	~Logger();
};

class LoggerException : public std::runtime_error {
public:
	explicit LoggerException(const char* mess);
};

}	//namespace

#endif
