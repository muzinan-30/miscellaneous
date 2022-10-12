#pragma once
template<class Type>
class SeqList
{
public:
	Type* data;//˳���洢����
	int MaxSize;//���������
	int last;//��ǰ���Ԫ���±�
	//���C++Լ������0��ʼ�洢����-1��ʾ������λ
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
	}//�㷨ʱ�临�Ӷ�ΪO��1�������������ȡ����
};