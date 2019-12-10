#include <iostream>
#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>

using namespace std;
typedef uint8_t byte;

class limited_queue_mutex
{
private:
	queue<byte> _queue;
	mutex mtx;
    int _size;
	condition_variable _read_condition;
	condition_variable _write_condition;

public:
	limited_queue_mutex(int size);
	
	void push(byte temp);
	bool pop(byte& temp);
};

limited_queue_mutex::limited_queue_mutex(int size)
{
	_size = size;
}


void limited_queue_mutex::push(byte temp)
{
	unique_lock<mutex> mutex_flag(mtx);
	if (_size == _queue.size()) 
	{
		_write_condition.wait(mutex_flag); 
		_read_condition.notify_one(); 
	}
	_queue.push(temp);
	_read_condition.notify_one(); 
}

bool limited_queue_mutex::pop(byte& temp)
{
	unique_lock<mutex> mutex_flag(mtx);

	if (_queue.empty())
	{
		_read_condition.wait_for(mutex_flag, chrono::milliseconds(10)); 
		if (_queue.empty())
			return false;
	}
	temp = _queue.front();
	_queue.pop();
	_write_condition.notify_one();
	return true;
}

