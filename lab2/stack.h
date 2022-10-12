#pragma once
#pragma once
//顺序结构实现
//链的顺序表示
//从0下标开始存储数据
#include<assert.h>
template<class T>
class Stack {
public:
	Stack(int sz = 10);
	~Stack() { delete[] elements; }
	void Push(const T& item);//压栈
	T Pop();//出栈
	T GetTop();//栈顶
	void MakeEmpty() { top = -1; };//置空栈
	int IsEmpty() const { return top == -1; };
	int IsFull() const;
public:
	int top;
	int MaxSize;
	T* elements;
};

template<class T>
Stack<T>::Stack(int maxSize) {
	if (maxSize > 0) {
		elements = new T[maxSize];
		top = -1;
		MaxSize = maxSize;
	}
	else {
		MaxSize = -1;
		top = -1;
	}
}

template<class T>
void Stack<T>::Push(const T& item) {
	assert(!IsFull());
	elements[++top] = item;
}
template<class T>
T Stack<T>::Pop() {
	assert(!IsEmpty());
	return elements[top--];
}
template<class T>
T Stack<T>::GetTop() {
	assert(!IsEmpty());
	return elements[top];
}
template<class T>
int Stack<T>::IsFull() const {
	return top == MaxSize - 1;
}