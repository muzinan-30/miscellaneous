#pragma once
#include<iostream>
using namespace std;
template<class T>
class LinkedList;
template<class T>
class ListNode {
	friend class LinkedList<T>;
	ListNode() :data(nullptr), next(nullptr) {};
	ListNode(T x, ListNode* node) {
		data = x;
		next = node;
	}
public:
	T data;
	ListNode* next;
};

template<class T>
class LinkedList {
public:
	ListNode<T>* first, * last;
public:
	LinkedList() :first(nullptr), last(nullptr) {};
	int Length() const;
	void Append(T* x);
	ListNode<T>* Get(int i);
	bool IsEmpty() { return last == first; }
};

template<class T>
int LinkedList<T>::Length() const
{
	ListNode<T>* p = first;
	int count = 0;
	while (p != nullptr) {
		count++;
		p = p->next;
	}
	return count;
}

template<class T>
void LinkedList<T>::Append(T* x)
{
	ListNode<T>* newnode = new ListNode<T>(*x, nullptr);
	if (first != nullptr) {
		last->next = newnode;
		last = newnode;
	}
	else {
		first = last = new ListNode<T>(*x,NULL);
		first->next = newnode;
		last = newnode;
	}
}

template<class T>
ListNode<T>* LinkedList<T>::Get(int i) {
	int len = Length();
	if (i > len) return NULL;
	int count = 0;
	ListNode<T>* p = first;
	for (count = 1; count < i; count++) {
		p = p->next;
	}
	return p;
}

