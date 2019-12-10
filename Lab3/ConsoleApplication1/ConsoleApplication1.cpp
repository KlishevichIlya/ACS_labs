#include "pch.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <omp.h>


using namespace std;

 double one_branch(const vector<int>& vect1, const vector<int>& vect2)
{
	auto start = chrono::high_resolution_clock::now();
	vector<int> result(vect1.size(), 0);
	int counter = 0;
	for (int i = 0; i < vect1.size(); i++)
	{
		counter += vect1[i] * vect2[i];
	}
	auto finish = chrono::high_resolution_clock::now();
	double first_time = chrono::duration_cast<chrono::nanoseconds>(finish - start).count() * 1e-9;
	cout << "Сумма при одном потоке:" << counter;
	cout << endl << "Время выполнения одного потока: "<< first_time << "сек."<< endl;
	cout << "<--------------------------------------------------------------->"<<endl;
	return first_time;
}

double parallel_branchs(const vector<int>& v1, const vector<int>& v2)
{
	auto start = chrono::high_resolution_clock::now();
	int counter = 0;
#pragma omp parallel for reduction(+:ans) schedule(static)
	for (int i = 0; i < v1.size(); i++)
	{
		counter += v1[i] * v2[i];
	}
	auto finish = chrono::high_resolution_clock::now();
	double second_time = chrono::duration_cast<chrono::nanoseconds>(finish - start).count() * 1e-9;
	cout <<"Сумма при параллельном вычислении:" << counter;
	cout << endl << "Время параллельного выполнения: "<< second_time << "сек."<< endl;
	return second_time;
}

int main()
{
	setlocale(LC_ALL, "rus");
	unsigned int n = 10000000;
	vector<int> a(n, 1), b(n, 2);
	double branch1 = one_branch(a, b);
	double branch2 = parallel_branchs(a, b);
	cout << "Параллельные потоки работают быстрее в " << branch1 / branch2 << " раз" <<endl;	
	return 0;
}