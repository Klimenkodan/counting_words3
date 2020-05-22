#ifndef COUNTING_WORDS2_SAFEQUEUE_H
#define COUNTING_WORDS2_SAFEQUEUE_H
#include <deque>
#include <mutex>
#include <condition_variable>

template <class T>
class safeQueue{
	std::deque<T> que;
	mutable std::mutex m;
	std::condition_variable cvNotEmpty;
	std::condition_variable cvNotMax;

public:
	size_t max_size = 10;

	safeQueue() = default;

	void setMaxSize(size_t newMax) {
		std::lock_guard<std::mutex> lg{m};
		max_size = newMax;
	}

	size_t size() const {
		std::lock_guard<std::mutex> lg{m};
		return que.size();
	}



	void pushFront(const T& item) {
		std::unique_lock<std::mutex> lg{m};
		cvNotMax.wait(lg, [this](){return max_size != que.size();});

		que.push_front(item);
		cvNotEmpty.notify_one();
	}

	void pushBack(const T& item) {
		std::unique_lock<std::mutex> lg{m};
		cvNotMax.wait(lg, [this](){return max_size != que.size();});

		que.push_back(item);
		cvNotEmpty.notify_one();
	}



	T popBack(){
		std::unique_lock<std::mutex> lg{m};
		cvNotEmpty.wait(lg, [this](){return que.size() != 0;});
		T result = que.back();
		que.pop_back();
		return result;
	}

	T popFront(){
		std::unique_lock<std::mutex> lg{m};
		cvNotEmpty.wait(lg, [this](){return que.size() != 0;});
		T result = que.front();
		que.pop_front();
		return result;
	}
};


#endif //COUNTING_WORDS2_SAFEQUEUE_H
