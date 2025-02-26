#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>
#include "lib/lib.hpp"
#include "app/task.hpp"
#include "app/task_queue.hpp"

namespace EMMTaskQueue {

template <typename T>
class queue_emtpy_predicate {
	std::queue<T> &queue;
public:
	queue_emtpy_predicate(std::queue<T> &queue) : queue(queue) {}
	bool operator()() {
		return !queue.empty();
	}
};

template <typename T>
class queue_full_predicate {
	std::queue<T> &queue;
	const int maxSize;
public:
	queue_full_predicate(std::queue<T> &queue, int maxSize) : queue(queue), maxSize(maxSize) {}
	bool operator()() {
		return queue.size() != maxSize;
	}
};


ConcurrentTaskQueue::ConcurrentTaskQueue(int maxItems) : maxItems(maxItems > 1 ? maxItems : 1) {}

void ConcurrentTaskQueue::push(std::unique_ptr<EMMTask::ITask> task) {
	{
		std::lock_guard<std::mutex> lockm(mutex);
		if (queue.size() == maxItems) {
			throw QueueOverflowException("Concurrent task queue overflowed");
		}
		queue.push(std::move(task));
	}
	pop_conditional.notify_one();
}

void ConcurrentTaskQueue::pushAndWait(std::unique_ptr<EMMTask::ITask> task) {
	{
		std::unique_lock<std::mutex> lockm(mutex);
		push_conditional.wait(lockm, queue_full_predicate(queue, maxItems));
		queue.push(std::move(task));
	}
	pop_conditional.notify_one();
}

std::unique_ptr<EMMTask::ITask> ConcurrentTaskQueue::popAndWait() {
	std::unique_lock<std::mutex> lockm(mutex);
	pop_conditional.wait(lockm, queue_emtpy_predicate(queue));

	std::unique_ptr<EMMTask::ITask> task = std::move(queue.front());
	queue.pop();
	lockm.unlock();

	push_conditional.notify_one();
	return task;
}

QueueOverflowException::QueueOverflowException(const char* mess) : std::runtime_error(mess) {}

}	//namespace
