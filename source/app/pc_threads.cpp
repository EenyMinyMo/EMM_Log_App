#include <iostream>
#include "lib/lib.hpp"
#include "app/task.hpp"
#include "app/task_queue.hpp"
#include "app/pc_threads.hpp"

namespace EMM_PCThreads {

Consumer::Consumer(EMMLogger::Logger &logger, EMMTaskQueue::ConcurrentTaskQueue &queue) : logger(logger), taskQueue(queue), resultCode(0) {}

void Consumer::operator()() {
	try {
		while (*isAlive) {
			std::unique_ptr<EMMTask::ITask> taskp = taskQueue.popAndWait();
			taskp->run();
		}
	} catch (EMMLogger::LoggerException &e) {
		std::cout << e.what() << std::endl;
		makeDead();
		resultCode = -1;
	} catch (...) {
		makeDead();
		resultCode = -1;
		throw;
	}
}

int Consumer::getResultCode() {
	return resultCode;
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


Stoper::Stoper(std::shared_ptr<bool> aliveFlag) : aliveFlag(aliveFlag) {}

void Stoper::run() {
	*aliveFlag = false;
}

Producer::Producer(EMMLogger::Logger &logger, EMMTaskQueue::ConcurrentTaskQueue &taskQueue, std::shared_ptr<bool> consumerAliveFlag) : logger(logger), taskQueue(taskQueue), consumerAliveFlag(consumerAliveFlag) {}

static void sendTaskPushNoWait(EMMTaskQueue::ConcurrentTaskQueue &queue, std::unique_ptr<EMMTask::ITask> up) {
	try {
		queue.push(std::move(up));
	} catch (EMMTaskQueue::QueueOverflowException &e) {
		std::cout << "Task logging queue overflowed, log not sended" << std::endl;
	}
}

static void sendTaskPushAndWait(EMMTaskQueue::ConcurrentTaskQueue &queue, std::unique_ptr<EMMTask::ITask> up) {
	queue.pushAndWait(std::move(up));
}

void Producer::operator()() {
	try {
		while ((std::cin) && *isAlive && *consumerAliveFlag) {
			std::string str;
			getline(std::cin, str);

			if (std::cin.eof()) {
				break;
			}

			std::time_t time = std::time(0);

			int fnpc = str.find_first_not_of(" ");	//первый непробельный символ в строке
			if (fnpc == std::string::npos) {
				std::cout << "No message" << std::endl;
				continue;
			}
			std::string partialTrimmedString = str.substr(str.find_first_not_of(" "));
			if (partialTrimmedString.size() == 0) {
				std::cout << "No message" << std::endl;
				continue;
			}

			std::unique_ptr<EMMTask::ITask> *up;

			int llCode = -1;
			if (sscanf(partialTrimmedString.c_str(), logLevelDecodeString, &llCode) == 1) {	//поиск кода уровня логирования
				EMMLogger::LogLevel ll;
				bool flag = EMMLogger::logLevelFromInt(llCode, ll);
				if (flag) {	//если уровень логирования введен правильно
					int messStart = partialTrimmedString.find(" ");
					if (messStart == std::string::npos) {	//сообщения нет
						std::cout << "No message" << std::endl;
						continue;
					}

					messStart = partialTrimmedString.find_first_not_of(" ", messStart);
					if (messStart == std::string::npos) {	//сообщения нет
						std::cout << "No message" << std::endl;
						continue;
					}

					std::string message = partialTrimmedString.substr(messStart);
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

}	//namespace
