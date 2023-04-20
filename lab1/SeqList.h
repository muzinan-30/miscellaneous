#pragma once
template<class Type>
/*
顺序表定义和实现
*/
class SeqList
{
public:
	Type* data;//顺序表存储数组
	int MaxSize;//最大允许长度
	int last;//当前最后元素下标
	//遵从C++约定，从0开始存储，用-1表示不存在位
public:
	SeqList(int MaxSize = defaultSize);
	~SeqList() { delete[]data; }
	int Length()const { return last + 1; }
	int Find(Type& x)const;
	int IsIn(Type& x);
	int Insert(Type& x, int i);
	int Remove(Type& x);
	int Next(Type& x);
	int Prior(Type& x);
	int IsEmpty() { return last == -1; }
	int ISFull() { return last == MaxSize - 1; }
	Type Get(int i)
	{
		return i<0 || i>last ? NULL : data[i];
	}//算法时间复杂度为O（1），顺序表具有随机存取特性
};
