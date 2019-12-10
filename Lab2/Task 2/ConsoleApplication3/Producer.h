#include "pch.h"
#include <thread>

using namespace std;

typedef uint8_t byte;

template<typename Q>
class Producer
{
private:
    int _counter;
	int _producerNum;
	Q* _queue;

public:
	thread* _threads;
	Producer(int counter,  int producerNum, Q* queue);
	void input();
	void start();
	
};

template<typename Q>
Producer<Q>::Producer(int counter, int producerNum, Q* queue)
{
	_counter = counter * producerNum;
	_producerNum = producerNum;
	_queue = queue;
}

template<typename Q>
void Producer<Q>::input()
{
	for (int i = 0; i < _counter; i++)
		_queue->push(1);
}

template<typename Q>
void Producer<Q>::start()
{
	_threads = new thread[_producerNum];
	for (int i = 0; i < _producerNum; i++)
		_threads[i] = thread(&Producer::input, this);
}
