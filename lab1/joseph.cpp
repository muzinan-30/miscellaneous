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
	int size;//Ԫ�ظ���
	Listnode<T>* GetPointAt(int pos) {
		Listnode<T>* pNode = NULL;
		if (pos <= 0 || pos > size)
			std::cout << "out of range." << std::endl;   //����ǰλ��Խ��,�쳣
		else
		{
			pNode = head;            //��ǰλ����������,��һ��ʼ������ͷ			
			for (int i = 1; i <= pos - 1; ++i)
				pNode = pNode->next;
		}
		return pNode;
	}
public:
	bool isempty() { return size == 0 ? true : false; }
	//ɾ��ָ��λ�õĽڵ�
	bool removeAt( int pos);
	//β������һ���ڵ�
	void AddBack(T val);
	//չʾ����
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
	if (size == 1)       //ֻ��1��Ԫ��ʱ�൱���������
	{
		Clear();
	}
	if (pos == 1)        //����size!=1, ɾ��ͷ��Ԫ��ʱ 
	{
		pNode = head;
		cout << pNode->index;
		head = head->next;
		tail->next = head;
		delete pNode;
	}
	else if (pos == size)//ɾ��β�ڵ�
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
	if (isempty())   //����Ϊ��ʱ 
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
//	cout << "������m��n��ֵ" <<endl;
//	cin >> m >> n;
//	cout << "����������:" << endl;
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