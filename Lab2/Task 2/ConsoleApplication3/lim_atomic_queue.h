#include <queue>
#include <atomic>
#include <vector>

using namespace std;
typedef uint8_t byte;

class limited_queue_atomic
{
private:
	unsigned int _size;
	atomic<byte>* _vector;
	atomic<int> _end;
	atomic<int> _start;

public:
	limited_queue_atomic(unsigned int size);
	
	void push(byte val);
	bool pop(byte& val);
};

limited_queue_atomic::limited_queue_atomic(unsigned int size)
{
	_size = size;
	_vector = new atomic<byte>[size];
	_start = 0;
	_end = 0;
	for (int i = 0; i < size; i++)
		_vector[i] = 0;
}


void limited_queue_atomic::push(byte val)
{
	while(true)
	{
		metka:
		auto local_end = _end.load(); 
		byte temp = _vector[local_end % _size];
		if (local_end != _end) 
			goto metka;
		if (local_end == _start + _size)  
			goto metka;
		if (temp == 0)
		{
			if (_vector[local_end % _size].compare_exchange_strong(temp, val))
			{
				_end.compare_exchange_strong(local_end, local_end + 1); 
				return;
			}
		}
		else _end.compare_exchange_strong(local_end, local_end + 1);
	}
}

bool limited_queue_atomic::pop(byte& val)
{
	while(true)
	{
		metka2:
		auto local_start = _start.load(); 
		byte temp = _vector[local_start % _size];
		if (local_start != _start) 
			goto metka2;
		if (local_start == _end.load())             
		{
			this_thread::sleep_for(chrono::milliseconds(10));
			local_start = _start.load(); 
			if (local_start == _end.load()) 
				return false; 
			else if (local_start != _start)
				goto metka2;
		}
	
		if( temp == 0)
			_start.compare_exchange_strong(local_start, local_start + 1);
		else
		{
			if (_vector[local_start % _size].compare_exchange_strong(temp, 0))
			{
				_start.compare_exchange_strong(local_start, local_start + 1);
				val = temp;
				return true;
			}
		}

	}
}
