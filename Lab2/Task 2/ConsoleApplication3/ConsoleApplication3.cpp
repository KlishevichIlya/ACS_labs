﻿#include "pch.h"
#include <iostream>
#include <thread>
#include "queue_mutex.h"
#include "lim_mutex_queue.h"
#include "lim_atomic_queue.h"
#include "Consumer.h"
#include "Producer.h"

using namespace std;

typedef uint8_t byte;

unsigned int ProducerNum = 2, ConsumerNum = 2, QueueSize = 2;
const unsigned int TaskNum = 1024;

template<typename Q>
void Task(Q* _queue)
{
	Producer<Q> producer(TaskNum, ProducerNum, _queue);
	Consumer<Q> consumer(ConsumerNum, _queue);

	auto start = chrono::high_resolution_clock::now();
	producer.start();
	consumer.start();

	for (int i = 0; i < ProducerNum; i++)
		producer._threads[i].join();
	for (int i = 0; i < ConsumerNum; i++)
		consumer._threads[i].join();

	auto finish = chrono::high_resolution_clock::now();	
	cout << "Counter: " << consumer.getCounter() << endl;
	cout << "Time: " << chrono::duration_cast<chrono::milliseconds>(finish - start).count() << " ms" << endl;
}

int main()
{
	queue_mutex* _queue_mutex = new queue_mutex();
	limited_queue_mutex* fixed_queue_mutex = new limited_queue_mutex(QueueSize);
	limited_queue_atomic* fixed_queue_atomic = new limited_queue_atomic(QueueSize);

	Task<queue_mutex>(_queue_mutex);
	Task<limited_queue_mutex>(fixed_queue_mutex);
	Task<limited_queue_atomic>(fixed_queue_atomic);
}

