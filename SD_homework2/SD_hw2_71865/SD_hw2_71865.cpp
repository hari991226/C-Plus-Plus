
#include "pch.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include <string>

template<typename T>
class Stack
{
private:
	T* arr;
	unsigned size, capacity;

	void copy(const Stack& tmp)
	{
		size = tmp.size;
		capacity = tmp.capacity;
		strcpy_s(arr, size, tmp.arr);
	}

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
		arr = new T[capacity];
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

std::string decompress(std::string compressedString)
{
	std::string tmpString = "";
	std::string toAdd = "";
	Stack<int> numbersConteiner;
	Stack<std::string> stringContainer;
	int number = 0;
	int bracketCount = 0;

	for (std::string::size_type i = 0; i < compressedString.size(); ++i)
	{
		if (compressedString[i] >= '1' && compressedString[i] <= '9')
		{
			stringContainer.push(tmpString);
			tmpString.clear();
			number = (int)compressedString[i] - 48;
			i++;
			for (std::string::size_type j = i; j < compressedString.size(); ++j)
			{
				if (j == compressedString.size() - 1)
				{
					std::cout << "Invalid String! ";
					exit(1);
				}
				if (compressedString[j] >= '0' && compressedString[j] <= '9')
				{
					number = number * 10 + ((int)compressedString[i] - 48);
					i++;
				}
				else if (compressedString[i] == '(')
				{
					bracketCount++;
					numbersConteiner.push(number);
					break;
				}
				else if (j == compressedString.size() - 1)
				{
					std::cout << "Invalid String! ";
					exit(1);
				}
				else
				{
					std::cout << "Invalid String! ";
					exit(1);
				}
			}
		}
		else if (compressedString[i] == '\\')
		{
			i++;
			tmpString = tmpString + compressedString[i];
		}
		else if (compressedString[i] == '"')
		{
			i++;
			for (std::string::size_type j = i; j < compressedString.size(); ++j)
			{
				if (compressedString[j] == '"')
				{
					break;
				}
				if (j == compressedString.size() - 1)
				{
					std::cout << "Invalid String! ";
					exit(1);
				}
				if (compressedString[i] == '\\')
				{
					j++;
					tmpString = tmpString + compressedString[j];
					i++;
				}
				else 
				{
					tmpString = tmpString + compressedString[j];
					i++;
				}
			}
		}
		else if (compressedString[i] >= 'a' && compressedString[i] <= 'z' || compressedString[i] >= 'A' && compressedString[i] <= 'Z')
		{
			tmpString = tmpString + compressedString[i];
		}
		else if (compressedString[i] == ')')
		{
			bracketCount--;
			if (bracketCount < 0)
			{
				std::cout << "Invalid string! " << std::endl;
				exit(1);
			}
			number = numbersConteiner.top();
			numbersConteiner.pop();
			for (int k = 0; k < number; ++k)
			{
				toAdd = toAdd + tmpString;
			}
			tmpString = stringContainer.top();
			stringContainer.pop();
			tmpString = tmpString + toAdd;
			toAdd.clear();
		}
		else
		{
			std::cout << "Invalid String! ";
			exit(1);
		}
	}
		if (bracketCount != 0)
		{
			std::cout << "Invalid String! ";
			exit(1);
		}
		return tmpString;
}



int main(int argc, char* argv[])
{
	std::ifstream infile(argv[1]);
	if (!infile)
	{
		std::cout << "Can not open file! " << std::endl;
		exit(1);
	}
	std::string hui;
	getline(infile, hui);
	infile.close();
	std::ofstream outfile(argv[2], std::ios::trunc);
	if (!outfile)
	{
		std::cout << "Cannot open file! " << std::endl;
		exit(1);
	}
	std::cout << decompress(hui);
	outfile << decompress(hui);
	outfile.close();
}

