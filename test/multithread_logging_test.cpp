#include <iostream>
#include <thread>
#include <memory>
#include <chrono>
#include <ctime>
#include <random>
#include "lib/lib.hpp"
#include "app/task.hpp"
#include "app/task_queue.hpp"
#include "app/pc_threads.hpp"

class Producer {
	EMMLogger::Logger &logger;
	EMMTaskQueue::ConcurrentTaskQueue &taskQueue;
	int countLogs;
	int prodNumber;
public:
	Producer(EMMLogger::Logger &logger, EMMTaskQueue::ConcurrentTaskQueue &taskQueue, int prodNumber, int countLogs) : 
		logger(logger), 
		taskQueue(taskQueue), 
		prodNumber(prodNumber), 
		countLogs(countLogs) {}
	
	void operator()() {
		std::cout << "start producer " << prodNumber << std::endl;
		std::random_device rd;   // non-deterministic generator
    	std::mt19937 gen(rd()); 
		std::uniform_int_distribution<> sleep_dist(0,0);
		std::uniform_int_distribution<> level_dist(0,2);

		for (int i = 0; i < countLogs; i++) {
			EMMLogger::LogLevel ll = EMMLogger::LogLevel(level_dist(gen));

			std::string str = "Thread prod ";
			str += std::to_string(prodNumber);
			str += " produce log number ";
			str += std::to_string(i);
			str += " with log level ";
			str += std::to_string(int(ll));

			std::time_t t = std::time(0);

			std::unique_ptr<EMMTask::ITask> up = std::make_unique<EMMTask::SendLogTaskLL>(EMMTask::SendLogTaskLL(logger, ll, t, str));
			try {
				taskQueue.push(std::move(up));
			} catch (EMMTaskQueue::QueueOverflowException &e) {
				std::cout << "Prod " << prodNumber << " queue buf overflow, mess \"" << str << "\" not logged" << std::endl; 
			}

			std::chrono::milliseconds timespan(sleep_dist(gen));
			std::this_thread::sleep_for(timespan);
		}
		std::cout << "end producer " << prodNumber << std::endl;
	}
};

int main() {
	std::cout << "start test" << std::endl;

	const char *logFileName = "./temp/mt_log.log";
	EMMLogger::Logger logger{ logFileName, EMMLogger::LogLevel::info };
	EMMTaskQueue::ConcurrentTaskQueue queue{ 5 };

	EMM_PCThreads::Consumer consumer(logger, queue);

	std::thread consumerThread(consumer);
	std::thread producerThread_1(Producer(logger, queue, 1, 10000));
	std::thread producerThread_2(Producer(logger, queue, 2, 10000));


	producerThread_2.join();
	producerThread_1.join();

	queue.push(std::move(std::make_unique<EMM_PCThreads::ConsumerStoper>(consumer.createStoper())));
	consumerThread.join();

	std::cout << "end test" << std::endl;
	return 0;
}
