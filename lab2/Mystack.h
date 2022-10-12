#pragma once
#include<iostream>
#include"error.h"
using namespace std;

template<class Type>
class Stack
{
private:
	int top;//元素总数-1，用于栈的数组实现，充当index
	Type* data;//数据存储类型
	int maxSize;
public:
	Stack(int sz = 300);//构造函数，输入的表达式的字符个数不会超过300
	~Stack() { delete[] data; }//析构函数
	void clear() { top = -1; }//清空栈,数组的index<0
	void push(Type e);//压栈
	void pop();//弹栈
	Type getTop();//得到栈顶元素
	bool isEmpty() { return top == -1; }
	bool isFull() { return top == maxSize - 1; }
};

template<class Type>
Stack<Type>::Stack(int sz)
{
	data = new Type[sz];
	if (data == nullptr)//数组空间创建失败
	{
		throw errorOperationOnStack();
	}
	maxSize = sz;
	top = -1;
}

template<class Type>
void Stack<Type>::push(Type e)
{
	if (isFull())
	{
		throw errorOperationOnStack();
	}
	else
	{
		data[++top] = e;
	}
}

template<class Type>
void Stack<Type>::pop()
{
	if (isEmpty())
	{
		throw errorOperationOnStack();
	}
	else
	{
		top--;
	}
}

template<class Type>
Type Stack<Type>::getTop()
{
	if (isEmpty())
	{
		throw errorOperationOnStack();
	}
	else
	{
		return data[top];
	}
}