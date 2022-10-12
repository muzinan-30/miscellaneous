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
	cout << "输入停车场的容量" << endl;
	cin >> n;
	Stack<Car> s(n);        //车库
	Stack<Car> s1(100);  //停在便道上的车
	Car* c;

	cout << "请输入所有的数据序列:" << endl;
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
				cout << "车牌号为 " << name << " 停在了停车场内的第 " << s.top + 1 << " 个位置" << endl;
			}
			else {
				s1.Push(*c);
				cout << "车牌号为 " << name << " 停在了便道内的第 " << s1.top+1 << " 个位置" << endl;
			}
		}
		else if (flag == 'D') {
			int i = 0;
			int sum = s.top;
			while (i < sum) {
				if (name == s.elements[i].name) break;
				i++;
			}
			if (i > sum) cout << "查无此车" << endl;
			else {
				//将该车之后的车出库, 进入便道
					while (i < sum) {
						//将该车之后的所有车出库.
							i++;
						s1.Push(s.Pop());
					}
				Car c = s.Pop();
				if((time - c.time)>0)
				cout << "车牌号为 " << c.name << " 的车选择离开车库,共驻停了 " << time - c.time << " 分钟" << endl;
				else {
					cout << "似乎出现了异常" << endl;
					exit(1);
				}
				//将便道上的车入库
					if (s.top < s.MaxSize) {
						//判断是否便道车有车, 将所有便道的车进入车库
							while (!s1.IsEmpty()) {
								if (!s.IsFull()) s.Push(s1.Pop());//如果此时出库有位置,将便道的车进入车库,否则等待
								else break;
							}
					}
			}
		}
		else {
			cout << "欢迎下次光临本停车场" << endl;
			break;
		}
		cin >> ch;//','
		cin >> ch;
	}
	
}
//int main() {
//	park();
//}