#include "lib.hpp"
#include "log_level.hpp"

int main() {
	std::string fileName = "./temp_log.log";
	EMMLogger::Logger logger(fileName, EMMLogger::LogLevel::error);

	logger.log(time(NULL), "str1");
	logger.log(time(NULL), "str2");
	logger.log(time(NULL), "str3");
	logger.log(time(NULL), "str4");

	logger.setLogLevel(EMMLogger::LogLevel::error);
	logger.log(EMMLogger::LogLevel::info, time(NULL), "str5");
	logger.log(EMMLogger::LogLevel::warning, time(NULL), "str6");
	logger.log(EMMLogger::LogLevel::error, time(NULL), "str7");

	logger.setLogLevel(EMMLogger::LogLevel::warning);
	logger.log(EMMLogger::LogLevel::info, time(NULL), "str8");
	logger.log(EMMLogger::LogLevel::warning, time(NULL), "str9");
	logger.log(EMMLogger::LogLevel::error, time(NULL), "str10");

	logger.setLogLevel(EMMLogger::LogLevel::info);
	logger.log(EMMLogger::LogLevel::info, time(NULL), "str11");
	logger.log(EMMLogger::LogLevel::warning, time(NULL), "str12");
	logger.log(EMMLogger::LogLevel::error, time(NULL), "str13");

	logger.setLogLevel(EMMLogger::LogLevel::error);
	logger.log(EMMLogger::LogLevel::info, time(NULL), "str14");
	logger.log(EMMLogger::LogLevel::warning, time(NULL), "str15");
	logger.log(EMMLogger::LogLevel::error, time(NULL), "str16");

	return 0;
}
