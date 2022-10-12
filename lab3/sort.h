#pragma once
#include<iostream>
#include<algorithm>
#include<string>
#include<stdio.h>
#include <stdlib.h> 
#include <time.h>
#include<fstream>
#include<vector>
using namespace std;

class Advertisement {
public:
    string name;
    int quantity;
    string email;
    time_t starttime;
    time_t endtime;
    Advertisement() :name(""), quantity(0), email(""), starttime(), endtime() {};
    void show() {
        cout << name << quantity << email << starttime << endtime << endl;
    }
};

vector<Advertisement*> Merge(vector<Advertisement*>& data, int begin, int mid, int end, int pattern);

vector<Advertisement*> mergeSort(vector<Advertisement*> data, int len, int pattern);

void sort();
