#include <iostream>
#include "lib/lib.hpp"
#include "app/task.hpp"
#include "app/task_queue.hpp"
#include "app/pc_threads.hpp"

namespace EMMProdCons {

Consumer::Consumer(EMMTaskQueue::ConcurrentTaskQueue &queue) : taskQueue(queue) {
	*isAlive = true;
}

void Consumer::operator()() {
	try {
		while (*isAlive) {
			std::unique_ptr<EMMTask::ITask> taskp = taskQueue.popAndWait();
			taskp->run();
		}
	} catch (EMMLogger::LoggerException &e) {
		std::cout << e.what() << std::endl;
		makeDead();
	} catch (...) {
		makeDead();
		throw;
	}
}

void Consumer::makeDead() {
	*isAlive = false;
}

Stoper Consumer::createStoper() {
	return Stoper(isAlive);
}

std::shared_ptr<bool> Consumer::consumerAliveFlag() {
	return isAlive;
}

Consumer::~Consumer() {
	makeDead();
}

Stoper::Stoper(std::shared_ptr<bool> aliveFlag) : aliveFlag(aliveFlag) {}

void Stoper::run() {
	*aliveFlag = false;
}

Producer::Producer(EMMLogger::Logger &logger, EMMTaskQueue::ConcurrentTaskQueue &taskQueue, std::shared_ptr<bool> consumerAliveFlag) : logger(logger), taskQueue(taskQueue), consumerAliveFlag(consumerAliveFlag) {
	*isAlive = true;
}

//Отправляет таску в очередь. Если отправка вызовет исключение, выведет сообщение об этом в консоль
static void sendTaskPushNoWait(EMMTaskQueue::ConcurrentTaskQueue &queue, std::unique_ptr<EMMTask::ITask> up) {
	try {
		queue.push(std::move(up));
	} catch (EMMTaskQueue::QueueOverflowException &e) {
		std::cout << "Task logging queue overflowed, log not sended" << std::endl;
	}
}

//Отправляет таску в очередь, отправка блокирующая
static void sendTaskPushAndWait(EMMTaskQueue::ConcurrentTaskQueue &queue, std::unique_ptr<EMMTask::ITask> up) {
	queue.pushAndWait(std::move(up));
}

void Producer::operator()() {
	try {
		while ((std::cin) && *isAlive && *consumerAliveFlag) {	//цикл пока поток ввода валиден, производитель и потребитель живы
			std::string str;
			getline(std::cin, str);	//считываем строку с консоли

			if (std::cin.eof()) {	//EOF - конец работы
				break;
			}

			std::time_t time = std::time(0);	//сохраняем время поступления лога

			int fnpc = str.find_first_not_of(" ");	//первый непробельный символ в строке
			if (fnpc == std::string::npos) {	//строка пуста
				std::cout << "No message" << std::endl;
				continue;
			}
			std::string partialTrimmedString = str.substr(str.find_first_not_of(" "));
			// if (partialTrimmedString.size() == 0) {
			// 	std::cout << "No message" << std::endl;
			// 	continue;
			// }

			int llCode = -1;	//код уровня логирования
			int scanCode = sscanf(partialTrimmedString.c_str(), logLevelDecodeString, &llCode);	//поиск кода уровня логирования
			if (scanCode == 1) {	//уровень логирования присутствует в строке
				EMMLogger::LogLevel ll;
				bool flag = EMMLogger::logLevelFromInt(llCode, ll);	//пытаемся получить LogLevel
				if (flag) {	//если уровень логирования введен правильно
					int messStart = partialTrimmedString.find(" ");
					if (messStart == std::string::npos) {	//сообщения нет
						std::cout << "No message" << std::endl;
						continue;
					}

					messStart = partialTrimmedString.find_first_not_of(" ", messStart);	//поиск сообщения после уровня логирования
					if (messStart == std::string::npos) {	//сообщения нет
						std::cout << "No message" << std::endl;
						continue;
					}

					std::string message = partialTrimmedString.substr(messStart);	//удаляем входные пробелы после кода логирования
					std::unique_ptr<EMMTask::ITask> up = std::make_unique<EMMTask::SendLogTaskLL>(EMMTask::SendLogTaskLL(logger, ll, time, message));
					sendTaskPushNoWait(taskQueue, std::move(up));
				} else {	//уровень логирования введен неверно, не логируем
					std::cout << "Please enter log level code in right range" << std::endl;
				}
			} else {	//код уровня логирования отсутствует
				std::unique_ptr<EMMTask::ITask> up = std::make_unique<EMMTask::SendLogTask>(EMMTask::SendLogTask(logger, time, partialTrimmedString));
				sendTaskPushNoWait(taskQueue, std::move(up));
			}
		}
		makeDead();
	} catch (...) {
		makeDead();
		std::cout << "Error while logging" << std::endl;
		throw;
	}
}

void Producer::makeDead() {
	*isAlive = false;
}

Stoper Producer::createStoper() {
	return Stoper(isAlive);
}

Producer::~Producer() {
	makeDead();
}

}	//namespace
