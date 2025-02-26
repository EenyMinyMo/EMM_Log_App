#include <iostream>
#include <thread>
#include "lib/lib.hpp"
#include "app/task.hpp"
#include "app/task_queue.hpp"
#include "app/pc_threads.hpp"

struct config {
	const char* logFileName;
	EMMLogger::LogLevel defaultLogLevel;
};

int argToConfig(int argc, char *argv[], config &conf);

const int QUEUE_SIZE = 10;

int main(int argc, char *argv[]) {
	try {
		config conf{};
		if (argToConfig(argc, argv, conf)) {
			std::cout << "please exec program with argument \"prog_name <log_file_name> <log_level>\" without \"<>\" and errors" << std::endl;
			return -1;
		}

		EMMLogger::Logger logger(conf.logFileName, conf.defaultLogLevel);
		EMMTaskQueue::ConcurrentTaskQueue queue(QUEUE_SIZE);

		EMM_PCThreads::Consumer consumer(logger, queue);
		EMM_PCThreads::Producer producer(logger, queue, consumer.consumerAliveFlag());	//ужасное решение, но другого способа не вижу

		std::thread consumerThread(consumer);
		std::thread producerThread(producer);

		// producer();

		producerThread.join();
		queue.push(std::make_unique<EMM_PCThreads::Stoper>(consumer.createStoper()));
		consumerThread.join();
	} catch (...) {	//destructors in try
		throw;	
	}

	return 0;
}

int argToConfig(int argc, char *argv[], config &conf) {
	if (argc != 3) {
		return -1;
	}

	int llCode = -1;
	int scode = sscanf(argv[2], "%d", &llCode);
	if (scode != 1) {
		return -1;
	}
	
	bool successFlag = EMMLogger::logLevelFromInt(llCode, conf.defaultLogLevel);
	if (!successFlag) {
		return -1;
	}

	conf.logFileName = argv[1];

	return 0;
}
