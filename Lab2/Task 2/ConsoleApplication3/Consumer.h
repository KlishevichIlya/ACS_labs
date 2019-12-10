#include "pch.h"
#include <thread>

using namespace std;

typedef uint8_t byte;
atomic<int> atc = 0;
template<typename Q>
class Consumer
{
private:
    long long _counter;
    int _consumerNum;
	Q* _queue;

public:
	thread* _threads;
	Consumer(int consumerNum, Q* queue);
	void read();
    int getCounter();
	void start();
	
};

template<typename Q>
Consumer<Q>::Consumer(int consumerNum, Q* queue)
{
	_counter = 0;
	_consumerNum = consumerNum;
	_queue = queue;
	atc = 0;
}

template<typename Q>
void Consumer<Q>::read()
{
	bool flag = true;
	byte value;
	while (4096 > atc.load())
	{
		flag = _queue->pop(value);
		if (flag == true)
		{
			_counter += value;
			atc++;
		}		
		
	}
} 

template<typename Q>
int Consumer<Q>::getCounter()
{
	return _counter;
}

template<typename Q>
void Consumer<Q>::start()
{
	_threads = new thread[_consumerNum];
	for (int i = 0; i < _consumerNum; i++)
		_threads[i] = thread(&Consumer::read, this);
}

