#include<iostream>
#include<string>
#include"stack.h"
#include"queue.h"
using namespace std;
class Car {
public:
	char name;
	int time;
	Car(char s=' ', int t = 0) :name(s), time(t) {}
};
void park() {
	int n;
	cout << "����ͣ����������" << endl;
	cin >> n;
	Stack<Car> s(n);        //����
	Stack<Car> s1(100);  //ͣ�ڱ���ϵĳ�
	Car* c;

	cout << "���������е���������:" << endl;
	char flag;
	char name;
	int time;
	char ch;
	cin >> ch;
	while (ch != 'p') {
		cin >> ch;
		cin >> flag;
		cin >> ch;
		cin >> ch;
		cin >> name;
		cout << name << endl;
		cin >> ch;
		cin >> time;
		cout << time << endl;
		cin >> ch; //')'		
		if (flag == 'A') {
			c = new Car(name, time);
			if (!s.IsFull()) {
				s.Push(*c);
				cout << "���ƺ�Ϊ " << name << " ͣ����ͣ�����ڵĵ� " << s.top + 1 << " ��λ��" << endl;
			}
			else {
				s1.Push(*c);
				cout << "���ƺ�Ϊ " << name << " ͣ���˱���ڵĵ� " << s1.top+1 << " ��λ��" << endl;
			}
		}
		else if (flag == 'D') {
			int i = 0;
			int sum = s.top;
			while (i < sum) {
				if (name == s.elements[i].name) break;
				i++;
			}
			if (i > sum) cout << "���޴˳�" << endl;
			else {
				//���ó�֮��ĳ�����, ������
					while (i < sum) {
						//���ó�֮������г�����.
							i++;
						s1.Push(s.Pop());
					}
				Car c = s.Pop();
				if((time - c.time)>0)
				cout << "���ƺ�Ϊ " << c.name << " �ĳ�ѡ���뿪����,��פͣ�� " << time - c.time << " ����" << endl;
				else {
					cout << "�ƺ��������쳣" << endl;
					exit(1);
				}
				//������ϵĳ����
					if (s.top < s.MaxSize) {
						//�ж��Ƿ������г�, �����б���ĳ����복��
							while (!s1.IsEmpty()) {
								if (!s.IsFull()) s.Push(s1.Pop());//�����ʱ������λ��,������ĳ����복��,����ȴ�
								else break;
							}
					}
			}
		}
		else {
			cout << "��ӭ�´ι��ٱ�ͣ����" << endl;
			break;
		}
		cin >> ch;//','
		cin >> ch;
	}
	
}
//int main() {
//	park();
//}