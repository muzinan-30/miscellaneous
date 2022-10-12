#pragma once
#include<assert.h>
template<class T>
class Queue;
template<class T>
class QueueNode {
	friend class Queue<T>;
private:
	T data;
	QueueNode<T>* next;
	QueueNode(T d = 0, QueueNode* node = nullptr) :data(d), next(node) {}
};
template<class T>
class Queue {
public:
	Queue() :front(nullptr), rear(nullptr),counter(0) {};
	~Queue();
	void EnQueue(const T& item);//入队
	T DeQueue();//队头出队列
	T GetQueue();//取出队头的元素
	int IsEmpty() const { return front == nullptr; };
	//int IsFull() const;
	int counter;
private:
	QueueNode<T>* front, * rear;
};

//析构函数,删除每一个结点
template<class T>
Queue<T>::~Queue() {
	QueueNode<T>* p;
	while (front != nullptr) {
		p = front;
		front = front->next;
		delete p;
	}
	counter = 0;
}
template<class T>
void Queue<T>::EnQueue(const T& item) {
	if (IsEmpty()) {
		front = rear = new QueueNode<T>(item, nullptr);
	}
	else { //队列不为空
		rear = rear->next = new QueueNode<T>(item, nullptr);
	}
	counter++;
}
template<class T>
T Queue<T>::DeQueue() {
	assert(!IsEmpty());
	QueueNode<T>* p = front;
	T retvalue = p->data;
	front = front->next;
	if (front == nullptr) {
		rear = nullptr;
	}
	delete p;
	counter--;
	return retvalue;
}
template<class T>
T Queue<T>::GetQueue() {
	assert(!IsEmpty());
	T value = front->data;
	return value;
}

