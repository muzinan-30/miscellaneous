#pragma once
#ifndef  CONNECTSERVER_H_
#define CONNECTSERVER_H_
#include<winsock2.h> //�����ͷ�ļ�
#pragma comment(lib,"ws2_32.lib")//���ļ�
#include<string>
#include<iostream>

#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

using namespace std;

class connectServer
{
public:
	connectServer();
	~connectServer();
	int connectTo(string ip_addr, short port);
	static void closeSocket(int sockfd);

private:
	struct sockaddr_in addr = {0};
};

#endif // ! CONNECTSERVER_H_
