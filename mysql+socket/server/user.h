#pragma once

#ifndef  USER_H_
#define USER_H_
#include<winsock2.h> //�����ͷ�ļ�
#pragma comment(lib,"ws2_32.lib")//���ļ�

#include <string>
using namespace std;

struct user_info {
    char user_name[11] = { 0 };
    char passwd[33] = { 0 };
    int flag;
};

class User
{
	
	
public:
    User(){};
    User(string id, string name, int csockfd);
    ~User();

    string id;
    string name;
    int csockfd;
};


#endif // ! USER_H_
