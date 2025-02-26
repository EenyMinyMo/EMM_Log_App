#include <string>
#include <fstream>
#include <stdexcept>
#include <ctime>
#include "lib/lib.hpp"

namespace EMMLogger {

//для получения строк, соответствующим уровню логирования
static const char *logLevelStr[] = { "info", "warning", "error" };
static const char *undefinedLogLevelStr = "*undefined log level";

//вставляет дополнительный 0, если переданное число между 0 и 9
static void insertZeroIfNeed(int val, std::ofstream &stream) {
	if (val >= 0 && val < 10) {
		stream << '0';
	}
}

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

bool logLevelFromInt(int i, LogLevel &output) {
	LogLevel ll = static_cast<LogLevel>(i);
	switch (ll) {
		case LogLevel::info :
		case LogLevel::warning :
		case LogLevel::error : {
			output = ll;
			return true;
		} break;

		default: {
			return false;
		}
	}
}

Logger::Logger(const char *fileName, LogLevel defaultLogLevel) : logOutStream(fileName), logLevel(defaultLogLevel) {
	if (!logOutStream) {
		throw LoggerException("IO Exception Logger::Logger - file output stream error");
	}
}

Logger::~Logger() {
	logOutStream.close();
}

void Logger::setLogLevel(LogLevel ll) {
	logLevel = ll;
}

void Logger::log(LogLevel ll, std::time_t time, const std::string &mess) {
	if (!logOutStream) {
		throw LoggerException("IO Exception Logger::log(LogLevel, std::time_t, const std::string) - file output stream error");
	}

	if (ll < logLevel) {	//не логируем, если уровень лога ниже того, что по умолчанию
		return;
	}

	std::tm *timeStamp = std::localtime(&time);	//Время лога -> дата лога
	
	logOutStream << (timeStamp->tm_year + 1900) << '-';

	int month = timeStamp->tm_mon + 1;
	insertZeroIfNeed(month, logOutStream);
	logOutStream << month << '-';

	int day = timeStamp->tm_mday;
	insertZeroIfNeed(day, logOutStream);
	logOutStream << day << ' ';

	int hour = timeStamp->tm_hour;
	insertZeroIfNeed(hour, logOutStream);
	logOutStream << hour << ':';

	int min = timeStamp->tm_min;
	insertZeroIfNeed(min, logOutStream);
	logOutStream << min << ':';

	int sec = timeStamp->tm_sec;
	insertZeroIfNeed(sec, logOutStream);
	logOutStream << sec;

	logOutStream << " [" << logLevelToStr(ll) << "] " << mess << std::endl;
}

void Logger::log(std::time_t time, const std::string &mes) {
	log(logLevel, time, mes);
}

LoggerException::LoggerException(const char* mess) : std::runtime_error(mess) {}

}	//namespace
