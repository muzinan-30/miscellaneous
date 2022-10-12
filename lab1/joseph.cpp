#include<iostream>
using namespace std;
template<class T>
class Listnode {
public:
	T element;
	int index;
	Listnode* next;
	Listnode(const T& theElement, const int& index,Listnode* nextnode = NULL) :element(theElement), next(nextnode),index(index) {};
};
template<class T>
class CircleLinkList {
public:
	Listnode<T>* head;
	Listnode<T>* tail;
	int size;//元素个数
	Listnode<T>* GetPointAt(int pos) {
		Listnode<T>* pNode = NULL;
		if (pos <= 0 || pos > size)
			std::cout << "out of range." << std::endl;   //链表当前位置越界,异常
		else
		{
			pNode = head;            //当前位置满足条件,则一开始在链表头			
			for (int i = 1; i <= pos - 1; ++i)
				pNode = pNode->next;
		}
		return pNode;
	}
public:
	bool isempty() { return size == 0 ? true : false; }
	//删除指定位置的节点
	bool removeAt( int pos);
	//尾部插入一个节点
	void AddBack(T val);
	//展示函数
	void show() {
		Listnode<T>* node = head;
		if (node->next == NULL) cout<<node->element;
		else {
			cout << node->element;
			node = node->next;
			while (node != head) {
				cout << node->element;
				node = node->next;
			}
		}
	};
	void showindex() {
		Listnode<T>* node = head;
		if (node->next == NULL) cout << node->index;
		else {
			cout << node->index;
			node = node->next;
			while (node != head) {
				cout << node->index;
				node = node->next;
			}
		}
	};
	void Clear() {
		const int nums = Size();
		if (!isempty()) {
			for (int k = 1; k < nums; k++) {
				Listnode<T>* temp = head->next;
				cout << head->index;
				delete head;
				head = temp;
				size--;
			}
		}
	};
	CircleLinkList() :head(), tail(), size(0) {};
	~CircleLinkList() { Clear(); }
	int Size() {
		return size;
	};

};

template<class T>
bool CircleLinkList<T>::removeAt(int pos) {
	Listnode<T>* pNode = NULL;
	if (isempty())
	{
		std::cout << "the link list is empty" << std::endl;
		return false;
	}
	if (pos <= 0 || pos > size)
	{
		std::cout << "out of range." << std::endl;
		return false;
	}
	if (size == 1)       //只有1个元素时相当于清空链表
	{
		Clear();
	}
	if (pos == 1)        //并且size!=1, 删除头部元素时 
	{
		pNode = head;
		cout << pNode->index;
		head = head->next;
		tail->next = head;
		delete pNode;
	}
	else if (pos == size)//删除尾节点
	{
		Listnode<T>* pPreNode = GetPointAt(pos - 1);
		pNode = pPreNode->next;
		pPreNode->next = pNode->next;
		cout << pNode->index;
		delete pNode;
		tail = pPreNode;
		tail->next = head;
	}
	else
	{
		Listnode<T>* pPreNode = GetPointAt(pos - 1);
		pNode = pPreNode->next;
		pPreNode->next = pNode->next;
		cout << pNode->index;
		delete pNode;
		head = pPreNode->next;
		tail = pPreNode;
	}
	size--;
}
template<class T>
void CircleLinkList<T>::AddBack(T val)
{
	Listnode<T>* pNode = new Listnode<T>(val,size+1);
	if (isempty())   //链表为空时 
	{
		head = pNode; tail = pNode;
		tail->next = head;
	}
	else
	{
		tail->next = pNode;
		tail = pNode;
		tail->next = head;
	}
	size++;
}

//
//int main() {
//	CircleLinkList<int>* list = new CircleLinkList<int>();
//	int m; int n; int val;
//	cout << "请输入m和n的值" <<endl;
//	cin >> m >> n;
//	cout << "请输入密码:" << endl;
//	for (int i = 0; i < n; i++) {
//		cin >> val;
//		list->AddBack(val);
//	}
//	list->show();
//	list->showindex();
//	cout << "" << endl;
//	while (!list->isempty()) {
//		 Listnode<int>* temp=list->GetPointAt(m % list->size+1);
//		 m = temp->element;
//		 list->removeAt(m % list->size+1);
//	}
//	cout << "------" << endl;
//	list->show();
//}