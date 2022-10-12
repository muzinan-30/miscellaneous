#include<iostream>
#include<algorithm>
#include<string>
#include<stdio.h>
#include <stdlib.h> 
#include <time.h>
#include<fstream>
#include<vector>
#include"sort.h"
using namespace std;
/*���ַ�����ʽת��Ϊtime_t*/
time_t FormatTime(string szTime)
{   
    struct tm tm1;
    time_t time1;

    tm1.tm_year = (szTime[0] - '0') * 1000 + (szTime[1] - '0') * 100 + (szTime[2]-'0') * 10 + (szTime[3] - '0') * 1;
    tm1.tm_mon = (szTime[4] - '0') * 10 + (szTime[5] - '0') * 1;
    tm1.tm_mday = (szTime[6] - '0') * 10 + (szTime[7] - '0') * 1;
    tm1.tm_hour = (szTime[8] - '0') * 10 + (szTime[9] - '0') * 1;
    tm1.tm_min= (szTime[10] - '0') * 10 + (szTime[11] - '0') * 1;
    tm1.tm_sec= (szTime[12] - '0') * 10 + (szTime[13] - '0') * 1;
    tm1.tm_year -= 1900;
    tm1.tm_mon--;
    tm1.tm_isdst = -1;
    time1 = mktime(&tm1);
    return time1;
}
//��ʾ���������и�����Ʒ����Ϣ
void showVec(vector<Advertisement*>  vec,int sum) {
    for (int i = 0; i < sum;i++) {
        vec[i]->show();
    }
}

int min(int x, int y) {
    return (x < y) ? x : y;
}
//����ȽϺ���
int compare(string s1, string s2) {
    if (s1 >= s2) return 1;
    else return 0;
}
//�����Ƚ�
int compare(int a1, int a2) {
    if (a1 >= a2) return 1;
    else return 0;
}
//ʱ�����͵ıȽ�
int compare(time_t time1, time_t time2) {
    if (time1 >= time2) return 1;
    else return 0;
}

vector<Advertisement*> Merge(vector<Advertisement*>& data, int begin, int mid, int end,int pattern) {
    int n1 = mid - begin + 1;
    int n2 = end - mid;
    vector<Advertisement*>  left(n1), right(n2);
    for (int i = 0; i < n1; i++) {
        left[i] = data[begin + i];
    }
    for (int i = 0; i < n2; i++) {
        right[i] = data[mid + i + 1];
    }
    int i = 0, j = 0, k = begin;
    while (i < n1 && j < n2) {
        //�޸�
        switch (pattern) {
        case 1:
            if (compare(left[i]->name, right[j]->name) == 0) {
                data[k++] = left[i++];
            }
            else {
                data[k++] = right[j++];
            }
            break;
        case 2:
            if (compare(left[i]->quantity, right[j]->quantity) == 0) {
                data[k++] = left[i++];
            }
            else {
                data[k++] = right[j++];
            }
            break;
        case 3:
            if (compare(left[i]->email, right[j]->email) == 0) {
                data[k++] = left[i++];
            }
            else {
                data[k++] = right[j++];
            }
            break;
        case 4:
            if (compare(left[i]->starttime, right[j]->starttime) == 0) {
                data[k++] = left[i++];
            }
            else {
                data[k++] = right[j++];
            }
            break;
        case 5:
            if (compare(left[i]->endtime, right[j]->endtime) == 0) {
                data[k++] = left[i++];
            }
            else {
                data[k++] = right[j++];
            }
            break;
        default:
            cout << "������ıȽ�ָ���������" << endl;
        }       
    }
    //��������ڣ����ں���
    while (i < n1) {
        data[k++] = left[i++];
    }
    while (j < n2) {
        data[k++] = right[j++];
    }
    return data;
}

vector<Advertisement*> mergeSort(vector<Advertisement*> data, int len,int pattern) 
{
    vector<Advertisement*> res;
    if (data.empty()) {
    return res;
    }
    unsigned int i = 1;
    while (i < len) {
    for (unsigned int begin = 0; begin < len; begin += 2 * i) {
        int mid = begin + i - 1;
        int end = min(begin + 2 * i - 1, len - 1);
        res = Merge(data, begin, mid, end,pattern);
        }
    i = 2 * i;
    }
    return res;
}

void sort() {
    int pattern = 0, sum = 0;
    vector<Advertisement*> vec;
    string et, st, cname;
    Advertisement* c = new Advertisement();
    ifstream in;
    in.open("input111.txt", ios::in);
    if (!in.is_open()) {
        cout << "���ļ�����" << endl;
        exit(1);
    }
    ofstream out;
    out.open("output111.txt", ios::out);
    if (!out.is_open()) {
        cout << "���ļ�����" << endl;
        exit(1);
    }
    int j = 0;
    while (in >> cname) {
        Advertisement* c = new Advertisement();
        c->name = cname;
        if (in >> c->quantity) {}
        else break;
        in >> c->email;
        in >> st;
        in >> et;
        time_t t1 = FormatTime(st);
        time_t t2 = FormatTime(et);
        c->starttime = t1;
        c->endtime = t2;
        vec.push_back(c);
        sum++;
    }
    //�Ƚϵı�׼
    if (cname == "Title") pattern = 1;
    else if (cname == "quantity") pattern = 2;
    else if (cname == "email") pattern = 3;
    else if (cname == "starttime") pattern = 4;
    else pattern = 5;
    cout << "�Ƚϵı�׼Ϊ��" << pattern << endl;
    //���˳��
    showVec(vec, sum);
    //�鲢����
    vector<Advertisement*> data = mergeSort(vec, sum, pattern);
    //չʾ�����Ľ��
    showVec(data, sum);
    //�����д���ļ�
    for (int i = 0; i < sum; i++) {
        out << data[i]->name << endl;
        out << data[i]->quantity << endl;
        out << data[i]->email << endl;
        out << data[i]->starttime << endl;
        out << data[i]->endtime << endl;
    }
    //�ر��ļ�
    in.close();
    out.close();
}