#include<iostream>
//顺序表部分公共操作的实现
#include"SeqList.h"
//存储结构：采用动态数组
//实现集合的运算
using namespace std;

template<class Type>
SeqList<Type>::SeqList(int sz)//构造函数
{
	if (sz > 0)
	{
		MaxSize = sz;
		last = -1;
		data = new Type[MaxSize];
		if (data == NULL)
		{
			MaxSize = 0;
			last = -1;
			return;
		}
	}
}
//顺序搜索 看是否包含某个元素
template <class Type>
int SeqList<Type>::Find(Type& x)const
{
	//搜索函数：在表中从前向后顺序查找x
	int i = 0;
	while (i <= last && data[i] != x)
		i++;
	if (i > last)
		return -1;//not find
	   else return i;
}

//implementation of insert
template<class Type>
int SeqList<Type>::Insert(Type& x, int i)
{
	if (i<0 || i>last + 1 || last == MaxSize - 1)
		return 0;//insert failed
	else
	{
		last++;
		for (int  j = last; j >i; j--)
			data[j] = data[j - 1];
			data[i] = x;
			return 1;//insert successfully
	}
}
//implementation of the delete
template<class Type>
int SeqList<Type>::Remove(Type& x)
{
	//already have x in the list
	int i = Find(x);//search for x
	if (i >= 0)
	{
		last--;
		for (int j = i; j <= last; j++)
			data[j] = data[j + 1];
		return 1;
	}
		return 0;//not find x
}


//给一个集合去重
template<class Type>
void  check(SeqList<Type>* L) {
	int m = L->Length();
	for (int i = 0; i < m; ) {
		if (!(L->Get(i) >= 97 && L->Get(i) <= 122)) {
			Type x = L->Get(i);
			L->Remove(x);
			m--;
			continue;
		}
		else {
			i++;
		}
	}

	cout << "L集合去掉不是小写字母的元素后：" << endl;
	for (int i = 0; i < L->Length(); i++) {
		cout << L->Get(i);
	}
	cout << endl;
	int n = L->Length();
	int i = 0;
	while (i < n) {
		for (int j = i + 1; j < n; ) {
			if (L->Get(i) == L->Get(j)) {
				Type x = L->Get(j);
				L->Remove(x);
				n--;
				continue;
			}
			else {
				j++;
			}
		}
		i++;
	}
}

//集合的并运算
template<class Type>
SeqList<Type>* Union(SeqList<Type>& LA, SeqList<Type>& LB)
{   
	int n = LA.Length();
	int m = LB.Length();
	SeqList<char>* resSeq = new SeqList<char>(100);
	int index = 0;
	for (int i = 0; i < n; i++) {
		Type x= LA.Get(i);
		resSeq->Insert(x, index);
		index++;
	}
	for (int i = 0; i < m; i++)
	{
		Type x = LB.Get(i);
		if (LA.Find(x)==-1)
		{
			resSeq->Insert(x, index);
			index++;
		}
	}
	return resSeq;
}


//集合的“交”运算
template<class Type>
SeqList<Type>* Intersection(SeqList<Type>& LA, SeqList<Type>& LB)
{
	int n = LA.Length();
	int m = LB.Length();
	int i = 0;
	SeqList<char>* resSeq = new SeqList<char>(100);
	int index = 0;
	while (i <= n)
	{
		Type x = LA.Get(i);//在LA中取一元素
		int k = LB.Find(x);//在LB中搜索它
		if (k == -1)//没找到去到下一位
		{
			i++;
		}//找到了，添加进来后,比较下一位
		else { 
			resSeq->Insert(x, index);
			index++;
			i++; }
	}
	return resSeq;
}

//集合的"差"运算
template<class Type>
SeqList<Type>* difference(SeqList<Type>& LA, SeqList<Type>& LB) {
	int n = LA.Length();
	SeqList<char>* list_Intersection=Intersection(LA, LB);
	SeqList<char>* resSeq = new SeqList<char>(100);
	int index = 0;
	for (int i = 0; i < n; i++) {
		Type x = LA.Get(i);
		if (list_Intersection->Find(x) == -1) {
			resSeq->Insert(x, index);
			index++;
		}
	}
	return resSeq;
}

//测试程序
//int main() {
//	string s1, s2;
//	cout << "输入set1的元素:" << endl;
//	cin >> s1;
//	cout << "输入set2的元素:" << endl;
//	cin>>s2;
//	SeqList<char>  sq1(100);
//	SeqList<char> sq2(100);
//	for (int i = 0; i < s1.length(); i++) {
//		sq1.Insert(s1[i], i);
//	}
//
//	for (int j = 0; j < s2.length(); j++) {
//		sq2.Insert(s2[j], j );
//	}
//	//显示输入的两个集合
//	cout << "你输入的两个集合的元素为：" << endl;
//	for (int i = 0; i < sq1.Length(); i++) {
//		cout << sq1.Get(i);
//	}
//	cout << endl;
//	for (int j = 0; j < sq2.Length(); j++) {
//		cout << sq2.Get(j);
//	}
//	cout << endl;
//	//规范化两个集合中的元素
//	check(&sq1);
//	check(&sq2);
//	//输出两个集合的元素
//	cout << "两个集合的元素去掉不符合要求的元素之后为：" << endl;
//	for (int i = 0; i < sq1.Length(); i++) {
//		cout << sq1.Get(i);
//	}
//	cout << endl;
//	for (int j = 0; j < sq2.Length(); j++) {
//		cout << sq2.Get(j);
//	}
//	cout << endl;
//	//交运算
//	SeqList<char>* result1=Intersection(sq1, sq2);
//	cout << "Set1∩Set2=";
//	for (int i = 0; i < result1->Length(); i++) {
//		cout << result1->Get(i);
//	}
//	//并运算
//	SeqList<char>* result2 = Union(sq1, sq2);
//	cout << endl;
//	cout << "Set1∪Set2=";
//	for (int i = 0; i < result2->Length(); i++) {
//		cout << result2->Get(i);
//	}
//	//差运算
//	SeqList<char>* result3 = difference(sq1, sq2);
//	cout << endl;
//	cout << "Set1-Set2=";
//	for (int i = 0; i < result3->Length(); i++) {
//		cout << result3->Get(i);
//	}
//	
//}