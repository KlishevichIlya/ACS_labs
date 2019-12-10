#include <queue>
#include <mutex>
#include <thread>
#include <condition_variable>

using namespace std;
typedef uint8_t byte;

class queue_mutex
{
private:
	queue<byte> _queue;
	mutex mtx;

public:	
	void push(byte val);
	bool pop(byte& val);
};

void queue_mutex::push(byte val)
{
	unique_lock<mutex> mutex_flag(mtx);
	_queue.push(val);
}

bool queue_mutex::pop(byte& val)
{
	unique_lock<mutex> mutex_flag(mtx);

	if (_queue.empty())
	{
		mutex_flag.unlock();
		this_thread::sleep_for(chrono::milliseconds(10));
		mutex_flag.lock();
		if (_queue.empty())
			return false;
	}

	val = _queue.front();
	_queue.pop();
	return true;
}

