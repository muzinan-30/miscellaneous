#pragma once
#pragma once
//˳��ṹʵ��
//����˳���ʾ
//��0�±꿪ʼ�洢����
#include<assert.h>
template<class T>
class Stack {
public:
	Stack(int sz = 10);
	~Stack() { delete[] elements; }
	void Push(const T& item);//ѹջ
	T Pop();//��ջ
	T GetTop();//ջ��
	void MakeEmpty() { top = -1; };//�ÿ�ջ
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