#include<iostream>
#include<math.h>
#include"Mystack.h"
using namespace std;

class Calculator {
public:
	Calculator(int sz) :s(sz) {}//���캯��
	void run();//����
	void clear() { s.clear(); c.clear(); }

private:
	void meetNext(char oper);//�����ַ�
	void addOperator(char c);//�������ջ
	void addOperand(int value);//��������ջ
	void meetOperator(char oper);//ȡ�������
	int getPriority(char oper);//��ȡ���ȼ�
	void calculate(Stack<int>& oped, char oper);//����
	bool getTwoOperands(int& left, int& right);//��ȡ����������

	Stack<int> s;//������ջ
	Stack<char> c;//�����ջ
};

//�����ַ�
void Calculator::meetNext(char oper)
{
	int temp;
	if (isdigit(oper) != 0)//��������
	{
		cin.putback(oper);
		cin >> temp;
		addOperand(temp);
	}
	else if (isalpha(oper) != 0)//������ĸ
	{
		char temp1, temp2;
		cin >> temp1;
		cin >> temp2;
		if (oper == 'a' && temp1 == 'b' && temp2 == 's')//����abs
		{
			c.push('#');
		}
		else//����abs���������ĸ������
		{
			cout << "input error" << endl;
			this->clear();
			exit(1);
		}
	}
	else if (oper == '+' || oper == '*' || oper == '/' || oper == '%' || oper == '^' || oper == '#' || oper == '-')//������abs����������
	{
		meetOperator(oper);
	}
	else if (oper == '(')
	{
		c.push(oper);
	}
	else if (oper == ')')
	{
		oper = c.getTop();
		while (oper != '(' && !c.isEmpty())
		{
			calculate(s, oper);
			c.pop();
			if (!c.isEmpty())
				oper = c.getTop();
		}
		if (c.isEmpty())
		{
			cout << "NULL" << endl;
			exit(1);
		}
		c.pop();
		if (!c.isEmpty() && c.getTop() == '#')
		{
			if (s.getTop() < 0)
			{
				temp = s.getTop();
				s.pop();
				s.push(-temp);
			}
			c.pop();
		}
	}
	else
	{
		cout << "Error expection" << endl;
		this->clear();
		exit(1);
	}
}

//�������ջ
void Calculator::addOperator(char oper)
{
	c.push(oper);
}

//��������ջ
void Calculator::addOperand(int value)
{
	s.push(value);
}

//ȡ�������
void Calculator::meetOperator(char oper)
{
	if (oper == '-')
	{
		int temp;
		cin >> temp;
		temp = -temp;
		s.push(temp);
	}
	else if (c.isEmpty())
	{
		addOperator(oper);
	}
	else
	{
		if (getPriority(oper) > getPriority(c.getTop()))//��ǰ��������ȼ���ջ��������ߣ���ջ
		{
			addOperator(oper);
		}
		else//��ǰ��������ȼ�С�ڻ����ջ�����������������
		{
			while (!c.isEmpty() && getPriority(oper) <= getPriority(c.getTop()))
			{
				calculate(s, c.getTop());
				c.pop();
			}
			c.push(oper);
		}
	}
}

//��ȡ���ȼ�
int Calculator::getPriority(char oper)
{
	//Խ�����ȼ�Խ��
	int priority = -1;
	if (oper == '^')
	{
		priority = 4;
	}
	else if (oper == '*' || oper == '/' || oper == '%')
		priority = 3;
	else if (oper == '+')
		priority = 2;
	else if (oper == '(')
		priority = 1;
	return priority;
}

//����
void Calculator::calculate(Stack<int>& oped, char oper)
{
	int left;
	int right;

	//others
	bool ret;
	ret = getTwoOperands(left, right);

	if (ret)
	{
		switch (oper)
		{
		case'+':
			s.push(left + right);
			break;
		case'*':
			s.push(left * right);
			break;
		case'/':
			if (right == 0)
			{
				cout << "NULL" << endl;
				this->clear();
				exit(1);
			}
			s.push(left / right);
			break;
		case'%':
			s.push(left % right);
			break;
		case'^':
			if (left == 0 && right == 0)
			{
				cout << "NULL" << endl;
				this->clear();
				exit(1);
			}
			s.push(pow(left, right));
			break;
		}
	}
}

//��ȡ����������
bool Calculator::getTwoOperands(int& left, int& right)
{
	if (s.isEmpty())
	{
		cout << "Error expression" << endl;
		this->clear();
		return false;
	}
	right = s.getTop();
	s.pop();
	if (s.isEmpty())
	{
		cout << "Error expression" << endl;
		this->clear();
		return false;
	}
	left = s.getTop();
	s.pop();
	return true;
}

//����
void Calculator::run() {
	char ch;
	cin >> ch;
	while (ch != '=') {
		meetNext(ch);
		cin >> ch;
	}
	if (!c.isEmpty() && c.getTop() == '(')
	{
		cout << "NULL" << endl;
		this->clear();
		exit(1);
	}
	while (!c.isEmpty())
	{
		char oper = c.getTop();
		c.pop();
		calculate(s, oper);
	}
	cout << s.getTop() << endl;
}