#include<iostream>
#include<fstream>
#include<stdio.h>
#include "median.h"
using namespace std;
float median=0;

void  Find1(float a[], int start1, int end1, float b[], int start2, int end2) {
	if ((end1 - start1) == 0) {
		median = (a[end1] + b[end2]) / 2;
		cout << "中位数：" << (a[end1] + b[end2]) / 2 << endl;
		return;
	}
	/*找到两个有序数组中的中位数*/
	float median1;
	float median2;
	if ((end1 - start1) % 2 == 0) median1 = a[(end1 + start1) / 2 ];
	else median1 = (a[(end1 + start1) / 2 ] + a[(end1 + start1) / 2+1 ]) / 2;
	if ((end2 - start2) % 2 == 0) median2 = b[(end2 +start2) / 2];
	else median2 = (b[(end2 + start2) / 2] + b[(end2 + start2) / 2 + 1]) / 2;
	if (median1 == median2) {
		median= median1;
		cout << "中位数为：" << median1 << endl;
	}
	else if (median1 < median2)
	{
		if ((end2 - start2) % 2 == 0)
			Find1(a, (end1 + start1) / 2, end1, b, start2, (end2 + start2) / 2);
		else
			Find1(a, (end1 + start1) / 2 + 1, end1, b, start2, (end2 + start2) / 2);
	}
	else {
		if ((end2 - start2) % 2 == 0)
			Find1(a, start1,(end1 + start1) / 2, b, (end2 + start2) / 2, end2);
		else
			Find1(a,start1 ,(end1 + start1) / 2, b, (end2 + start2) / 2 + 1,end2);
	}
}

void  Find(float a[],int start1,int end1,float b[],int start2,int end2 ){
	if ((end1 - start1) == 0) {
		median = (a[end1] + b[end2]) / 2;
		cout << "中位数：" << (a[end1] + b[end2]) / 2 << endl;
		return;
	}
	/*找到两个有序数组中的中位数*/
	float median1;
	float median2;
	if ((end1 - start1) % 2 == 0) median1 = a[(end1 - start1) / 2+1];
	else median1 = (a[(end1 - start1) / 2+1] + a[(end1 - start1) / 2+2])/2;
	if ((end2 - start2) % 2 == 0) median2 = b[(end2 - start2) / 2+1];
	else median2 = (b[(end2 - start2) / 2+1] + b[(end2 - start2 ) / 2+2])/2;
	if (median1 == median2) {
		median = median1;
		cout << "中位数为：" << median1 << endl;
	}
	else if (median1 < median2)  Find1(a, end1 / 2 + 1, end1, b, start2, (end2+1) / 2);
	else Find1(a, start1, (end1+1)/ 2, b, end2 /2+1, end2);                                                                                                                                                                                                              
}

void Findmedian() {
	int len;
	int i = 1; int j = 0;
	float a[256] = {};
	float b[256] = {};
	ifstream  in;  //创建输入流对象
	ofstream out;//创建输出流对象
	in.open("input.txt", ios::in);
	out.open("output.txt", ios::out | ios::app);
	if (!in.is_open())
	{
		cout << "Error opening file"; exit(1);
	}
	in >> len;
	if (!out.is_open()) {
		cout << "Error opening file"; exit(1);
	}
	while (!in.eof())
	{
		/*in.getline(buffer, 100);*/
		in >> a[i];
		i++;
	}
	for (int i = 1; i <= len; i++) {
		b[i] = a[len + i];
	}
	//输出a，b数组中存储的数据
	for (int i = 1; i <= len; i++) {
		cout << a[i];
	}
	cout << endl;
	for (int i = 1; i <= len; i++) {
		cout << b[i];
	}
	cout << endl;
	Find(a, 1, len, b, 1, len);
	out << median;
	out.close();
	in.close();
}