#include "pch.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>

using namespace std;

typedef int8_t byte;
int arrSize = 1024 * 1024;
atomic<int> counterAtomic = 0;
int counterMutex = 0;
mutex mtx;

void Print(int NumThreads, long long duration)
{
	cout << "Кол-во потоков: " << NumThreads << ", Время: " << duration << "\n";
}

void Atomic(byte *array)
{
    int counter = 0;
	while (true)
	{
		counter = counterAtomic++;
		if (counter < arrSize)
			(*(array + counter))++;
		else
			return;
	}
}

void AtomicSleep(byte *array)
{
    int counter = 0;
	while (true)
	{
		counter = counterAtomic++;
		if (counter < arrSize)
		{
			(*(array + counter))++;
			this_thread::sleep_for(chrono::nanoseconds(10));
		}
		else
			return;
	}
}

void Mutex(byte *array)
{
    int counter = 0;
	while (true)
	{
		mtx.lock();
		counter = counterMutex++;
		mtx.unlock();
		if (counter < arrSize)
			(*(array + counter))++;
		else
			break;
	}
}

void MutexSleep(byte *array)
{
    int counter = 0;
	while (true)
	{
		mtx.lock();
		counter = counterMutex++;
		mtx.unlock();
		if (counter < arrSize)
		{
			(*(array + counter))++;
			this_thread::sleep_for(chrono::nanoseconds(10));
		}
		else
			break;
	}
}

byte AllFunction(byte *array, unsigned int NumThreads)
{
	thread *threadArray = new thread[NumThreads];

	chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
	cout << "Mutex:\n";
	for (int i = 0; i < NumThreads; i++)
	{
		threadArray[i] = thread(Mutex, array);
		threadArray[i].join();
	}
	chrono::high_resolution_clock::time_point finish = chrono::high_resolution_clock::now();
	Print(NumThreads, chrono::duration_cast<chrono::milliseconds>(finish - start).count());
	counterAtomic = 0;
	counterMutex = 0;

	cout << "Atomic:" << endl;
	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < NumThreads; i++)
	{

		threadArray[i] = thread(Atomic, array);
		threadArray[i].join();
	}
    finish = chrono::high_resolution_clock::now();
	Print(NumThreads, chrono::duration_cast<chrono::milliseconds>(finish - start).count());
	counterAtomic = 0;
	counterMutex = 0;

	cout << "Mutex Sleep:" << endl;
    start = chrono::high_resolution_clock::now();
	for (int i = 0; i < NumThreads; i++)
	{
		threadArray[i] = thread(MutexSleep, array);
		threadArray[i].join();
	}
    finish = chrono::high_resolution_clock::now();
	Print(NumThreads, chrono::duration_cast<chrono::milliseconds>(finish - start).count());
	counterAtomic = 0;
	counterMutex = 0;

	cout << "Atomic Sleep:" << endl;
	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < NumThreads; i++)
	{

		threadArray[i] = thread(AtomicSleep, array);
		threadArray[i].join();
	}
    finish = chrono::high_resolution_clock::now();
	Print(NumThreads, chrono::duration_cast<chrono::milliseconds>(finish - start).count());
	counterAtomic = 0;
	counterMutex = 0;
	return *array;
}

int main()
{	
	setlocale(LC_ALL, "RUSSIAN");
    int NumTreads[] = {4, 8, 16, 32};
	byte *array = new byte[arrSize];	
	
	for (int i = 0; i < 4; i++)
		AllFunction(array, NumTreads[i]);
}