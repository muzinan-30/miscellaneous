#include"LinkedList.h"
#include<iostream>
#include<string>
using namespace std;
// 3,-1.2,4,3,5
struct Term {
	double coef;
	int exp;
	Term() :coef(0.0), exp(0) {};
	Term(double c, int e) {
		coef = c; exp = e;
	}
};

class Polynomial {
public:
	Polynomial() {
		char a;
		double b;
		int d;
		int c;
		cin >> d;
		for (int i = 0; i < d; i++) {
			cin >> a;
			cin >> b;
			cin >> a;
			cin >> c;
			poly.Append(new Term(b, c));
			cout << "添加成功" << endl;
		}
	}
	LinkedList<Term> poly;
	void show();
	friend Polynomial& operator+(Polynomial& ah, Polynomial& bh);
	friend Polynomial& operator -(Polynomial& ah, Polynomial& bh);
};

void Polynomial::show() {
	int len = poly.Length();
	int i = 2;
	string s="";
	while (i <= len) {
		ListNode<Term>* temp = poly.Get(i);
		s = s + to_string(temp->data.coef) + "X" + "^" +to_string (temp->data.exp)+"\n";
		i++;
	}
	cout << s;
}

char compare(int a, int b) {
	if (a == b) return '=';
	else if (a > b) return '>';
	else return '<';
}

Polynomial& operator +(Polynomial& ah, Polynomial& bh) {
	ListNode <Term>* pa, * pb, * pc, * p;
	pa = pc = ah.poly.first;//头结点
	pb = p = bh.poly.first;
	pa = pa->next;
	pb = pb->next;
	delete p;
	while (pa != nullptr && pb != nullptr) {
		switch (compare(pa->data.exp, pb->data.exp))
		{
		case '=':
			pa->data.coef = pa->data.coef + pb->data.coef;
			p = pb;
			pb = pb->next;
			delete p;
			if (!pa->data.coef) {
				p = pa;
				pa = pa->next;
				delete p;
			}
			else {
				pc->next = pa;
				pc = pa;
				pa = pa->next;
			}
			break;
		case '<':
			pc->next = pb;
			pc = pb;
			pb = pb->next;
			break;
		case '>':
			pc->next = pa;
			pc = pa;
			pa = pa->next;
			break;
		}
	}

	if (pa !=nullptr) { pc->next = pa; }
	else { pc->next = pb; }
	return ah;
}

Polynomial& operator -(Polynomial& ah, Polynomial& bh) {
	ListNode <Term>* pa, * pb, * pc, * p;
	pa = pc = ah.poly.first;//头结点
	pb = p = bh.poly.first;
	pa = pa->next;
	pb = pb->next;
	delete p;
	while (pa != nullptr && pb != nullptr) {
		switch (compare(pa->data.exp, pb->data.exp))
		{
		case '=':
			pa->data.coef = pa->data.coef - pb->data.coef;
			p = pb;
			pb = pb->next;
			delete p;
			if (!pa->data.coef) {
				p = pa;
				pa = pa->next;
				delete p;
			}
			else {
				pc->next = pa;
				pc = pa;
				pa = pa->next;
			}
			break;
		case '<':
			pb->data.coef = 0 - pb->data.coef;
			pc->next = pb;
			pc = pb;
			pb = pb->next;
			break;
		case '>':
			pc->next = pa;
			pc = pa;
			pa = pa->next;
			break;
		}
	}
	if (!pa) pc->next = pa;
	else pc->next = pb;
	return ah;
}

int main() {
	cout << "请输入第一个多项式:" << endl;
	Polynomial* a = new Polynomial();
	cout << "请输入第二个多项式:" << endl;
	Polynomial* b = new Polynomial();
	cout << "" << endl;
	a->show();
	cout << "" << endl;
	b->show();

	Polynomial* c;
	int flag;
	cout << "请选择你要进行的操作：（1.相加 2.相减）：" << endl;
	cin >> flag;
	switch (flag) {
	case 1:
		c =&(*a + *b);
		c->show();
		break;
	case 2:
		c = &(*a - *b);
		c->show();
		break;
	default:
		cout << "输入正确的操作" << endl;
		break;
	}
	return 0;
}