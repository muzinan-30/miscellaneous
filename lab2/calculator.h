#include<iostream>
#include<math.h>
#include"Mystack.h"
using namespace std;

class Calculator {
public:
	Calculator(int sz) :s(sz) {}//构造函数
	void run();//运行
	void clear() { s.clear(); c.clear(); }

private:
	void meetNext(char oper);//读到字符
	void addOperator(char c);//运算符入栈
	void addOperand(int value);//操作数入栈
	void meetOperator(char oper);//取得运算符
	int getPriority(char oper);//获取优先级
	void calculate(Stack<int>& oped, char oper);//计算
	bool getTwoOperands(int& left, int& right);//读取两个操作数

	Stack<int> s;//操作数栈
	Stack<char> c;//运算符栈
};

//读到字符
void Calculator::meetNext(char oper)
{
	int temp;
	if (isdigit(oper) != 0)//读到数字
	{
		cin.putback(oper);
		cin >> temp;
		addOperand(temp);
	}
	else if (isalpha(oper) != 0)//读到字母
	{
		char temp1, temp2;
		cin >> temp1;
		cin >> temp2;
		if (oper == 'a' && temp1 == 'b' && temp2 == 's')//读到abs
		{
			c.push('#');
		}
		else//读到abs外的其他字母，报错
		{
			cout << "input error" << endl;
			this->clear();
			exit(1);
		}
	}
	else if (oper == '+' || oper == '*' || oper == '/' || oper == '%' || oper == '^' || oper == '#' || oper == '-')//读到除abs以外的运算符
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

//运算符入栈
void Calculator::addOperator(char oper)
{
	c.push(oper);
}

//操作数入栈
void Calculator::addOperand(int value)
{
	s.push(value);
}

//取得运算符
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
		if (getPriority(oper) > getPriority(c.getTop()))//当前运算符优先级比栈顶运算符高，入栈
		{
			addOperator(oper);
		}
		else//当前运算符优先级小于或等于栈顶运算符，进行运算
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

//获取优先级
int Calculator::getPriority(char oper)
{
	//越大优先级越高
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

//计算
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

//读取两个操作数
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

//运行
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