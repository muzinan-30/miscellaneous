#pragma once
#include<iostream>
#include"error.h"
using namespace std;

template<class Type>
class Stack
{
private:
	int top;//Ԫ������-1������ջ������ʵ�֣��䵱index
	Type* data;//���ݴ洢����
	int maxSize;
public:
	Stack(int sz = 300);//���캯��������ı��ʽ���ַ��������ᳬ��300
	~Stack() { delete[] data; }//��������
	void clear() { top = -1; }//���ջ,�����index<0
	void push(Type e);//ѹջ
	void pop();//��ջ
	Type getTop();//�õ�ջ��Ԫ��
	bool isEmpty() { return top == -1; }
	bool isFull() { return top == maxSize - 1; }
};

template<class Type>
Stack<Type>::Stack(int sz)
{
	data = new Type[sz];
	if (data == nullptr)//����ռ䴴��ʧ��
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