
#include "pch.h"
#include <iostream>

template<typename T>
class Queue
{
private:
	T* arr;
	int first;
	int last;
	unsigned capacity, size;


	//method for copying queue
	void copy(const Queue& other)
	{
		size = other.size;
		capacity = other.capacity;
		first = 0;
		last = other.last;
		arr = new T[capacity];

		for (unsigned i = 0; i < size; ++i)
		{
			arr[i] = other.arr[(other.first + i) % capacity];
		}

	}

	//resizing method
	void resize()
	{
		T* tmp = arr;
		capacity *= 2;
		arr = new T[capacity];

		for (unsigned i = 0; i < size; ++i)
		{
			arr[i] = tmp[(first + i) % capacity];
		}

		first = 0;
		last = size;
		delete[] tmp;
	}

public:
	Queue(): first(0), last(0), capacity(3), size(0)
	{
		arr = new T[capacity];
	}

	~Queue()
	{
		delete[] arr;
	}

	Queue(const Queue& other)
	{
		copy(other);
	}

	Queue& operator=(const Queue& other)
	{
		if (this != &other)
		{
			delete[] arr;
			copy(other);
		}

		return *this;
	}

	bool isEmpty() const
	{
		return !size;
	}

	void push(const T& newElem)
	{
		if (size == capacity)
		{
			resize();
		}
		arr[last] = newElem;
		last = (last + 1) % capacity;
		size++;
	}

	void pop()
	{
		if (isEmpty())
		{
			throw "empty queue";
		}

		first = (first + 1) % capacity;
		size--;
	}

	const T& peek() const
	{
		if (isEmpty())
		{
			throw "empty queue";
		}

		return arr[first];
	}

	void print(int i = 0)
	{
		static unsigned tmp = size;
		std::cout << arr[(first + i)%capacity];
		tmp--;
		if (tmp == 0)return;
		return print(++i);
	}

};

int main()
{
	Queue <char> tmp;
	tmp.push('k');
	tmp.push('2');
	tmp.push('\\');
	tmp.push('4');
	tmp.push('k');
	tmp.push('(');
	tmp.push('k');
	tmp.push('*');
	tmp.push('c');
	tmp.pop();
	tmp.pop();
	tmp.pop();
	tmp.print();
}

