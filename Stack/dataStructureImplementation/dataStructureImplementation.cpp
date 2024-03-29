
#include "pch.h"
#include <iostream>
#include <fstream>
#include <cassert>

//implementing stack
template<typename T>
class Stack
{
private:
	T* arr;
	unsigned size, capacity;

	//method for copying stack
	void copy(const Stack& tmp)
	{
		size = tmp.size;
		capacity = tmp.capacity;
		strcpy_s(arr, size, tmp.arr);
	}

	//method for resizing stack
	void extend()
	{
		T* tmp = arr;
		capacity = capacity * 2;
		arr = new T[capacity];
		for (unsigned i = 0; i < size; ++i)
		{
			arr[i] = tmp[i];
		}
		delete[] tmp;
	}


public:
	Stack() : size(0), capacity(1)
	{
		assert(capacity > 0);
		arr = new T(capacity);
	}

	Stack(const Stack& other)
	{
		copy(other);
	}

	~Stack()
	{
		delete[] arr;
	}

	Stack& operator=(const Stack& other)
	{
		if (this != &other)
		{
			delete[] arr;
			copy(other);
		}

		return *this;
	}

	void push(const T& newElement)
	{
		if (size == capacity)
		{
			extend();
		}

		arr[size++] = newElement;
	}

	void pop()
	{
		assert(size);
		size--;
	}

	//returns the top element of the stack
	T top() const
	{
		assert(size);
		return arr[size - 1];
	}

	bool isEmpty() const
	{
		return !size;
	}

	unsigned getSize()
	{
		return size;
	}
};


