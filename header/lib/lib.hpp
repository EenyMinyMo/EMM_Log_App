#ifndef EMM_LIB_HEADER
#define EMM_LIB_HEADER

#include <string>
#include <fstream>
#include <stdexcept>
#include <ctime>

namespace EMMLogger {

enum class LogLevel { info = 0,	warning = 1, error = 2 };

/*
	Возвращает строку, соответсвующую переданному уровню логирования
*/
const char* logLevelToStr(LogLevel ll);

/*
	Записывает в output уровень логирования, соответствующий коду i и возвращает true.
	Если i не соответствует уровню лога, в output ничего не записывается, возвращается false.
*/
bool logLevelFromInt(int i, LogLevel &output);

/*
	Логгер, записывающий данные в файл.
	В случае ошибки операции с файлами выкидывает LoggerException.
*/
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
