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
	void EnQueue(const T& item);//���
	T DeQueue();//��ͷ������
	T GetQueue();//ȡ����ͷ��Ԫ��
	int IsEmpty() const { return front == nullptr; };
	//int IsFull() const;
	int counter;
private:
	QueueNode<T>* front, * rear;
};

//��������,ɾ��ÿһ�����
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
	else { //���в�Ϊ��
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

