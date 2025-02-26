#include <iostream>
#include <thread>
#include "lib/lib.hpp"
#include "app/task.hpp"
#include "app/task_queue.hpp"
#include "app/pc_threads.hpp"

/*
	Данные конфигурации программы.
*/
struct config {
	const char* logFileName;
	EMMLogger::LogLevel defaultLogLevel;
};

/*
	Обрабатывает данные с консоли, помещает данные в conf. 
	В случае успеха возвращает true, иначе false.
*/
bool argToConfig(int argc, char *argv[], config &conf);

//размер очереди для задач
const int QUEUE_SIZE = 10;

int main(int argc, char *argv[]) {
	//обрабатываем аргументы для получения конфигурации
	config conf{};
	if (!argToConfig(argc, argv, conf)) {
		std::cout << "please exec program with argument \"prog_name <log_file_name> <log_level>\" without \"<>\" and errors" << std::endl;
		return -1;
	}

	EMMLogger::Logger logger(conf.logFileName, conf.defaultLogLevel);
	EMMTaskQueue::ConcurrentTaskQueue queue(QUEUE_SIZE);

	EMMProdCons::Consumer consumer(queue);
	EMMProdCons::Producer producer(logger, queue, consumer.consumerAliveFlag());
	//передаем флаг жизни потребителя производителю, чтобы он мог закончить работу при досрочной смерти потребителя
	//ужасное решение, но другого способа пока не придумал

	try {
		//старт производителя и потребителя.
		//объекты копируются, объекты в этой области видимости использовать нельзя
		std::thread consumerThread(consumer);
		std::thread producerThread(producer);

		producerThread.join();	//ожидаем конец работы производителя, чтобы отдать потребителю смертельную пилюлю
		queue.push(std::make_unique<EMMProdCons::Stoper>(consumer.createStoper()));	//убиваем потребителя, если он жив
		consumerThread.join();
	} catch (...) {	//destructors in try
		consumer.makeDead();
		producer.makeDead();
		throw;	
	}

	return 0;
}

bool argToConfig(int argc, char *argv[], config &conf) {
	if (argc != 3) {
		return false;
	}

	int llCode = -1;
	int scode = sscanf(argv[2], "%d", &llCode);
	if (scode != 1) {
		return false;
	}
	
	bool successFlag = EMMLogger::logLevelFromInt(llCode, conf.defaultLogLevel);
	if (!successFlag) {
		return false;
	}

	conf.logFileName = argv[1];

	return true;
}
