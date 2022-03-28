#pragma once
template<class T>
#define MAXSIZE 10;
class Queue {
public:
	Queue() {};
	int QueueLength() { return (Q.rear - Q.front + MAXSIZE) % MAXSIZE; };
	bool EnQueue(T e);
	bool DeQueue(T& e);
	bool ClearQueue() {
		front = rear = 0;
		return true;
	}
public:
	T* data;
	T front;
	T rear;
};